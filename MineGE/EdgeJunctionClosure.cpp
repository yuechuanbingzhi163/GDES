#include "stdafx.h"

#include "LinkedGE.h"
#include <algorithm>
#include <vector>

// 查找连接点junctionPt关联的分支图元(包含隐形的图元)
static void FindLinesByPoint( const AcGePoint3d& junctionPt, AcDbObjectIdArray& objIds )
{
    AcDbBlockTable* pBlkTbl;
    acdbHostApplicationServices()->workingDatabase()->getSymbolTable( pBlkTbl, AcDb::kForRead );

    AcDbBlockTableRecord* pBlkTblRcd;
    pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();

    AcDbBlockTableRecordIterator* pBlkTblRcdItr;
    pBlkTblRcd->newIterator( pBlkTblRcdItr );

    for ( pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step() )
    {
        // 不采用transaction的方法查找LinkedGE，
        // 等价于排除当前正在以write状态编辑的LinkedGE
        // 重要(***)
        AcDbEntity* pEnt = 0;
        if( Acad::eOk != pBlkTblRcdItr->getEntity( pEnt, AcDb::kForRead ) ) continue;

        LinkedGE* pEdge = LinkedGE::cast( pEnt );
        if( pEdge != 0 )
        {
            AcGePoint3d startPt, endPt;
            pEdge->getSEPoint( startPt, endPt );
            if( startPt == junctionPt || endPt == junctionPt )
            {
                objIds.append( pEdge->objectId() );
            }
        }
        pEnt->close();
    }
    delete pBlkTblRcdItr;
    pBlkTblRcd->close();
}

struct JunctionEdgeInfo
{
    AcDbObjectId objId;             // 图元的id
    bool startOrEnd;                // 连接点是始点还是末点
    AcGeVector3d angle;             // 向内延伸方向向量

    bool operator==( const JunctionEdgeInfo& info )
    {
        return ( objId == info.objId ) &&
               ( startOrEnd == info.startOrEnd ) &&
               ( angle == info.angle );
    }
};

typedef std::vector<JunctionEdgeInfo> EdgeInfo;
typedef EdgeInfo::iterator EdgeInfoIter;

static LinkedGE* OpenEdge2( AcTransaction* pTrans, const AcDbObjectId& objId, AcDb::OpenMode openMode )
{
    AcDbObject* pObj = 0;
    if( Acad::eOk != pTrans->getObject( pObj, objId, openMode ) ) return 0;

    LinkedGE* pEdge = LinkedGE::cast( pObj );
    return pEdge;
}

static void BuildJunctionEdgeInfo( const AcDbObjectIdArray& objIds,
                                   const AcGePoint3d& junctionPt,
                                   EdgeInfo& ges )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int n = objIds.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbObjectId objId = objIds.at( i );

        JunctionEdgeInfo info = {objId, true, AcGeVector3d::kIdentity};

        LinkedGE* pEdge = OpenEdge2( pTrans, objId, AcDb::kForRead );
        if( pEdge == 0 ) continue;

        AcGePoint3d startPt, endPt;
        pEdge->getSEPoint( startPt, endPt );
        if( startPt == junctionPt )
        {
            info.startOrEnd = true;
            info.angle = pEdge->getStartPointInExtendAngle();
        }
        else if( endPt == junctionPt )
        {
            info.startOrEnd = false;
            info.angle = pEdge->getEndPointInExtendAngle();
        }

        if( info.angle.isZeroLength() ) continue; // 零向量表示不处理闭合
        //acutPrintf(_T("\nid:%d\tangle:%.3f\tdraw:%s"),
        //			info.objId,
        //			info.angle.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis),
        //			pEdge->getCurrentDraw());

        ges.push_back( info );
    }
    actrTransactionManager->endTransaction();
}

struct SortEdgeByAngle
{
    bool operator()( const JunctionEdgeInfo& info1, const JunctionEdgeInfo& info2 )
    {
        double angle1 = info1.angle.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
        double angle2 = info2.angle.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
        return angle1 < angle2;
    }
};

static void SortJunctionEdge( EdgeInfo& ges )
{
    // 按照角度大小排序（逆时针）
    std::sort( ges.begin(), ges.end(), SortEdgeByAngle() ); // 不能使用list，只能使用随机容器
}

// 新的计算方法
static AcGeVector3d CaclAverageVector2( const AcGeVector3d& v1, double w1, const AcGeVector3d& v2, double w2 )
{
    // 从v1->v2的转角
    double angle = v2.angleTo( v1, -AcGeVector3d::kZAxis );

    // 计算向量v1的长度
    double L1 = 0.5 * w1 / sin( angle );

    // 计算向量L2的长度
    double L2 = 0.5 * w2 / sin( angle );

    // 计算中心角向量
    AcGeVector3d v3 = v1 * L2 + v2 * L1;

    // 向量的"夹角"大于PI时，向量是反向的
    //if(angle >= PI)
    //{
    //	//acutPrintf(_T("\n夹角:%.3f   向量反向..."), angle);
    //	v3.negate();
    //}

    return v3;
}

// 原来的计算方法
static AcGeVector3d CaclAverageVector3( const AcGeVector3d& v1, const AcGeVector3d& v2 )
{
    double angle = v2.angleTo( v1, -AcGeVector3d::kZAxis );

    // 计算中心角向量
    AcGeVector3d v3 = v1 + v2;

    // 向量的"夹角"大于PI时，向量是反向的
    if( angle > 3.1415926535897932384626433832795 )
    {
        //acutPrintf(_T("\n夹角:%.3f   向量反向..."), angle);
        v3.negate();
    }

    return v3;
}

static void DealWithBoundary2( AcTransaction* pTrans,
                               const JunctionEdgeInfo& info,
                               const AcGePoint3d& junctionPt,
                               const AcGeVector3d& v )
{
    LinkedGE* pEdge = OpenEdge2( pTrans, info.objId, AcDb::kForWrite ); // 以write模式打开

    if( pEdge == 0 ) return;

    //acutPrintf(_T("\n射线角度:%.3f"), v.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis));
    AcGeRay3d boundaryLine( junctionPt, v );
    if( info.startOrEnd )
    {
        //acutPrintf(_T("\n处理id:%d的始端边界..."), pEdge->objectId());
        pEdge->dealWithStartPointBoundary( boundaryLine );
    }
    else
    {
        //acutPrintf(_T("\n处理id:%d的末端边界..."), pEdge->objectId());
        pEdge->dealWithEndPointBoundary( boundaryLine );
    }
}

// 至少需要2个元素才能正确的闭合
static void EdgeJunctionClosureImpl( const AcGePoint3d& junctionPt, EdgeInfo& ges )
{
    //acutPrintf(_T("\n队列中的元素个数:%d"), ges.size());
    if( ges.size() == 1 )
    {
        ges.push_back( ges.front() );
    }

    // 把第1个添加到末尾，构成循环
    ges.push_back( ges.front() );

    // 记录每次处理闭合的当前向量
    AcGeVector3d v3 = ges.front().angle;
    v3.rotateBy( PI / 2, AcGeVector3d::kZAxis );

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    for( EdgeInfoIter itr = ges.begin(); itr != ges.end(); itr++ )
    {
        EdgeInfoIter itr2 = itr + 1;
        if( itr2 == ges.end() ) break;

        //acutPrintf(_T("\n巷道1角度:%.3f, 巷道2角度:%.3f"),
        //	itr->angle.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis),
        //	itr2->angle.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis));

        AcGeVector3d cv = itr->angle.crossProduct( itr2->angle ); // 叉乘(如果夹角等=0或PI，则向量=0)
        //if(cv.length() > 0.001)
        if( !cv.isZeroLength() )
        {
            //acutPrintf(_T("\n叉乘长度=%.3f"), cv.length());
            //v3 = CaclAverageVector3(itr->angle, itr2->angle);
            v3 = CaclAverageVector2( itr->angle, 1, itr2->angle, 1 );
        }
        else
        {
            // 平行(夹角=0或PI)
            //acutPrintf(_T("\n叉乘=0"));
            v3.negate();
        }

        DealWithBoundary2( pTrans, *itr, junctionPt, v3 );
        DealWithBoundary2( pTrans, *itr2, junctionPt, v3 );
    }
    actrTransactionManager->endTransaction();
}

static void UpdateEdge( const AcDbObjectIdArray& objIds )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int n = objIds.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbObjectId objId = objIds.at( i );

        LinkedGE* pEdge = OpenEdge2( pTrans, objId, AcDb::kForWrite );
        pEdge->recordGraphicsModified( true ); // 标签图形已经修改，需要更新图形
    }
    actrTransactionManager->endTransaction();
}

void LinkedGEJunctionClosure_Helper( const AcGePoint3d& junctionPt )
{
    //acutPrintf(_T("\n闭合点:(%.3f, %.3f)"), junctionPt.x, junctionPt.y);
    AcDbObjectIdArray objIds;
    FindLinesByPoint( junctionPt, objIds );

    int len = objIds.length();
    //acutPrintf(_T("\n找到要处理闭合的分支个数:%d"), len);
    if( len < 1 ) return;

    EdgeInfo ges;
    BuildJunctionEdgeInfo( objIds, junctionPt, ges );          // 查找junctionPt坐标处的关联分支类图元
    if( ges.size() > 0 )
    {
        SortJunctionEdge( ges );                          // 调整顺序，按角度大小逆时针排列
        EdgeJunctionClosureImpl( junctionPt, ges );            // 处理并修改相邻巷道的参数
    }
    UpdateEdge( objIds );                                          // 更新实体
}