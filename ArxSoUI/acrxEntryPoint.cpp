//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"

// 定义注册服务名称
#ifndef ARX_SOUI_SERVICE_NAME
#define ARX_SOUI_SERVICE_NAME _T("ARXSOUI_SERVICE_NAME")
#endif

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CArxSoUIApp : public AcRxArxApp {

public:
	CArxSoUIApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		acrxRegisterService( ARX_SOUI_SERVICE_NAME );

		acutPrintf( _T( "\nArxSoUI::On_kInitAppMsg\n" ) );

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		delete acrxServiceDictionary->remove( ARX_SOUI_SERVICE_NAME );

		acutPrintf( _T( "\nArxSoUI::On_kUnloadAppMsg\n" ) );

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
	{
		AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt );

		acutPrintf( _T( "\nArxSoUI::On_kLoadDwgMsg\n" ) );

		return retCode;
	}

	virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
	{
		AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg( pkt ) ;

		acutPrintf( _T( "\nArxSoUI::On_kUnloadDwgMsg\n" ) );

		return retCode;
	}

	virtual void RegisterServerComponents () {
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CArxSoUIApp)

