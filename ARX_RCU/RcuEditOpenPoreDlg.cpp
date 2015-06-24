// RcuEditClosePoreDlg.cpp : 实现文件
#include "stdafx.h"
#include "RcuEditOpenPoreDlg.h"


IMPLEMENT_DYNAMIC(RcuEditOpenPoreDlg, RcuAcUiBaseDlg)

RcuEditOpenPoreDlg::RcuEditOpenPoreDlg(CWnd* pParent /*=NULL*/)
	: RcuAcUiBaseDlg(RcuEditOpenPoreDlg::IDD, pParent)
	, m_num(10)
	, m_gap(0.1)
	, m_radius(0)
{

}

RcuEditOpenPoreDlg::~RcuEditOpenPoreDlg()
{
}

void RcuEditOpenPoreDlg::DoDataExchange(CDataExchange* pDX)
{
	RcuAcUiBaseDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_num);
	DDX_Text(pDX, IDC_EDIT2, m_gap);
	DDX_Text(pDX, IDC_EDIT3, m_radius);
}


BEGIN_MESSAGE_MAP(RcuEditOpenPoreDlg, RcuAcUiBaseDlg)
	ON_BN_CLICKED(IDOK, &RcuEditOpenPoreDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// RcuEditClosePoreDlg 消息处理程序

void RcuEditOpenPoreDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL RcuEditOpenPoreDlg::OnInitDialog()
{
	RcuAcUiBaseDlg::OnInitDialog();

	//修改标题
	if(!m_title.IsEmpty())
	{
		this->SetWindowText(m_title);
	}
	//修改钻场信息
	this->SetDlgItemText(IDC_DRILL_SITE_TEXT, m_pos);

	//更新数据到界面
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
