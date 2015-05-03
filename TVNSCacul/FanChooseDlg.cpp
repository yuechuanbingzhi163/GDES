// FanChooseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FanChooseDlg.h"


// FanChooseDlg 对话框

IMPLEMENT_DYNAMIC(FanChooseDlg, CDialog)

FanChooseDlg::FanChooseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(FanChooseDlg::IDD, pParent)
{
}

FanChooseDlg::~FanChooseDlg()
{
}

void FanChooseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FanChooseDlg, CDialog)
	ON_BN_CLICKED(IDOK, &FanChooseDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL FanChooseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CButton* radioPower=(CButton*)GetDlgItem(IDC_POWER_RADIO);
	radioPower->SetCheck(TRUE);
	return TRUE;
}

// FanChooseDlg 消息处理程序

void FanChooseDlg::OnBnClickedOk()
{
	CButton* radioPower=(CButton*)GetDlgItem(IDC_POWER_RADIO);

	int powerSelted = radioPower->GetCheck();
	if (1 == powerSelted)
	{
		m_chFanType = POWER;
	}
	else
	{
		m_chFanType = QUANTITY;
	}
	OnOK();
}
