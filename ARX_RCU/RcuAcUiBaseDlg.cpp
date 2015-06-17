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
	EnableToolTips(TRUE);
	m_tt.Create(this);
	m_tt.Activate(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL RcuAcUiBaseDlg::PreTranslateMessage( MSG* pMsg )
{
	if (NULL != m_tt.GetSafeHwnd())
	{
		m_tt.RelayEvent(pMsg);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void RcuAcUiBaseDlg::SetToolTip( int itemID,const CString& tooltip )
{
	CWnd* pW=GetDlgItem(itemID);//得到控件的指针
	m_tt.AddTool(pW,tooltip);//为此控件添加tip
}