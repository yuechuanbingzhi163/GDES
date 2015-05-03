#include "stdafx.h"
#include "DrawCmd.h"

#include "../DefGE/TTunnel.h"

#include "../MineGE/DrawHelper.h"
#include "../ArxHelper/ArxUtilHelper.h"

/* 全局函数(实现在PromptTool.cpp) */
extern bool PromptSEPt( const CString& name, AcGePoint3d& startPt, AcGePoint3d& endPt, double& angle );
extern bool GetClosePtAndAngle( const AcDbObjectId& objId, AcGePoint3d& pt, double& angle );

void DrawCmd::DrawTTunnel()
{
    //AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "\n请选择一条进风巷道:" ) );
    //if( objId.isNull() ) return;
    //if( !ArxUtilHelper::IsEqualType( _T( "Tunnel" ), objId ) ) return;

    AcGePoint3d startPt, endPt;
    double angle;
    if( !PromptSEPt( _T( "掘进工作面" ), startPt, endPt, angle ) ) return;

    TTunnel* pTT = new TTunnel( startPt, endPt );
    //pTT->setInTunnel( objId );

    // 初始化并提交到数据库
    if( !ArxUtilHelper::PostToModelSpace( pTT ) ) delete pTT;
}

static void SetInTunnel( const AcDbObjectId& objId, const AcDbObjectId& inTunnel )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    TTunnel* pTT = TTunnel::cast( pObj );
    //pTT->setInTunnel( inTunnel );

    actrTransactionManager->endTransaction();
}

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
    //inTunnel = pTT->getInTunnel();

    actrTransactionManager->endTransaction();

    return true;
}

void DrawCmd::ResetInTunnel()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "\n请选择一个掘进工作面:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId ) ) return;

    AcDbObjectId inTunnel = ArxUtilHelper::SelectObject( _T( "\n请选择掘进工作面的风筒所在的进风巷道:" ) );
    if( inTunnel.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "Tunnel" ), inTunnel ) ) return;

    SetInTunnel( objId, inTunnel );
}

void DrawCmd::ShowInTunnel()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "\n请选择一个掘进工作面:" ) );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId ) ) return;

    AcDbObjectId inTunnel;
    if( !GetInTunnel( objId, inTunnel ) ) return;

    ArxUtilHelper::ShowEntityWithColor( inTunnel, 2 ); // 使用黄颜色高亮显示
}
