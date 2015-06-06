#include "StdAfx.h"
#include "resource.h"
#include "ReportHelper.h"

#include "../MineGE/DataListHelper.h"
#include "../MineGE/config.h"
#include "../ArxHelper/HelperClass.h"

#include "../MineGE/DataObject.h"
#include "config.h"

#ifndef ARX_REPORTHELPER_SERVICE_NAME
#define ARX_REPORTHELPER_SERVICE_NAME _T("ARXREPORTHELPER_SERVICE_NAME")
#endif

static void AddReportObject()
{
	GDESDataObjectHelper::AddObject(PERMENT_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(TEMPGAS_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(RUNNING_SYS_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(GAS_PLAN_BOOKMKS_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(INSPECT_BOOKMKS_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(SM_BOOKMKS_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(EPCMFD_BOOKMKS_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(GAS_PUMP_OBJCT_NAME);
	
	GDESDataObjectHelper::AddObject(RET_GASSYS_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(RET_EPCMFD_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(RET_GASPLAN_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(RET_INSPECT_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(RET_SM_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(RET_GASPUMP_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(RET_MERSURE_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(RET_REALATE_OBJCT_NAME);

	GDESDataObjectHelper::AddObject(DESC_EPCMFD_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(DESC_GASPLAN_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(DESC_INSPECT_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(DESC_SM_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(DESC_GASPUMP_OBJCT_NAME);
	GDESDataObjectHelper::AddObject(DESC_GASSYS_OBJCT_NAME);

}
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CARX_ReportHelperApp : public AcRxArxApp {

public:
	CARX_ReportHelperApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {

		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		acrxRegisterService( ARX_REPORTHELPER_SERVICE_NAME );

		ArxDataTool::RegDict( REPORTER_INFO_DICT );
		AddReportObject();
		//DataObject* pDO = new DataObject();
		//pDO->initData();
		//acutPrintf(_T("\n加载%s报告数据对象..."),PERMENT_OBJCT_NAME);

		acutPrintf( _T( "\nARX_ReportHelper::On_kLoadAppMsg\n" ) );

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {

		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		delete acrxServiceDictionary->remove( ARX_REPORTHELPER_SERVICE_NAME );
		acutPrintf( _T( "\nARX_ReportHelper::On_kUnloadAppMsg\n" ) );

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
