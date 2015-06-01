// PumpCapacityDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PumpCapacityDlg.h"


// PumpCapacityDlg 对话框

IMPLEMENT_DYNAMIC(PumpCapacityDlg, GasBaseAcesDlg)

PumpCapacityDlg::PumpCapacityDlg(CWnd* pParent /*=NULL*/)
	: GasBaseAcesDlg(PumpCapacityDlg::IDD, pParent)
	, m_absGas(_T(""))
	, m_gasConcentration(_T(""))
	, m_maxQ(_T(""))
	, m_localP(_T(""))
	, m_surplus(_T(""))
	, m_workCondiction(_T(""))
	, m_numPump(_T(""))
{

}

PumpCapacityDlg::~PumpCapacityDlg()
{
}

void PumpCapacityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ABS_GAS_EDIT, m_absGas);
	DDX_Text(pDX, IDC_CONCENTRA_GAS_EDIT, m_gasConcentration);
	DDX_Text(pDX, IDC_MINE_MAXQ_EDIT, m_maxQ);
	DDX_Text(pDX, IDC_LOCAL_AP_EDIT, m_localP);
	DDX_Text(pDX, IDC_OVERFLOWER_EDIT, m_surplus);
	DDX_Text(pDX, IDC_SYS_CON_EDIT, m_workCondiction);
	DDX_Text(pDX, IDC_EDIT4, m_numPump);
	DDX_Control(pDX, IDC_PUMP_LIST, m_pumpListCtrl);
}


BEGIN_MESSAGE_MAP(PumpCapacityDlg, CDialog)
END_MESSAGE_MAP()

BOOL PumpCapacityDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();
	return TRUE;
}

// PumpCapacityDlg 消息处理程序
