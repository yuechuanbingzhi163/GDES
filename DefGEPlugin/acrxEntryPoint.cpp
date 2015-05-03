#include "StdAfx.h"
#include "resource.h"

#include "DrawTool.h"

// 定义DefGEPlugin ARX模块对应的服务名称
// 用于注册服务和外部加载arx模块的时候
#ifndef DEFGE_PLUGIN_SERVICE_NAME
#define DEFGE_PLUGIN_SERVICE_NAME _T("DEFGEPLUGIN_SERVICE_NAME")
#endif

class CDefGEPluginApp : public AcRxArxApp
{

public:
    CDefGEPluginApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        acrxRegisterService( DEFGE_PLUGIN_SERVICE_NAME );

		CreateTextStyle(_T("GDESS"), _T("Times New Roman"), Adesk::kFalse, Adesk::kFalse, GB2312_CHARSET, 16);

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        delete acrxServiceDictionary->remove( DEFGE_PLUGIN_SERVICE_NAME );

        return ( retCode ) ;
    }

    virtual void RegisterServerComponents ()
    {
    }

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CDefGEPluginApp )

