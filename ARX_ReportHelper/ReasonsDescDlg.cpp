// ReasonsDescDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ReasonsDescDlg.h"


// ReasonsDescDlg 对话框

IMPLEMENT_DYNAMIC(ReasonsDescDlg, CDialog)

ReasonsDescDlg::ReasonsDescDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ReasonsDescDlg::IDD, pParent)
	, m_reasonsDesc(_T(""))
	, m_yesOrNo(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_GASSYS_ICON);
}

ReasonsDescDlg::~ReasonsDescDlg()
{
}

void ReasonsDescDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_REASONS_DESC_RICHEDIT2, m_reasonsDesc);
	DDX_Check(pDX, IDC_YesOrNo_CHECK, m_yesOrNo);
}


BEGIN_MESSAGE_MAP(ReasonsDescDlg, CDialog)
	ON_BN_CLICKED(IDOK, &ReasonsDescDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL ReasonsDescDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CFont font;
	font.CreatePointFont(120,_T("宋体"),NULL);
	GetDlgItem(IDC_REASONS_DESC_RICHEDIT2)->SetFont(&font);
	
	//不能输入太多字，否则在读写word的时候会崩溃
	CEdit* editCtrl =  (CEdit*)GetDlgItem(IDC_REASONS_DESC_RICHEDIT2);
	editCtrl->LimitText(_MAX_PATH / 2);
	UpdateData(FALSE);
	return TRUE;
}

// ReasonsDescDlg 消息处理程序

void ReasonsDescDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if(m_reasonsDesc.IsEmpty())
	{
		AfxMessageBox(_T("请填写自述内容"));
		return;
	}
	OnOK();
}
