#include "stdafx.h"

#include "../MineGE/LinkedGE.h"

#include "../ArxHelper/HelperClass.h"
#include "../DefGE/ArcTunnel.h"

void GetEdgeSEPts( const AcDbObjectIdArray& objIds, AcGePoint3dArray& spts, AcGePoint3dArray& epts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        LinkedGE* pEdge = LinkedGE::cast( pObj );
        if( pEdge == 0 ) continue;

        AcGePoint3d spt, ept;
        pEdge->getSEPoint( spt, ept );

        spts.append( spt );
        epts.append( ept );
    }
    actrTransactionManager->endTransaction();
}

bool GetSEPt( const AcDbObjectId& objId, AcGePoint3d& spt, AcGePoint3d& ept )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    LinkedGE* pEdge = LinkedGE::cast( pObj );
    if( pEdge == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    pEdge->getSEPoint( spt, ept );
    actrTransactionManager->endTransaction();

    return true;
}

static void CacLineClosePt( const AcGePoint3d& spt, const AcGePoint3d& ept,
                            AcGePoint3d& pt, double& angle )
{
    // 构造一条几何线段
    AcGeLineSeg3d line( spt, ept );
    pt = line.closestPointTo( pt ); // 计算距离中线最近的点

    AcGeVector3d v = ept - spt;
    angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
}

static void CacArcClosePt( const AcGePoint3d& spt, const AcGePoint3d& ept, const AcGePoint3d& arcCentPt,
                           AcGePoint3d& pt)
{
    AcGeCircArc3d arc( spt, arcCentPt, ept );
    AcGePoint3d cnt = arc.center();
    double radius = arc.radius();

    AcGeVector3d v = ept - cnt;
    AcGeVector3d v2 = ept - spt;
    AcGeVector3d v3 = v.crossProduct( v2 );
    int c = ( v3.z > 0 ? 1 : -1 );

    // 距离圆弧最近点
    pt = arc.closestPointTo( pt );

    //// 计算角度
    //v = pt - cnt;
    //v.rotateBy( c * PI / 2, AcGeVector3d::kZAxis );

    //angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
	//angle = getArcAngle(spt,ept,arcCentPt,pt);
}

bool GetClosePtAndAngle( const AcDbObjectId& objId, AcGePoint3d& pt, double& angle )
{
    // 获取LinkedGE图元参数
    AcDbObject* pObj;
    if( Acad::eOk != acdbOpenObject( pObj, objId, AcDb::kForRead ) ) return false;

    LinkedGE* pEdge = LinkedGE::cast( pObj );
    if( pEdge == 0 )
    {
        pObj->close();
        return false;
    }

    AcGePoint3d startPt, endPt;
    pEdge->getSEPoint( startPt, endPt );
    angle = pEdge->getAngle();

    // 是否弧线巷道
    bool isArcTunnel = false;
    AcGePoint3d arcCentPt;
    ArcTunnel* pArcTunnel = ArcTunnel::cast( pEdge );
    if( pArcTunnel != 0 )
    {
        isArcTunnel = true;
    }

    pObj->close();

    if( isArcTunnel )
    {
		arcCentPt = pArcTunnel->getArcCntPt();
        CacArcClosePt( startPt, endPt, arcCentPt, pt );
		angle = pArcTunnel->getArcAngle(pt);
    }
    else
    {
        CacLineClosePt( startPt, endPt, pt, angle );
    }
    return true;
}

bool PromptSEPt( const CString& name, AcGePoint3d& startPt, AcGePoint3d& endPt, double& angle )
{
    CString msg;
    msg.Format( _T( "\n绘制%s..." ), name );

    acutPrintf( msg );

    ads_point start_pt, end_pt;
    msg.Format( _T( "\n请选择%s的始节点坐标：" ), name );
    if( acedGetPoint( NULL, msg, start_pt ) != RTNORM )
    {
        acutPrintf( _T( "\n选择点坐标失败" ) );
        return false;
    }

    msg.Format( _T( "\n请选择%s的末节点坐标：" ), name );
    if( acedGetPoint( start_pt, msg, end_pt ) != RTNORM )
    {
        acutPrintf( _T( "\n选择点坐标失败" ) );
        return false;
    }

    // 返回结果
    startPt = asPnt3d( start_pt );
    endPt = asPnt3d( end_pt );

    AcGeVector3d v = endPt - startPt;
    angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    //acutPrintf(_T("\n原始角度：%.3f"), angle);
    return true;
}

bool PromptArcPt( const CString& name, AcGePoint3d& startPt, AcGePoint3d& endPt, AcGePoint3d& thirdPt )
{
    CString msg;
    msg.Format( _T( "\n绘制%s..." ), name );

    acutPrintf( msg );

    ads_point start_pt, end_pt, third_pt;
    msg.Format( _T( "\n请指定%s的第一点：" ), name );
    if( acedGetPoint( NULL, msg, start_pt ) != RTNORM )
    {
        acutPrintf( _T( "\n选择点坐标失败" ) );
        return false;
    }

    msg.Format( _T( "\n请指定%s的第二点" ), name );
    if( acedGetPoint( start_pt, msg, third_pt ) != RTNORM )
    {
        acutPrintf( _T( "\n选择点坐标失败" ) );
        return false;
    }

    msg.Format( _T( "\n请指定%s的第三点：" ), name );
    if( acedGetPoint( third_pt, msg, end_pt ) != RTNORM )
    {
        acutPrintf( _T( "\n选择点坐标失败" ) );
        return false;
    }

    // 返回结果
    startPt = asPnt3d( start_pt );
    endPt = asPnt3d( end_pt );
    thirdPt = asPnt3d( third_pt );

    return true;
}

bool PromptInsertPt( const AcDbObjectId& objId, AcGePoint3d& pt )
{
    AcGePoint3d spt, ept;
    if( !GetSEPt( objId, spt, ept ) ) return false;

    AcGeVector3d v = ept - spt;

    return ArxUtilHelper::PromptPt2( _T( "\n请选择插入点坐标" ), spt + v * 0.5, pt );
}