#include "stdafx.h"
#include "RcuAcUiBaseDlg.h"


IMPLEMENT_DYNAMIC(RcuAcUiBaseDlg, CAcUiDialog)


RcuAcUiBaseDlg::RcuAcUiBaseDlg( UINT idd,CWnd* pParent /*= NULL*/,HINSTANCE hDialogResource/*=NULL*/ )
: CAcUiDialog(idd, pParent,hDialogResource)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_GAS_ICON);
}
RcuAcUiBaseDlg::~RcuAcUiBaseDlg()
{
}

void RcuAcUiBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RcuAcUiBaseDlg, CAcUiDialog)
END_MESSAGE_MAP()

BOOL RcuAcUiBaseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	return TRUE;  // return TRUE unless you set the focus to a control
}
