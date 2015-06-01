#include "stdafx.h"
#include "PumpCapacityDlg.h"
#include "config.h"

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
	ON_BN_CLICKED(IDOK, &PumpCapacityDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL PumpCapacityDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();

	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(MINE_GAS_PUMP_CAPACITY,datasVector,1);
	if(!datasVector.empty())
	{
		m_absGas = datasVector[0][0].kACharPtr();
		m_gasConcentration = datasVector[1][0].kACharPtr();
		m_maxQ = datasVector[2][0].kACharPtr();
		m_localP = datasVector[3][0].kACharPtr();
		m_surplus = datasVector[4][0].kACharPtr();
		m_workCondiction = datasVector[5][0].kACharPtr();
		m_numPump = datasVector[6][0].kACharPtr();
	}

	UpdateData(FALSE);

	return TRUE;
}

// PumpCapacityDlg 消息处理程序

void PumpCapacityDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	ArrayVector dataVector;
	AcStringArray dataArray;
	dataArray.append(m_absGas);
	dataArray.append(m_gasConcentration);
	dataArray.append(m_maxQ);
	dataArray.append(m_localP);
	dataArray.append(m_surplus);
	dataArray.append(m_workCondiction);
	dataArray.append(m_numPump);

	dataVector.push_back(dataArray);

	ReportDataHelper::WriteDatas(MINE_GAS_PUMP_CAPACITY,dataVector);
}
