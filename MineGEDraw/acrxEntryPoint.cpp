#include "StdAfx.h"
#include "resource.h"

#ifndef MINEGEDRAW_SERVICE_NAME
#define MINEGEDRAW_SERVICE_NAME _T("MINEGEDRAW_SERVICE_NAME")
#endif

class CMineGEDrawApp : public AcRxArxApp
{

public:
    CMineGEDrawApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        acrxRegisterService( MINEGEDRAW_SERVICE_NAME );

        acutPrintf( _T( "\nMineGEDraw::On_kInitAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {

        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        delete acrxServiceDictionary->remove( MINEGEDRAW_SERVICE_NAME );

        acutPrintf( _T( "\nMineGEDraw::On_kUnloadAppMsg\n" ) );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kLoadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kLoadDwgMsg ( pkt );

        acutPrintf( _T( "\nMineGEDraw::On_kLoadDwgMsg\n" ) );

        return retCode;
    }

    virtual AcRx::AppRetCode On_kUnloadDwgMsg( void* pkt )
    {
        AcRx::AppRetCode retCode = AcRxDbxApp::On_kUnloadDwgMsg( pkt ) ;

        acutPrintf( _T( "\nMineGEDraw::On_kUnloadDwgMsg\n" ) );

        return retCode;
    }

    virtual void RegisterServerComponents ()
    {
    }

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT( CMineGEDrawApp )

