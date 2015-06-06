#include "stdafx.h"
#include "DrawCmd.h"

#include "../ArxHelper/HelperClass.h"

#include "../GasGE/GOP.h"
#include "../GasGE/GCP.h"
#include "../GasGE/GPP.h"
#include "../GasGE/PPP.h"
#include "../GasGE/DCI.h"
#include "../GasGE/GFP.h"
#include "../GasGE/CDH.h"

void DrawCmd::DrawGOP()
{
    AcGePoint3d pt;
    if( !ArxUtilHelper::PromptPt( _T( "\n请选择插入点坐标：" ), pt ) )
    {
        acutPrintf( _T( "\n选择点坐标失败" ) );
        return;
    }

    int flag;
    if( RTNORM != acedGetInt( _T( "\n请选择突出点类型:[1)小型 2)中型 3)大型 4) 特大型]: " ), &flag ) )
    {
        return;
    }

    GOP* pGOP = 0;
    switch( flag )
    {
    case 1:
        pGOP = new SmallGOP( pt );
        break;

    case 2:
        pGOP = new MidGOP( pt );
        break;

    case 3:
        pGOP = new LargeGOP( pt );
        break;

    case 4:
        pGOP = new LargerGOP( pt );
        break;
    }
    if( pGOP == 0 ) return;

    if( !ArxUtilHelper::PostToModelSpace( pGOP ) ) delete pGOP;
}

void DrawCmd::DrawGCP()
{
    ads_point _pt;
    if( acedGetPoint( NULL, _T( "\n请选择插入点坐标：" ), _pt ) != RTNORM )
    {
        acutPrintf( _T( "\n选择点坐标失败" ) );
        return;
    }
    AcGePoint3d pt( _pt[X], _pt[Y], _pt[Z] );

    GCP* pGCP = new GCP( pt );
    if( !ArxUtilHelper::PostToModelSpace( pGCP ) ) delete pGCP;
}

void DrawCmd::DrawGPP()
{
    ads_point _pt;
    if( acedGetPoint( NULL, _T( "\n请选择插入点坐标：" ), _pt ) != RTNORM )
    {
        acutPrintf( _T( "\n选择点坐标失败" ) );
        return;
    }
    AcGePoint3d pt( _pt[X], _pt[Y], _pt[Z] );

    GPP* pGPP = new GPP( pt );
    if( !ArxUtilHelper::PostToModelSpace( pGPP ) ) delete pGPP;
}

void DrawCmd::DrawPPP()
{
    ads_point _pt;
    if( acedGetPoint( NULL, _T( "\n请选择插入点坐标：" ), _pt ) != RTNORM )
    {
        acutPrintf( _T( "\n选择点坐标失败" ) );
        return;
    }
    AcGePoint3d pt( _pt[X], _pt[Y], _pt[Z] );

    PPP* pPPP = new PPP( pt );
    if( !ArxUtilHelper::PostToModelSpace( pPPP ) ) delete pPPP;
}

// flag = 1 --> 煤层预测指标
// flag = 2 --> 工作面预测指标1
// flag = 3 --> 工作面预测指标2
void DrawCmd::DrawDCI( int flag )
{
    ads_point _pt;
    if( acedGetPoint( NULL, _T( "\n请选择插入点坐标：" ), _pt ) != RTNORM )
    {
        acutPrintf( _T( "\n选择点坐标失败" ) );
        return;
    }
    AcGePoint3d pt( _pt[X], _pt[Y], _pt[Z] );

    DCI* pDCI = 0;
    switch( flag )
    {
    case 1:
        pDCI = new CSDCI( pt );
        break;

    case 2:
        pDCI = new WDCI1( pt );
        break;

    case 3:
        pDCI = new WDCI2( pt );
        break;

    }
    if( pDCI == 0 ) return;

    if( !ArxUtilHelper::PostToModelSpace( pDCI ) ) delete pDCI;
}

// flag = 1 --> 工作面瓦斯涌出
// flag = 2 --> 掘进工作面瓦斯涌出
void DrawCmd::DrawGFP( int flag )
{
    ads_point _pt;
    if( acedGetPoint( NULL, _T( "\n请选择插入点坐标：" ), _pt ) != RTNORM )
    {
        acutPrintf( _T( "\n选择点坐标失败" ) );
        return;
    }
    AcGePoint3d pt( _pt[X], _pt[Y], _pt[Z] );

    GFP* pGFP = 0;
    switch( flag )
    {
    case 1:
        pGFP = new WGFP( pt );
        break;

    case 2:
        pGFP = new TGFP( pt );
        break;
    }
    if( pGFP == 0 ) return;

    if( !ArxUtilHelper::PostToModelSpace( pGFP ) ) delete pGFP;
}

void DrawCmd::DrawCDH()
{
    ads_point _pt;
    if( acedGetPoint( NULL, _T( "\n请选择插入点坐标：" ), _pt ) != RTNORM )
    {
        acutPrintf( _T( "\n选择点坐标失败" ) );
        return;
    }
    AcGePoint3d pt( _pt[X], _pt[Y], _pt[Z] );

    CDH* pCDH = new CDH( pt );
    if( !ArxUtilHelper::PostToModelSpace( pCDH ) ) delete pCDH;
}

