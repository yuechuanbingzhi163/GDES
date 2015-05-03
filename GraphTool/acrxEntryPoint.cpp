#include "StdAfx.h"
#include "resource.h"
#include "GraphAlgo.h"

// 定义注册服务名称
#ifndef GESES_GRAPH_TOOL_SERVICE_NAME
#define GESES_GRAPH_TOOL_SERVICE_NAME _T("GRAPHTOOL_SERVICE_NAME")
#endif

class CGraphToolApp : public AcRxArxApp
{

public:
    CGraphToolApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        // TODO: Load dependencies here

        // You *must* call On_kInitAppMsg here
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;
		acrxRegisterService( GESES_GRAPH_TOOL_SERVICE_NAME );

        // TODO: Add your initialization code here

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        // TODO: Add your code here

        // You *must* call On_kUnloadAppMsg here
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;
		delete acrxServiceDictionary->remove( GESES_GRAPH_TOOL_SERVICE_NAME );

        // TODO: Unload dependencies here

        return ( retCode ) ;
    }

    virtual void RegisterServerComponents ()
    {
    }

	static void JL_DFSTest( void )
	{
		//GraphAlgo::FindGasTubePath();
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CGraphToolApp )
ACED_ARXCOMMAND_ENTRY_AUTO( CGraphToolApp, JL, _DFSTest, DFSTest, ACRX_CMD_TRANSPARENT, NULL )

