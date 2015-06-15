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
		//Ïú»ÙÍ£¿¿¶Ô»°¿ò
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

	static void JL_RCUDlg()
	{
		UIHelper::ShowRcuDesignDockBar();
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CARX_RCUApp)

ACED_ARXCOMMAND_ENTRY_AUTO( CARX_RCUApp, JL, _RCUDlg, RCUDlg, ACRX_CMD_TRANSPARENT, NULL )
