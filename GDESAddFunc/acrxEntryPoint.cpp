#include "StdAfx.h"
#include "resource.h"
#include "UIHelper.h"

#include "../MineGE/DataListHelper.h"
#include "../MineGE/DataObject.h"
#include "../MineGE/config.h"
#include "../ArxHelper/HelperClass.h"

#include "config.h"


#ifndef GDES_ADDFUNC_SERVICE_NAME
#define GDES_ADDFUNC_SERVICE_NAME _T("GDESADDFUNC_SERVICE_NAME")
#endif

static void AddReportObject()
{
	GDESDataObjectHelper::AddObject(MINE_GAS_CAPACITY);
	GDESDataObjectHelper::AddObject(PUMP_CAPACITY_LIST);
	GDESDataObjectHelper::AddObject(YEAR_REAL_GAS);
	GDESDataObjectHelper::AddObject(PRE_GAS);
	GDESDataObjectHelper::AddObject(PRE_GAS_LIST);
	GDESDataObjectHelper::AddObject(RATE_GAS);
	GDESDataObjectHelper::AddObject(RET_GAS);
}

class CGDESAddFuncApp : public AcRxArxApp {

public:
	CGDESAddFuncApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		acrxRegisterService( GDES_ADDFUNC_SERVICE_NAME );

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		ArxDataTool::RegDict( ADDFUN_INFO_DICT );
		AddReportObject();
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here
		delete acrxServiceDictionary->remove( GDES_ADDFUNC_SERVICE_NAME );
		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

	static void JL_PumpCapacityDlg( void )
	{
		UIHelper::ShowPumpCapacityDlg();
	}

	static void JL_RealGasDlg( void )
	{
		UIHelper::ShowRealGasDlg();
	}

	static void JL_PreGasDlg( void )
	{
		UIHelper::ShowPreGasDlg();
	}

	static void JL_RateGasDlg( void )
	{
		UIHelper::ShowRateGasDlg();
	}

	static void JL_RetDlg( void )
	{
		UIHelper::ShowRetDlg();
	}

	static void JL_TestRCU()
	{
		extern void vector_to_angle(const AcGeVector3d& v, double& ang1, double& ang2);

		AcGeVector3d v(1,1,1);
		double ang1, ang2;
		vector_to_angle(v, ang1, ang2);
		acutPrintf(_T("\n向量:(%.3f, %.3f, %.3f)"), v.x, v.y, v.z);
		acutPrintf(_T("\n弧度-->方向角1:%f  仰角：%f"), ang1, ang2);
		acutPrintf(_T("\n角度-->方向角1:%f  仰角：%f"), ang1*57.295, ang2*57.295);

		AcGeVector3d v1(1,0,0);
		vector_to_angle(v1, ang1, ang2);
		acutPrintf(_T("\n向量:(%.3f, %.3f, %.3f)"), v1.x, v1.y, v1.z);
		acutPrintf(_T("\n弧度-->方向角1:%f  仰角：%f"), ang1, ang2);
		acutPrintf(_T("\n角度-->方向角1:%f  仰角：%f"), ang1*57.295, ang2*57.295);

		AcGeVector3d v2(-1,-1,1);
		vector_to_angle(v2, ang1, ang2);
		acutPrintf(_T("\n向量:(%.3f, %.3f, %.3f)"), v2.x, v2.y, v2.z);
		acutPrintf(_T("\n弧度-->方向角1:%f  仰角：%f"), ang1, ang2);
		acutPrintf(_T("\n角度-->方向角1:%f  仰角：%f"), ang1*57.295, ang2*57.295);
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CGDESAddFuncApp)
ACED_ARXCOMMAND_ENTRY_AUTO( CGDESAddFuncApp, JL, _PumpCapacityDlg, PumpCapacityDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGDESAddFuncApp, JL, _RealGasDlg, RealGasDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGDESAddFuncApp, JL, _PreGasDlg, PreGasDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGDESAddFuncApp, JL, _RateGasDlg, RateGasDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGDESAddFuncApp, JL, _RetDlg, RetDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGDESAddFuncApp, JL, _TestRCU, TestRCU, ACRX_CMD_TRANSPARENT, NULL )
