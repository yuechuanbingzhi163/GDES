#include "StdAfx.h"
#include "PropertyData_DockBarChildDlg.h"

IMPLEMENT_DYNAMIC ( PropertyData_DockBarChildDlg, DockBarChildDlg )

BEGIN_MESSAGE_MAP( PropertyData_DockBarChildDlg, DockBarChildDlg )
    //{{AFX_MSG_MAP(PropertyData_DockBarChildDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
PropertyData_DockBarChildDlg::PropertyData_DockBarChildDlg( UINT idd, CWnd* pParent, HINSTANCE hInstance )
    : DockBarChildDlg ( idd, pParent, hInstance )
{
    //{{AFX_DATA_INIT(PropertyData_DockBarChildDlg)
    //}}AFX_DATA_INIT
}

PropertyData_DockBarChildDlg::~PropertyData_DockBarChildDlg()
{
}

void PropertyData_DockBarChildDlg::updatePropertyData( bool save )
{
    if( save )
    {
        UpdateData( TRUE );

        acDocManager->lockDocument( curDoc() );
        writePropertyData();
        acDocManager->unlockDocument( curDoc() );
    }
    else
    {
        initMoreChildDlg();

        acDocManager->lockDocument( curDoc() );
        readPropertyData();
        acDocManager->unlockDocument( curDoc() );

        UpdateData( FALSE );
    }
}