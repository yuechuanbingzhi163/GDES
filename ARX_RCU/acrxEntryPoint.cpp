#include "StdAfx.h"
#include "resource.h"
//#include "ReactorHelper.h"
#include "UIHelper.h"

#ifndef ARX_RCU_SERVICE_NAME
#define ARX_RCU_SERVICE_NAME _T("ARX_RCU_SERVICE_NAME")
#endif

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CARX_RCUApp : public AcRxArxApp {

public:
	CARX_RCUApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {

		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;

		acrxRegisterService( ARX_RCU_SERVICE_NAME );
		//ReactorHelper::CreateDocumentReactorMap();
		acutPrintf( _T( "\nARX_RCU::On_kLoadAppMsg\n" ) );

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {

		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		//ReactorHelper::RemoveDocumentReactorMap();
		delete acrxServiceDictionary->remove( ARX_RCU_SERVICE_NAME );
		acutPrintf( _T( "\nARX_RCU::On_kUnloadAppMsg\n" ) );
		//销毁停靠对话框
		UIHelper::DestroyRcuDesignDockBar();

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
	{
		AcRx::AppRetCode retCode = AcRxArxApp::On_kLoadDwgMsg ( pkt ) ;

		//ReactorHelper::AddDocumentReactor( curDoc() );
		acutPrintf( _T( "\nARX_RCU::On_kLoadDwgMsg\n" ) );

		return retCode;
	}

	virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
	{
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadDwgMsg ( pkt ) ;

		//ReactorHelper::RemoveDocumentReactor( curDoc() );
		acutPrintf( _T( "\nARX_RCU::On_kUnloadDwgMsg\n" ) );

		return retCode;
	}

	virtual void RegisterServerComponents () {
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

	static void JL_TestRCUDlg()
	{
		UIHelper::ShowRcuDesignDockBar();
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CARX_RCUApp)

ACED_ARXCOMMAND_ENTRY_AUTO( CARX_RCUApp, JL, _TestRCU, TestRCU, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CARX_RCUApp, JL, _TestRCUDlg, TestRCUDlg, ACRX_CMD_TRANSPARENT, NULL )
