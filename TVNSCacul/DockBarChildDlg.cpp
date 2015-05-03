#include "StdAfx.h"
#include "DockBarChildDlg.h"

IMPLEMENT_DYNAMIC ( DockBarChildDlg, CAcUiDialog )

BEGIN_MESSAGE_MAP( DockBarChildDlg, CAcUiDialog )
    //{{AFX_MSG_MAP(DockBarChildDlg)
    ON_MESSAGE( WM_ACAD_KEEPFOCUS, OnAcadKeepFocus )  // Needed for modeless dialog.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
DockBarChildDlg::DockBarChildDlg( UINT idd, CWnd* pParent, HINSTANCE hInstance )
    : CAcUiDialog ( idd, pParent, hInstance )
{
    //{{AFX_DATA_INIT(DockBarChildDlg)
    //}}AFX_DATA_INIT
}

DockBarChildDlg::~DockBarChildDlg()
{
}

//- Needed for modeless dialogs to keep focus.
//- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT DockBarChildDlg::OnAcadKeepFocus ( WPARAM wParam, LPARAM lParam )
{
    return ( TRUE ) ;
}

//- As this dialog is a child dialog we need to disable ok and cancel
BOOL DockBarChildDlg::OnCommand ( WPARAM wParam, LPARAM lParam )
{
    switch ( wParam )
    {
    case IDOK:
    case IDCANCEL:
        return ( FALSE ) ;
    }
    return ( CAcUiDialog::OnCommand ( wParam, lParam ) ) ;
}

void DockBarChildDlg::OnSize ( UINT nType, int cx, int cy )
{
    CAcUiDialog::OnSize ( nType, cx, cy ) ;
    //- Now update the dialog
    MoveWindow ( 0, 0, cx, cy ) ;
}

void DockBarChildDlg::OnClosing()
{
    // do nothing
}