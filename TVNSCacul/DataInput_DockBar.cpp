#include "StdAfx.h"
#include "resource.h"
#include "DataInput_DockBar.h"

IMPLEMENT_DYNAMIC ( DataInput_DockBar, CAcUiDockControlBar )

BEGIN_MESSAGE_MAP( DataInput_DockBar, CAcUiDockControlBar )
    ON_WM_CREATE()
    ON_WM_SYSCOMMAND()
    ON_WM_SIZE()
END_MESSAGE_MAP()

//----- DataInput_DockBar *pInstance = new DataInput_DockBar;
//----- pInstance->Create (acedGetAcadFrame (), "My title bar") ;
//----- pInstance->EnableDocking (CBRS_ALIGN_ANY) ;
//----- pInstance->RestoreControlBar () ;

static CLSID clsVenEvalDataInput_DockBar = {0x9a7e71d1, 0xbc27, 0x4476, { 0xa3, 0x7e, 0xb5, 0x18, 0xe5, 0x81, 0x45, 0x58 }} ;


DataInput_DockBar::DataInput_DockBar () : CAcUiDockControlBar(), m_di( DI_HELP )
{
}

DataInput_DockBar::~DataInput_DockBar ()
{
}

#ifdef _DEBUG
//- Please uncomment the 2 following lines to avoid linker error when compiling
//- in release mode. But make sure to uncomment these lines only once per project
//- if you derive multiple times from CAdUiDockControlBar/CAcUiDockControlBar
//- classes.

//void CAdUiDockControlBar::AssertValid () const {
//}
#endif

BOOL DataInput_DockBar::Create ( CWnd* pParent, LPCTSTR lpszTitle )
{
    CString strWndClass ;
    strWndClass = AfxRegisterWndClass ( CS_DBLCLKS, LoadCursor ( NULL, IDC_ARROW ) ) ;
    CRect rect ( 0, 0, 140, 180 ) ;
    if ( !CAcUiDockControlBar::Create (
                strWndClass, lpszTitle, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN,
                rect, pParent, 0
            )
       )
        return ( FALSE ) ;

    SetToolID ( &clsVenEvalDataInput_DockBar ) ;

    // TODO: Add your code here

    return ( TRUE ) ;
}

static void ChildOnCreate( PropertyData_DockBarChildDlg* pDlg, long idd, CWnd* pParent )
{
    CAcModuleResourceOverride resourceOverride;
    pDlg->Create( idd, pParent );
    //pDlg->.ShowWindow(SW_HIDE);
}

//----- This member function is called when an application requests the window be
//----- created by calling the Create or CreateEx member function
int DataInput_DockBar::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
    if ( CAcUiDockControlBar::OnCreate ( lpCreateStruct ) == -1 )
        return ( -1 ) ;
    dlgArray[0] = &m_helpDlg;
	dlgArray[1] = &m_PropertyDlg;
	//dlgArray[2] = &m_ttunnelDlg;
	//dlgArray[3] = &m_chimneyDlg;
	//dlgArray[4] = &m_fanDlg;
	//dlgArray[5] = &m_libDlg;

    ChildOnCreate( dlgArray[DI_HELP], IDD_HELP_DLG, this );
	ChildOnCreate( dlgArray[DI_TUNNEL], IDD_PROPERTY_GRID_DLG, this );
	//ChildOnCreate( dlgArray[DI_TTUNNEL], IDD_TTUNNEL_PARAM_DLG, this );
	//ChildOnCreate( dlgArray[DI_CHIMNEY], IDD_CHIMNEY_PARAM_DLG, this );
	//ChildOnCreate( dlgArray[DI_FAN], IDD_FAN_DIALOG, this );
	//ChildOnCreate( dlgArray[DI_LIB], IDD_LIB_DIALOG, this );

	CRect rect ;
    GetWindowRect( &rect );
    ScreenToClient( &rect );
    dlgArray[m_di]->MoveWindow( 0, 0, rect.Width (), rect.Height (), TRUE );
    dlgArray[m_di]->ShowWindow( SW_SHOW );

    return ( 0 ) ;
}


static void ChildSizeChanged( PropertyData_DockBarChildDlg* pDlg, CWnd* pParent, CRect* lpRect )
{
    //// If valid
    if ( ::IsWindow( pDlg->GetSafeHwnd () ) )
    {
        //----- Always point to our resource to be safe
        CAcModuleResourceOverride resourceOverride ;
        pDlg->SetWindowPos ( pParent, lpRect->left , lpRect->top , lpRect->Width (), lpRect->Height (), SWP_NOZORDER ) ;
    }
}

void DataInput_DockBar::SizeChanged ( CRect* lpRect, BOOL bFloating, int flags )
{
    ChildSizeChanged( dlgArray[DI_HELP], this, lpRect );
	ChildSizeChanged( dlgArray[DI_TUNNEL], this, lpRect );
	//ChildSizeChanged( dlgArray[DI_TTUNNEL], this, lpRect );
	//ChildSizeChanged( dlgArray[DI_CHIMNEY], this, lpRect );
	//ChildSizeChanged( dlgArray[DI_FAN], this, lpRect );
	//ChildSizeChanged( dlgArray[DI_LIB], this, lpRect );
}

//-----  Function called when user selects a command from Control menu or when user
//----- selects the Maximize or the Minimize button.
void DataInput_DockBar::OnSysCommand ( UINT nID, LPARAM lParam )
{
    CAcUiDockControlBar::OnSysCommand ( nID, lParam ) ;
}

static void ChildOnSize( PropertyData_DockBarChildDlg* pDlg, int cx, int cy )
{
    // If valid
    if ( ::IsWindow( pDlg->GetSafeHwnd () ) )
    {
        //----- Always point to our resource to be safe
        CAcModuleResourceOverride resourceOverride ;
        //----- then update its window position relatively
        pDlg->MoveWindow ( 0, 0, cx, cy ) ;
    }
}
//----- The framework calls this member function after the window's size has changed
void DataInput_DockBar::OnSize ( UINT nType, int cx, int cy )
{
    CAcUiDockControlBar::OnSize ( nType, cx, cy ) ;

    ChildOnSize( dlgArray[DI_HELP], cx, cy );
	ChildOnSize( dlgArray[DI_TUNNEL], cx, cy );
	//ChildOnSize( dlgArray[DI_TTUNNEL], cx, cy );
	//ChildOnSize( dlgArray[DI_CHIMNEY], cx, cy );
	//ChildOnSize( dlgArray[DI_FAN], cx, cy );
	//ChildOnSize( dlgArray[DI_LIB], cx, cy );
}

bool DataInput_DockBar::OnClosing()
{
    bool ret = CAdUiDockControlBar::OnClosing();

    /* 手动添加 */
    //mChildDlg.OnClosing();

    dlgArray[m_di]->updatePropertyData( true );

    m_di = DI_HELP;
    m_objId = AcDbObjectId::kNull;

    return ret;
}

void DataInput_DockBar::showDlg( DI_TYPE di )
{
	for( int i = 0; i < 2; i++ )
    {
        int nCmdShow = ( ( di == i ) ? SW_SHOW : SW_HIDE );
        dlgArray[i]->ShowWindow( nCmdShow );
    }
}

void DataInput_DockBar::update( DI_TYPE di, const AcDbObjectId& objId )
{
    //acutPrintf(_T("\n原始id:%d,选择id:%d.."), m_objId, objId);

    dlgArray[m_di]->updatePropertyData( true );

    m_di = di;
    m_objId = objId;

    dlgArray[di]->m_objId = objId;
	//acutPrintf(_T("\n当前呗选择的ID:%Dd"),objId.asOldId());
    dlgArray[di]->updatePropertyData( false );
    showDlg( di );
}