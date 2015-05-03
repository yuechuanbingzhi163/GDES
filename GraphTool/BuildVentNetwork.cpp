#include "stdafx.h"
#include "BuildVentNetwork.h"

#include "../MineGE/TagGE.h"
#include "../MineGE/LinkedGE.h"
#include "../DefGE/TTunnel.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

//#include <set>
//#include <stack>
#include <vector>
#include <algorithm>

static void FindBlocks( AcDbObjectIdArray& tagObjIds )
{
    DrawHelper::FindMineGEs( _T( "Wall" ), tagObjIds );
    DrawHelper::FindMineGEs( _T( "DoubleGate" ), tagObjIds );
    DrawHelper::FindMineGEs( _T( "BalanceGate" ), tagObjIds );
    //DrawHelper::FindMineGEs(_T("Gate"), tagObjIds);
}

// 查找分支图元：巷道、工作面、硐室等
static void FindEdgeGEs( AcDbObjectIdArray& objIds )
{
    DrawHelper::FindMineGEs( _T( "LinkedGE" ), objIds );
}

static void GetEdgeSEPts( const AcDbObjectIdArray& objIds, AcGePoint3dArray& spts, AcGePoint3dArray& epts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;
        LinkedGE* pEdge = LinkedGE::cast( pObj );

        AcGePoint3d spt, ept;
        pEdge->getSEPoint( spt, ept );

        // 记录始末点坐标
        spts.append( spt );
        epts.append( ept );
    }
    actrTransactionManager->endTransaction();
}

static void GetNodePts( const AcGePoint3dArray& spts, const AcGePoint3dArray& epts, AcGePoint3dArray& pts )
{
    int len = spts.length();
    for( int i = 0; i < len; i++ )
    {
        if( !pts.contains( spts[i] ) ) pts.append( spts[i] );
        if( !pts.contains( epts[i] ) ) pts.append( epts[i] );
    }
}

static void BuildNodesMap( const AcGePoint3dArray& pts, Digraph& dg, NodeArray& nodes )
{
    // 建立点坐标和节点之间的映射
    int len = pts.length();
    for( int i = 0; i < len; i++ )
    {
        nodes.push_back( dg.addNode() );
    }
}

static void BuildEdgesMap( const AcDbObjectIdArray& objIds, const AcGePoint3dArray& pts, Digraph& dg, NodeArray& nodes, EdgeArray& edges )
{
    // 添加分支
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;
        LinkedGE* pEdge = LinkedGE::cast( pObj );

        AcGePoint3d startPt, endPt;
        pEdge->getSEPoint( startPt, endPt );
        Digraph::Node u = nodes[pts.find( startPt )];
        Digraph::Node v = nodes[pts.find( endPt )];
		Digraph::Arc e = dg.addArc( u, v );
        edges.push_back( e );
		//acutPrintf(_T("\n分支e%d(v%d,v%d)-->%ld"),Digraph::id(e),Digraph::id(u),Digraph::id(v), objIds[i].asOldId());
    }
    actrTransactionManager->endTransaction();
}

static void EraseZeroDegreeNodes( Digraph& dg )
{
    // 查找并删除度(出度+入度)等于0的节点
    NodeArray ns;
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        if( ( countOutArcs( dg, n ) + countInArcs( dg, n ) ) == 0 )
        {
            ns.push_back( n );
        }
    }
    int len = ns.size();
    for( int i = 0; i < len; i++ )
    {
        dg.erase( ns[i] ); // 删除空节点(度等于0)
    }
}

// 获取掘进工作面的导风巷道
static bool GetInTunnel( const AcDbObjectId& objId, AcDbObjectId& inTunnel )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    TTunnel* pTT = TTunnel::cast( pObj );
    if( pTT == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

   // inTunnel = pTT->getInTunnel();

    actrTransactionManager->endTransaction();

    return !inTunnel.isNull();
}

// 在objIds中查找掘进工作面，同时查找掘进工作面对应的导风巷道
static void FindTTunnels( const AcDbObjectIdArray& objIds, AcDbObjectIdArray& tObjIds, AcDbObjectIdArray& inTunnels )
{
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        if( !ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objIds[i] ) ) continue;

        AcDbObjectId inTunnel;
        if( !GetInTunnel( objIds[i], inTunnel ) ) continue;

        tObjIds.append( objIds[i] );
        inTunnels.append( inTunnel );
    }
}

// 将分支es的源点变成v
static void ChangeOutArcSourceNode( Digraph& dg, EdgeArray& es, Digraph::Node v )
{
    int len = es.size();
    for( int i = 0; i < len; i++ )
    {
        dg.changeSource( es[i], v );
    }
}

// 将分支es的末点变成v
static void ChangeInArcTargetNode( Digraph& dg, EdgeArray& es, Digraph::Node v )
{
    int len = es.size();
    for( int i = 0; i < len; i++ )
    {
        dg.changeTarget( es[i], v );
    }
}

#define ns_contains(a, x) (std::find(a.begin(), a.end(), x) != a.end())

// 分裂导风巷道的末节点, 在后面再增加一个虚拟节点
// 该方法会修改objIds和edges
static void SplitTargetNode( AcDbObjectIdArray& objIds, Digraph& dg, EdgeArray& edges, const EdgeArray& es )
{
    // 记录导风巷道分支的所有共同的末节点(排除重复情况)
    NodeArray ns;
    int len = es.size();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Node u = dg.target( es[i] );
        if( !ns_contains(ns, u) ) ns.push_back( u );
    }

    // 在导风巷道末节点后面再增加一个虚拟节点
    len = ns.size();
    //acutPrintf( _T( "\n掘进工作面添加%d个虚拟节点" ), len );
    for( int i = 0; i < len; i++ )
    {
        Digraph::Node u = ns[i];
        Digraph::Node v = dg.addNode(); // 新增一个虚拟节点

        EdgeArray outEs;
        for( Digraph::OutArcIt e( dg, u ); e != INVALID; ++e )
        {
            if( !ns_contains( es, e ) ) outEs.push_back( e );
        }

        EdgeArray inEs;
        for( Digraph::InArcIt e( dg, u ); e != INVALID; ++e )
        {
            if( !ns_contains( es, e ) ) inEs.push_back( e );
        }

        // 修改节点u的所有出边分支(不包括导风巷道分支)的始点坐标
        ChangeOutArcSourceNode( dg, outEs, v );

        // 修改节点u的所有入边分支(不包括导风巷道分支)的末点坐标
        ChangeInArcTargetNode( dg, inEs, v );

        // 增加虚拟分支,对应id为null
        objIds.append( AcDbObjectId::kNull );
        edges.push_back( dg.addArc( u, v ) );
    }
}

static void SplitInTunnelNode( AcDbObjectIdArray& objIds, AcDbObjectIdArray& tObjIds, AcDbObjectIdArray& inTunnels, Digraph& dg, EdgeArray& edges )
{
    EdgeArray es;
    int len = tObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        int pos = objIds.find( inTunnels[i] );
        es.push_back( edges[pos] );
    }
    SplitTargetNode( objIds, dg, edges, es );
}

// 将导风巷道的末节点和增加的虚拟节点连接起来
static void LinkInTunnelToTTunnel( AcDbObjectIdArray& objIds, AcDbObjectIdArray& tObjIds, AcDbObjectIdArray& inTunnels, Digraph& dg, EdgeArray& edges )
{
    int len = tObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        int p1 = objIds.find( inTunnels[i] );
        int p2 = objIds.find( tObjIds[i] );

        // "导风巷道"的末节点与掘进工作面分支的始节点
        // 之间增加一个虚拟分支(该分支可看做是风筒)
        Digraph::Node u = dg.target( edges[p1] ); // 导风巷道末节点
        Digraph::Node v = dg.source( edges[p2] ); // 掘进工作面始节点

        edges.push_back( dg.addArc( u, v ) );
        objIds.append( AcDbObjectId::kNull );     // 如果考虑"风筒"，id可能不为空???
    }
}

// 掘进工作面与导风巷道之间增加虚拟分支
static void DealWithTTunnel( AcDbObjectIdArray& objIds, Digraph& dg, EdgeArray& edges )
{
    // 查找掘进工作面以及对应的导风巷道(风筒进风巷道)
    AcDbObjectIdArray tObjIds;
    AcDbObjectIdArray inTunnels;
    FindTTunnels( objIds, tObjIds, inTunnels );


    // 导风巷道分割成2段(添加一个虚拟节点，以及一个虚拟分支)
    SplitInTunnelNode( objIds, tObjIds, inTunnels, dg, edges );


    // 连接"风筒"分支-->掘进巷道始节
    LinkInTunnelToTTunnel( objIds, tObjIds, inTunnels, dg, edges );
}

static Digraph::Node AddVirtualSource( Digraph& dg, ObjectIdMap& om )
{
    NodeArray sNodes;  // 源点集合

    // 对网络进行分析,查找源点
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        // 记录入度=0的源点
        if( 0 == countInArcs( dg, n ) ) sNodes.push_back( n );
    }

    //acutPrintf(_T("\n处理阻断分支之前,【源点】个数:%d"), sNodes.length());

    /*
     * !!!前提条件!!!
     * 至少有一个源点和一个汇点
     * 目前暂时不考虑其它的"不正常情况"
     * 例如：没有源点或没有汇点，或者源汇点都没有
     */
    if( sNodes.empty() ) return INVALID;

    // 增加一个虚拟源点
    Digraph::Node sn = dg.addNode();

    // 增加源点"流出分支"(虚拟源点-->源点)
    int len = sNodes.size();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = dg.addArc( sn, sNodes[i] );
        om[e] = AcDbObjectId::kNull;
    }
    return sn;  // 返回新增的虚拟源点
}

static Digraph::Node AddVirtualTarget( Digraph& dg, ObjectIdMap& om )
{
    NodeArray tNodes;  // 源点集合

    // 对网络进行分析,查找汇点
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        // 记录出度=0的汇点
        if( 0 == countOutArcs( dg, n ) ) tNodes.push_back( n );
    }

    //acutPrintf(_T("\n处理阻断分支之前,【汇点】个数:%d"), tNodes.length());

    /*
     * !!!前提条件!!!
     * 至少有一个源点和一个汇点
     * 目前暂时不考虑其它的"不正常情况"
     * 例如：没有源点或没有汇点，或者源汇点都没有
     */

    if( tNodes.empty() ) return INVALID;

    // 增加一个虚拟汇点
    Digraph::Node tn = dg.addNode();

    // 增加汇点"流入分支"(汇点-->虚拟汇点)
    int len = tNodes.size();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = dg.addArc( tNodes[i], tn );
        om[e] = AcDbObjectId::kNull;
    }
    return tn; // 返回新增的虚拟汇点
}

bool BuildNetwork( Digraph& dg, ObjectIdMap& om )
{
    AcDbObjectIdArray objIds;
    //double st = Timer::rdtscSeconds();

    // 1) 查找所有的分支图元
    FindEdgeGEs( objIds );

   // double et = Timer::rdtscSeconds();
   // acutPrintf( _T( "\n查找分支图元耗费时间:%.5fs\n" ), et - st );

    if( objIds.isEmpty() ) return false;

    //st = Timer::rdtscSeconds();

    // 2) 获取所有的分支图元的始末点坐标
    AcGePoint3dArray spts, epts;
    GetEdgeSEPts( objIds, spts, epts );
	//acutPrintf(_T("\n找到的管路条数:%d"),objIds.length());
    if( spts.length() != objIds.length() ) return false;

    // 分析始末点坐标，获取节点坐标
    AcGePoint3dArray pts;
    GetNodePts( spts, epts, pts );

    // 3) 建立点坐标和节点的映射
    NodeArray nodes;
    BuildNodesMap( pts, dg, nodes );

    // 4) 建立图元和分支的映射
    EdgeArray edges;
    BuildEdgesMap( objIds, pts, dg, nodes, edges );

    //et = Timer::rdtscSeconds();
   // acutPrintf( _T( "\n建立网络耗费时间:%.5fs\n" ), et - st );

   // st = Timer::rdtscSeconds();

    // 5) 在掘进工作面和导风巷道之间增加虚拟分支
    DealWithTTunnel( objIds, dg, edges );

    //et = Timer::rdtscSeconds();
    //acutPrintf( _T( "\n处理掘进工作面耗费时间:%.5fs\n" ), et - st );

    // 6) 构造分支对象id映射om
    int len = edges.size();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = edges[i];
        om[e] = objIds[i];
    }

    //acutPrintf( _T( "\n原始图节点个数:%d 分支个数:%d" ), countNodes( dg ), countArcs( dg ) );

    return true;
}

bool AddVirtualSTNode( Digraph& dg, ObjectIdMap& om, Digraph::Node& sn, Digraph::Node& tn )
{
    // 增加一个虚拟源点及分支
    /*Digraph::Node*/ sn = AddVirtualSource( dg, om );

    // 增加一个虚拟汇点及分支
    /*Digraph::Node*/
    tn = AddVirtualTarget( dg, om );

    return ( sn != INVALID ) && ( tn != INVALID );
}

static void GetHosts( const AcDbObjectIdArray& objIds, AcDbObjectIdArray& hosts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        TagGE* pTag = TagGE::cast( pObj );
        if( pTag == 0 ) continue;

        AcDbObjectId host = pTag->getRelatedGE();
        if( !host.isNull() ) hosts.append( host );
    }
    actrTransactionManager->endTransaction();
}

void FilterBlockEdges( Digraph& dg, ObjectIdMap& om, ArcFilter& bf )
{
    // 全部初始化为true
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        bf[e] = true;
    }

    // 查找所有的风门和密闭构筑物
    AcDbObjectIdArray tagObjIds;
    FindBlocks( tagObjIds );
    if( tagObjIds.isEmpty() ) return;

    // 查找构筑物关联的巷道
    AcDbObjectIdArray hosts;
    GetHosts( tagObjIds, hosts );

    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        AcDbObjectId objId = om[e];
        if( objId.isNull() )
        {
            bf[e] = true;
        }
        else
        {
            bf[e] = !hosts.contains( objId );
        }
    }
}

void EraseBlockEdges( Digraph& dg, ArcFilter& bf )
{
    EdgeArray es;
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        if( !bf[e] ) es.push_back( e );
    }

    int len = es.size();
    for( int i = 0; i < len; i++ )
    {
        dg.erase( es[i] );
    }

    //acutPrintf( _T( "\n删除阻断分支后的图节点个数:%d 分支个数:%d" ), countNodes( dg ), countArcs( dg ) );
}

bool GetNodeGeoData( Digraph& dg, ObjectIdMap& om, NodeGeoDataMap& ngm )
{
    // 初始化为原点(0,0,0)
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        ngm[n] = AcGePoint3d::kOrigin;
    }

    // 查找所有的巷道以及对应的分支
    AcDbObjectIdArray objIds;
    EdgeArray es;
    for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
    {
        AcDbObjectId objId = om[e];
        if( !objId.isNull() )
        {
            es.push_back( e );
            objIds.append( objId );
        }
    }

    // 查找所有巷道的始末点坐标
    AcGePoint3dArray spts, epts;
    GetEdgeSEPts( objIds, spts, epts );
    if( spts.length() != objIds.length() ) return false;

    int len = es.size();
    for( int i = 0; i < len; i++ )
    {
        Digraph::Arc e = es[i];

        ngm[dg.source( e )] = spts[i]; // 始节点坐标
        ngm[dg.target( e )] = epts[i]; // 末节点坐标
    }

    return true;
}