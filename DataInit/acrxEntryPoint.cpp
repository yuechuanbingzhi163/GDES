#include "StdAfx.h"
#include "resource.h"

/* 单独的一个数据初始化模块，每次新建一个文档(load dwg)
 * 都会重新注册数据(字段数据对象等)
 */

// 定义注册服务名称
#ifndef DATA_INIT_SERVICE_NAME
#define DATA_INIT_SERVICE_NAME _T("DATAINIT_SERVICE_NAME")
#endif

#include "DataImportDlg.h"

/* 全局函数(实现在DataInit.cpp) */
extern CString GetAppPathDir();
extern CString BuildPath( const CString& dir, const CString& fileName );
extern void InitAllData( const CString& dirPath );

class CDataInitApp : public AcRxArxApp
{

public:
    CDataInitApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // 注册服务
        acrxRegisterService( DATA_INIT_SERVICE_NAME );

        acutPrintf( _T( "\nDataInit::On_kLoadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // 删除服务
        delete acrxServiceDictionary->remove( DATA_INIT_SERVICE_NAME );

        acutPrintf( _T( "\nDataInit::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt ) ;

        CString dataDirName = _T( "Datas\\" );
        InitAllData( BuildPath( GetAppPathDir(), dataDirName ) );

        acutPrintf( _T( "\nDataInit::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg ( pkt ) ;

        acutPrintf( _T( "\nDataInit::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

    static void JL_DataImport( void )
    {
        // 切换资源
        CAcModuleResourceOverride myResources;

        DataImportDlg dlg;
        dlg.DoModal();
    }
} ;

IMPLEMENT_ARX_ENTRYPOINT( CDataInitApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CDataInitApp, JL, _DataImport, DataImport, ACRX_CMD_TRANSPARENT, NULL )

