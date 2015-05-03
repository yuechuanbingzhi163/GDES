#include "StdAfx.h"
#include "resource.h"
#include "DataList_DockBar.h"

IMPLEMENT_DYNAMIC ( DataList_DockBar, CAcUiDockControlBar )

BEGIN_MESSAGE_MAP( DataList_DockBar, CAcUiDockControlBar )
    ON_WM_CREATE()
    ON_WM_SYSCOMMAND()
    ON_WM_SIZE()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
//----- DataList_DockBar *pInstance = new DataList_DockBar;
//----- pInstance->Create (acedGetAcadFrame (), "My title bar") ;
//----- pInstance->EnableDocking (CBRS_ALIGN_ANY) ;
//----- pInstance->RestoreControlBar () ;

//-----------------------------------------------------------------------------
static CLSID clsGEList_DockBar = {0x9dd9391a, 0xc2da, 0x4d2e, { 0x8e, 0x27, 0x95, 0xad, 0xe, 0x8b, 0xba, 0xc8 }} ;


//-----------------------------------------------------------------------------
DataList_DockBar::DataList_DockBar () : CAcUiDockControlBar()
{
}

//-----------------------------------------------------------------------------
DataList_DockBar::~DataList_DockBar ()
{

}

//-----------------------------------------------------------------------------
#ifdef _DEBUG
//- Please uncomment the 2 following lines to avoid linker error when compiling
//- in release mode. But make sure to uncomment these lines only once per project
//- if you derive multiple times from CAdUiDockControlBar/CAcUiDockControlBar
//- classes.

//void CAdUiDockControlBar::AssertValid () const {
//}
#endif

//-----------------------------------------------------------------------------
BOOL DataList_DockBar::Create ( CWnd* pParent, LPCTSTR lpszTitle )
{
    CString strWndClass ;
    strWndClass = AfxRegisterWndClass ( CS_DBLCLKS, LoadCursor ( NULL, IDC_ARROW ) ) ;
    CRect rect ( 0, 0, 500, 200 ) ;
    if ( !CAcUiDockControlBar::Create (
                strWndClass, lpszTitle, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN,
                rect, pParent, 0
            )
       )
        return ( FALSE ) ;

    SetToolID ( &clsGEList_DockBar ) ;


    return ( TRUE ) ;
}

//-----------------------------------------------------------------------------
//----- This member function is called when an application requests the window be
//----- created by calling the Create or CreateEx member function
int DataList_DockBar::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
    if ( CAcUiDockControlBar::OnCreate ( lpCreateStruct ) == -1 )
        return ( -1 ) ;

    //----- Point to our resource
    CAcModuleResourceOverride resourceOverride;
    //----- Create it and set the parent as the dockctrl bar
    mChildDlg.Create ( IDD_DATA_LIST_DLG, this ) ;

    CRect rect ;
    GetWindowRect ( &rect ) ;
    //- Move the window over so we can see the control lines
    mChildDlg.MoveWindow ( 0, 0, rect.Width (), rect.Height (), TRUE ) ;

    return ( 0 ) ;
}

//-----------------------------------------------------------------------------
void DataList_DockBar::SizeChanged ( CRect* lpRect, BOOL bFloating, int flags )
{
    // If valid
    if ( ::IsWindow ( mChildDlg.GetSafeHwnd () ) )
    {
        //----- Always point to our resource to be safe
        CAcModuleResourceOverride resourceOverride ;
        //----- Then update its window size relatively
        mChildDlg.SetWindowPos ( this, lpRect->left , lpRect->top , lpRect->Width (), lpRect->Height (), SWP_NOZORDER ) ;
    }
}

//-----------------------------------------------------------------------------
//-----  Function called when user selects a command from Control menu or when user
//----- selects the Maximize or the Minimize button.
void DataList_DockBar::OnSysCommand ( UINT nID, LPARAM lParam )
{
    CAcUiDockControlBar::OnSysCommand ( nID, lParam ) ;
}

//-----------------------------------------------------------------------------
//----- The framework calls this member function after the window's size has changed
void DataList_DockBar::OnSize ( UINT nType, int cx, int cy )
{
    CAcUiDockControlBar::OnSize ( nType, cx, cy ) ;
    // If valid
    if ( ::IsWindow ( mChildDlg.GetSafeHwnd () ) )
    {
        //----- Always point to our resource to be safe
        CAcModuleResourceOverride resourceOverride ;
        //----- then update its window position relatively
        mChildDlg.MoveWindow ( 0, 0, cx, cy ) ;
    }
}

bool DataList_DockBar::OnClosing()
{
    bool ret = CAdUiDockControlBar::OnClosing();
    //MessageBox(_T("调用AirSetDockBar::OnClosing()"));

    /* 手动添加 */
    mChildDlg.OnClosing();

    return ret;
}