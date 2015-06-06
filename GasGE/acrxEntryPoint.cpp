#include "StdAfx.h"
#include "resource.h"

// 定义DefGE ARX模块对应的服务名称
// 用于注册服务和外部加载arx模块的时候
#ifndef GASGE_SERVICE_NAME
#define GASGE_SERVICE_NAME _T("GASGE_SERVICE_NAME")
#endif

class CGasGEApp : public AcRxArxApp
{

public:
    CGasGEApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // 注册服务
        acrxRegisterService( GASGE_SERVICE_NAME );

        acutPrintf( _T( "\nGasGE::On_kInitAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // 删除服务
        delete acrxServiceDictionary->remove( GASGE_SERVICE_NAME );

        acutPrintf( _T( "\nGasGE::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt );

        acutPrintf( _T( "\nGasGE::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg( pkt ) ;

        acutPrintf( _T( "\nGasGE::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CGasGEApp )

