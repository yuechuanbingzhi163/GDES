#include "StdAfx.h"
#include "PropertyDataBaseDlg.h"

IMPLEMENT_DYNAMIC ( PropertyDataBaseDlg, CAcUiDialog )

BEGIN_MESSAGE_MAP( PropertyDataBaseDlg, CAcUiDialog )
    //{{AFX_MSG_MAP(PropertyDataBaseDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
PropertyDataBaseDlg::PropertyDataBaseDlg( UINT idd, CWnd* pParent, HINSTANCE hInstance )
    : CAcUiDialog ( idd, pParent, hInstance )
{
    //{{AFX_DATA_INIT(PropertyDataBaseDlg)
    //}}AFX_DATA_INIT
}

PropertyDataBaseDlg::~PropertyDataBaseDlg()
{
}
