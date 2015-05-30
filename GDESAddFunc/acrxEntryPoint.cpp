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

	static void JL_TestRCU()
	{
		extern void vector_to_angle(const AcGeVector3d& v, double& ang1, double& ang2);

		AcGeVector3d v(1,1,1);
		double ang1, ang2;
		vector_to_angle(v, ang1, ang2);

		acutPrintf(_T("\n弧度-->方向角1:%f  仰角：%f"), ang1, ang2);

		acutPrintf(_T("\n角度-->方向角1:%f  仰角：%f"), ang1*57.295, ang2*57.295);
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CGDESAddFuncApp)
ACED_ARXCOMMAND_ENTRY_AUTO( CGDESAddFuncApp, JL, _TestRCU, TestRCU, ACRX_CMD_TRANSPARENT, NULL )
