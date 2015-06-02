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


} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CGDESAddFuncApp)
ACED_ARXCOMMAND_ENTRY_AUTO( CGDESAddFuncApp, JL, _PumpCapacityDlg, PumpCapacityDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CGDESAddFuncApp, JL, _RealGasDlg, RealGasDlg, ACRX_CMD_TRANSPARENT, NULL )
