#include "StdAfx.h"
#include "resource.h"

#include "DataCmd.h"
#include "UIHelper.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

#include "MineDescripDlg.h"

// 定义注册服务名称
#ifndef MINEGE_CMDS_SERVICE_NAME
#define MINEGE_CMDS_SERVICE_NAME _T("MINEGECMDS_SERVICE_NAME")
#endif

// 基本命令模块
class CMineGECmdsApp : public AcRxArxApp
{

public:
    CMineGECmdsApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // 注册服务
        acrxRegisterService( MINEGE_CMDS_SERVICE_NAME );

        acutPrintf( _T( "\nMineGECmds::On_kLoadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // 删除服务
        delete acrxServiceDictionary->remove( MINEGE_CMDS_SERVICE_NAME );

        acutPrintf( _T( "\nMineGECmds::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt );

        acutPrintf( _T( "\nMineGECmds::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg( pkt ) ;

        acutPrintf( _T( "\nMineGECmds::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    static void JL_CopyPropertyData( void )
    {
        DataCmd::CopyData();;
    }

    static void JL_CopyPropertyData2( void )
    {
        DataCmd::CopyData2();
    }

    static void JL_ConfigVisualDraw()
    {
        UIHelper::ShowDrawManagerDlg();
    }

    static void JL_ConfigDataField()
    {
        UIHelper::ShowFieldManagerDlg();
    }

    static void JL_DisplayData()
    {
        UIHelper::DisplayData();
    }

    static void JL_DisplayDataByDoubleClick()
    {
        UIHelper::DisplayDataByDoubleClick();
    }

	static void JL_MineBaseParam()
	{
		CString funcName = _T("基本信息");
		UIHelper::DisplayDataByFuncName(funcName);
	}

	static void JL_CoalResrevesCacul()
	{
		CString funcName = _T("参与评价的煤量");
		UIHelper::DisplayDataByFuncName(funcName);
	}

	static void JL_RemnantGas()
	{
		CString funcName = _T("通过瓦斯抽采率计算");
		UIHelper::DisplayDataByFuncName(funcName);
	}

	static void JL_RemnantGasByPress()
	{
		CString funcName = _T("通过残余瓦斯压力反算");
		UIHelper::DisplayDataByFuncName(funcName);
	}

	static void JL_AbsoGasPress()
	{
		CString funcName = _T("残余瓦斯压力");
		UIHelper::DisplayDataByFuncName(funcName);
	}

	static void JL_RemnantGasStand()
	{
		CString funcName = _T("残存瓦斯含量");
		UIHelper::DisplayDataByFuncName(funcName);
	}

	static void JL_AnalysisGas()
	{
		CString funcName = _T("可解吸瓦斯含量");
		UIHelper::DisplayDataByFuncName(funcName);
	}

	static void JL_FaceDrainGasRate()
	{
		CString funcName = _T("工作面瓦斯抽采率");
		UIHelper::DisplayDataByFuncName(funcName);
	}

	static void JL_MineDrainGasRate()
	{
		CString funcName = _T("矿井瓦斯抽采率");
		UIHelper::DisplayDataByFuncName(funcName);
	}

	static void JL_MiningFaceGas()
	{
		CString funcName = _T("本煤层瓦斯涌出量");
		UIHelper::DisplayDataByFuncName(funcName);
	}

	static void JL_NearbyFaceGas()
	{
		CString funcName = _T("邻近层瓦斯涌出量");
		UIHelper::DisplayDataByFuncName(funcName);
	}

	static void JL_GasTubeDiameter()
	{
		CString funcName = _T("管径及流速");
		//UIHelper::DisplayDataByFuncName(funcName);
		UIHelper::DisplayGasTubeDataByFunc(funcName);
	}

	static void JL_Tuberesistance()
	{
		CString funcName = _T("管路阻力");
		//UIHelper::DisplayDataByFuncName(funcName);
		UIHelper::DisplayGasTubeDataByFunc(funcName);
	}

	static void JL_TubeCacul()
	{
		CString funcName = _T("管路计算");
		UIHelper::DisplayGasTubeDataByFunc(funcName);
	}

	static void JL_PumpFlow()
	{
		CString funcName = _T("瓦斯泵的流量计算");
		UIHelper::DisplayGasPumpDataByFunc(funcName);
		//UIHelper::DisplayDataByFuncName(funcName);
	}

	static void JL_PumpPress()
	{
		CString funcName = _T("瓦斯泵的压力计算");
		//UIHelper::DisplayDataByFuncName(funcName);
		UIHelper::DisplayGasPumpDataByFunc(funcName);
	}

	static void JL_PumpCacul()
	{
		CString funcName = _T("瓦斯泵计算");
		//UIHelper::DisplayDataByFuncName(funcName);
		UIHelper::DisplayGasPumpDataByFunc(funcName);
	}

	static void JL_DrainDrillFy()
	{
		CString funcName = _T("孔口负压参数");
		UIHelper::DisplayDrillDataByFunc(funcName);
	}

	static void JL_ZkDbHeight()
	{
		CString funcName = _T("终孔设计高度参考值");
		//UIHelper::DisplayDataByFuncName(funcName);
		UIHelper::DisplayDrillDataByFunc(funcName);
	}
	
	static void JL_Drilldeflection()
	{
		CString funcName = _T("钻孔偏角参数");
		//UIHelper::DisplayDataByFuncName(funcName);
		UIHelper::DisplayDrillDataByFunc(funcName);
	}

	static void JL_DrillElevation()
	{
		CString funcName = _T("钻孔仰角参数");
		//UIHelper::DisplayDataByFuncName(funcName);
		UIHelper::DisplayDrillDataByFunc(funcName);
	}

	static void JL_DrillLength()
	{
		CString funcName = _T("钻孔长度参数");
		//UIHelper::DisplayDataByFuncName(funcName);
		UIHelper::DisplayDrillDataByFunc(funcName);
	}

	static void JL_DrillEffectiveLength()
	{
		CString funcName = _T("钻孔有效长度");
		//UIHelper::DisplayDataByFuncName(funcName);
		UIHelper::DisplayDrillDataByFunc(funcName);
	}

	static void JL_FaceEffectiveLength()
	{
		CString funcName = _T("工作面推进有效长度");
		//UIHelper::DisplayDataByFuncName(funcName);
		UIHelper::DisplayDrillDataByFunc(funcName);
	}

	static void JL_HighDrillDis()
	{
		CString funcName = _T("高位钻孔钻场间距");
		//UIHelper::DisplayDataByFuncName(funcName);
		UIHelper::DisplayDrillDataByFunc(funcName);
	}

	static void JL_AllDrillCacul()
	{
		CString funcName = _T("高位钻孔参数计算");
		//UIHelper::DisplayDataByFuncName(funcName);
		UIHelper::DisplayDrillDataByFunc(funcName);
	}


    static void JL_ConfigVarList()
    {
        UIHelper::ShowVarListDlg();
    }

    static void JL_DisplayDataObject()
    {
        UIHelper::ShowDataObjectStateDlg();
    }

    static void JL_ShowAllTagGE()
    {
        AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个图元:" ) );
        if( objId.isNull() ) return;
        if( !ArxUtilHelper::IsEqualType( _T( "MineGE" ), objId ) ) return;

        DrawHelper::ShowAllTagGE( objId, 2 ); // 用黄颜色高亮显示
    }

    static void JL_ShowHostGE()
    {
        AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个标签图元:" ) );
        if( objId.isNull() ) return;
        if( !ArxUtilHelper::IsEqualType( _T( "TagGE" ), objId ) ) return;

        DrawHelper::ShowHostGE( objId, 2 ); // 用黄颜色高亮显示
    }

	static void JL_MineDesc()
	{
		CAcModuleResourceOverride myResources;
		//acutPrintf(_T("\n显示对话框"));
		MineMainDescipDlg dlg;
		//一定要初始化多行编辑框RichEdit2
		AfxInitRichEdit2();
		dlg.DoModal();
		//acutPrintf(_T("\n显示完毕"));
	}

} ;

IMPLEMENT_ARX_ENTRYPOINT( CMineGECmdsApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _CopyPropertyData, CopyPropertyData, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _CopyPropertyData2, CopyPropertyData2, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _ConfigVisualDraw, ConfigVisualDraw, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _ConfigDataField, ConfigDataField, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _DisplayData, DisplayData, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _DisplayDataByDoubleClick, DisplayDataByDoubleClick, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _ConfigVarList, ConfigVarList, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _DisplayDataObject, DisplayDataObject, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _ShowAllTagGE, ShowAllTagGE, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _ShowHostGE, ShowHostGE, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _MineBaseParam, MineBaseParam, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _CoalResrevesCacul, CoalResrevesCacul, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _RemnantGas, RemnantGas, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _RemnantGasByPress, RemnantGasByPress, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _AbsoGasPress, AbsoGasPress, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _RemnantGasStand, RemnantGasStand, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _AnalysisGas, AnalysisGas, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _FaceDrainGasRate, FaceDrainGasRate, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _MineDrainGasRate, MineDrainGasRate, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _MiningFaceGas, MiningFaceGas, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _NearbyFaceGas, NearbyFaceGas, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _GasTubeDiameter, GasTubeDiameter, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _Tuberesistance, Tuberesistance, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _TubeCacul, TubeCacul, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _PumpFlow, PumpFlow, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _PumpPress, PumpPress, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _PumpCacul, PumpCacul, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _DrainDrillFy, DrainDrillFy, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _ZkDbHeight, ZkDbHeight, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _Drilldeflection, Drilldeflection, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _DrillElevation, DrillElevation, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _DrillLength, DrillLength, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _DrillEffectiveLength, DrillEffectiveLength, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _FaceEffectiveLength, FaceEffectiveLength, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _HighDrillDis, HighDrillDis, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _MineDesc, MineDesc, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CMineGECmdsApp, JL, _AllDrillCacul, AllDrillCacul, ACRX_CMD_TRANSPARENT | ACRX_CMD_USEPICKSET | ACRX_CMD_REDRAW, NULL )
