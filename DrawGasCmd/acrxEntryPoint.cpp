#include "StdAfx.h"
#include "resource.h"

#include "../MineGE/HelperClass.h"

#include "DrawCmd.h"

// 定义注册服务名称
#ifndef DRAW_GAS_CMD_SERVICE_NAME
#define DRAW_GAS_CMD_SERVICE_NAME _T("DRAWGASCMD_SERVICE_NAME")
#endif

/* 全局函数(实现在ImportBlock.cpp) */
extern void UpdateDwgBlock();

class CDrawGasCmdApp : public AcRxArxApp
{
public:
    CDrawGasCmdApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // 注册服务
        acrxRegisterService( DRAW_GAS_CMD_SERVICE_NAME );

        acutPrintf( _T( "\nDrawGasCmd::On_kLoadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // 删除服务
        delete acrxServiceDictionary->remove( DRAW_GAS_CMD_SERVICE_NAME );

        acutPrintf( _T( "\nDrawGasCmd::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt ) ;

        // 从当前arx模块所在路径下读取最新的瓦斯地质图块定义
        UpdateDwgBlock();

        acutPrintf( _T( "\nDrawGasCmd::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg ( pkt ) ;

        acutPrintf( _T( "\nDrawGasCmd::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    // 绘制小型突出点
    static void JL_DrawGOP()
    {
        //DrawCmd::DrawGOP();
        DrawCmd::DrawGOP2();
    }

    // 绘制瓦斯含量点
    static void JL_DrawGCP()
    {
        //DrawCmd::DrawGCP();
        DrawCmd::DrawGCP2();
    }

    // 绘制瓦斯压力点
    static void JL_DrawGPP()
    {
        //DrawCmd::DrawGPP();
        DrawCmd::DrawGPP2();
    }

    // 绘制动力现象点
    static void JL_DrawPPP()
    {
        //DrawCmd::DrawPPP();
        DrawCmd::DrawPPP2();
    }

    // 煤层突出危险性预测指标
    static void JL_DrawCSDCI()
    {
        //DrawCmd::DrawDCI(1);
        DrawCmd::DrawDCI2( 1 );
    }

    // 工作面突出危险性预测指标1
    static void JL_DrawWDCI1()
    {
        //DrawCmd::DrawDCI(2);
        DrawCmd::DrawDCI2( 2 );
    }

    // 工作面突出危险性预测指标2
    static void JL_DrawWDCI2()
    {
        //DrawCmd::DrawDCI(3);
        DrawCmd::DrawDCI2( 3 );
    }

    // 工作面瓦斯涌出
    static void JL_DrawWGFP()
    {
        //DrawCmd::DrawGFP(1);
        DrawCmd::DrawGFP2( 1 );
    }

    // 掘进工作面瓦斯涌出
    static void JL_DrawTGFP()
    {
        //DrawCmd::DrawGFP(2);
        DrawCmd::DrawGFP2( 2 );
    }

    // 见煤钻孔
    static void JL_DrawCDH()
    {
        //DrawCmd::DrawCDH();
        DrawCmd::DrawCDH2();
    }

    static void GAS_DisplayDataByDoubleClick()
    {
        DrawCmd::DisplayDataByDoubleClick();
    }

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CDrawGasCmdApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawGOP, DrawGOP, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawGCP, DrawGCP, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawGPP, DrawGPP, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawPPP, DrawPPP, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawCSDCI, DrawCSDCI, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawWDCI1, DrawWDCI1, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawWDCI2, DrawWDCI2, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawWGFP, DrawWGFP, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawTGFP, DrawTGFP, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, JL, _DrawCDH, DrawCDH, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CDrawGasCmdApp, GAS, _DisplayDataByDoubleClick, DisplayDataByDoubleClick, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
