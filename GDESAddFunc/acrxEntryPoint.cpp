#include "StdAfx.h"
#include "resource.h"

#ifndef GDES_ADDFUNC_SERVICE_NAME
#define GDES_ADDFUNC_SERVICE_NAME _T("GDESADDFUNC_SERVICE_NAME")
#endif

class CGDESAddFuncApp : public AcRxArxApp {

public:
	CGDESAddFuncApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		acrxRegisterService( GDES_ADDFUNC_SERVICE_NAME );

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
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

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CGDESAddFuncApp)

