#include "stdafx.h"
#include "AcUiBaseDlg.h"


IMPLEMENT_DYNAMIC(AcUiBaseDlg, CAcUiDialog)


AcUiBaseDlg::AcUiBaseDlg( UINT idd,CWnd* pParent /*= NULL*/,HINSTANCE hDialogResource/*=NULL*/ )
: CAcUiDialog(idd, pParent,hDialogResource)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_GAS_ICON);
}
AcUiBaseDlg::~AcUiBaseDlg()
{
}

void AcUiBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AcUiBaseDlg, CAcUiDialog)
END_MESSAGE_MAP()

BOOL AcUiBaseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
