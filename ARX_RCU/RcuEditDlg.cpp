// RcuDesignDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RcuEditDlg.h"


// RcuDesignDlg 对话框

IMPLEMENT_DYNAMIC(RcuEditDlg, CAcUiDialog)

RcuEditDlg::RcuEditDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(RcuEditDlg::IDD, pParent)
	, m_x(0)
	, m_y(0)
	, m_z(0)
	, m_length(0)
	, m_width(0)
	, m_height(0)
	, m_left(0)
	, m_right(0)
	, m_top(0)
	, m_bottom(0)
	, m_radius(0)
	, m_thick(0)
	, m_angle(0)
	, m_dist(0)
{

}

RcuEditDlg::~RcuEditDlg()
{
}

void RcuEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, m_x);
	DDX_Text(pDX, IDC_EDIT7, m_y);
	DDX_Text(pDX, IDC_EDIT9, m_z);
	DDX_Text(pDX, IDC_EDIT1, m_length);
	DDX_Text(pDX, IDC_EDIT2, m_width);
	DDX_Text(pDX, IDC_EDIT3, m_height);
	DDX_Text(pDX, IDC_EDIT10, m_left);
	DDX_Text(pDX, IDC_EDIT11, m_right);
	DDX_Text(pDX, IDC_EDIT12, m_top);
	DDX_Text(pDX, IDC_EDIT13, m_bottom);
	DDX_Text(pDX, IDC_EDIT17, m_radius);
	DDX_Text(pDX, IDC_EDIT14, m_thick);
	DDX_Text(pDX, IDC_EDIT15, m_angle);
	DDX_Text(pDX, IDC_EDIT16, m_dist);
	DDX_Control(pDX, IDC_LIST1, m_siteList);
}


BEGIN_MESSAGE_MAP(RcuEditDlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON4, &RcuEditDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &RcuEditDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &RcuEditDlg::OnBnClickedButton6)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &RcuEditDlg::OnHdnItemdblclickList1)
END_MESSAGE_MAP()

BOOL RcuEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void RcuEditDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
}

void RcuEditDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
}

void RcuEditDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
}

void RcuEditDlg::OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
