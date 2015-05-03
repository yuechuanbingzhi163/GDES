#include "StdAfx.h"
#include "resource.h"
#include "ReportHelper.h"

#include "../MineGE/DataListHelper.h"
#include "../MineGE/config.h"
#include "../ArxHelper/HelperClass.h"

#include "../MineGE/DataObject.h"
#include "config.h"

#ifndef ARX_REPORTHELPER_SERVICE_NAME
#define ARX_REPORTHELPER_SERVICE_NAME _T("ARX_REPORTHELPER_SERVICE_NAME")
#endif

static void AddReportObject()
{
	ReporterDataObjectHelper::AddObject(PERMENT_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(TEMPGAS_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(RUNNING_SYS_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(GAS_PLAN_BOOKMKS_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(INSPECT_BOOKMKS_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(SM_BOOKMKS_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(EPCMFD_BOOKMKS_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(GAS_PUMP_OBJCT_NAME);
	
	ReporterDataObjectHelper::AddObject(RET_GASSYS_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(RET_EPCMFD_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(RET_GASPLAN_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(RET_INSPECT_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(RET_SM_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(RET_GASPUMP_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(RET_MERSURE_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(RET_REALATE_OBJCT_NAME);

	ReporterDataObjectHelper::AddObject(DESC_EPCMFD_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(DESC_GASPLAN_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(DESC_INSPECT_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(DESC_SM_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(DESC_GASPUMP_OBJCT_NAME);
	ReporterDataObjectHelper::AddObject(DESC_GASSYS_OBJCT_NAME);

}
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CARX_ReportHelperApp : public AcRxArxApp {

public:
	CARX_ReportHelperApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		acrxRegisterService( ARX_REPORTHELPER_SERVICE_NAME );
		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		ArxDataTool::RegDict( REPORTER_INFO_DICT );
		AddReportObject();
		//DataObject* pDO = new DataObject();
		//pDO->initData();
		//acutPrintf(_T("\n加载%s报告数据对象..."),PERMENT_OBJCT_NAME);
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		delete acrxServiceDictionary->remove( ARX_REPORTHELPER_SERVICE_NAME );
		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

	//static void WordTest_ARXWORD( void )
	//{
	//	ReportHelper::CreatReportHelper();
	//}

	//static void WordTest_HELP()
	//{
	//	ReportHelper::OpenCHMFile();
	//}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CARX_ReportHelperApp)
//ACED_ARXCOMMAND_ENTRY_AUTO( CARX_ReportHelperApp, WordTest, _ARXWORD, ARXWORD, ACRX_CMD_TRANSPARENT, NULL )
//ACED_ARXCOMMAND_ENTRY_AUTO( CARX_ReportHelperApp, WordTest, _HELP, HELP, ACRX_CMD_TRANSPARENT, NULL )
