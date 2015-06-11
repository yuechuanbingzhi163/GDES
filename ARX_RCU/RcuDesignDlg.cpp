// RcuDesignDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RcuDesignDlg.h"


// RcuDesignDlg 对话框

IMPLEMENT_DYNAMIC(RcuDesignDlg, CAcUiDialog)

RcuDesignDlg::RcuDesignDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(RcuDesignDlg::IDD, pParent)
{

}

RcuDesignDlg::~RcuDesignDlg()
{
}

void RcuDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_rcuList);
}


BEGIN_MESSAGE_MAP(RcuDesignDlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &RcuDesignDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &RcuDesignDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &RcuDesignDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &RcuDesignDlg::OnBnClickedButton4)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &RcuDesignDlg::OnHdnItemdblclickList1)
END_MESSAGE_MAP()

BOOL RcuDesignDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// RcuDesignDlg 消息处理程序

void RcuDesignDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void RcuDesignDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
}

void RcuDesignDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
}

void RcuDesignDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
}

void RcuDesignDlg::OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

