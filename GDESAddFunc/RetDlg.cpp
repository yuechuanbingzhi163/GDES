#include "stdafx.h"
#include "RetDlg.h"
#include "config.h"


IMPLEMENT_DYNAMIC(RetDlg, GasBaseAcesDlg)

RetDlg::RetDlg(CWnd* pParent /*=NULL*/)
: GasBaseAcesDlg(RetDlg::IDD, pParent)
, m_pumpRet(_T(""))
, m_sysRet(_T(""))
, m_yearRet(_T(""))
, m_preRet(_T(""))
, m_rateRet(_T(""))
, m_ret(_T(""))
{

}

RetDlg::~RetDlg()
{
}

void RetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RET_PUMP_EDIT, m_pumpRet);
	DDX_Text(pDX, IDC_RET_SYS_EDIT, m_sysRet);
	DDX_Text(pDX, IDC_RET_YEAR_EDIT, m_yearRet);
	DDX_Text(pDX, IDC_RET_PRE_EDIT, m_preRet);
	DDX_Text(pDX, IDC_RET_RATE_EDIT, m_rateRet);
	DDX_Text(pDX, IDC_RET_EDIT, m_ret);
}


BEGIN_MESSAGE_MAP(RetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &RetDlg::OnBnClickedOk)
END_MESSAGE_MAP()

static BOOL IsNum(CString &str)
{
	int n=str.GetLength();
	for(int i=0;i<n;i++)
		if ((str[i]<'0'||str[i]>'9') && str[i] != '.') 
			return FALSE;
	return TRUE;
}

BOOL RetDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(MINE_GAS_CAPACITY,datasVector,1);
	m_pumpRet = m_sysRet 
		= m_yearRet = m_preRet 
		= m_rateRet = m_ret = _T("0");
	int len = datasVector.size();
	if(!datasVector.empty())
	{
		m_pumpRet = datasVector[len-2][0].kACharPtr();
		m_sysRet = datasVector[len-1][0].kACharPtr();
	}

	datasVector.clear();
	ReportDataHelper::ReadDatas(YEAR_REAL_GAS,datasVector,1);
	len = datasVector.size();
	if(!datasVector.empty())
		m_yearRet = datasVector[len-1][0].kACharPtr();

	datasVector.clear();
	ReportDataHelper::ReadDatas(PRE_GAS,datasVector,1);
	len = datasVector.size();
	if(!datasVector.empty())
		m_preRet = datasVector[len-1][0].kACharPtr();

	datasVector.clear();
	ReportDataHelper::ReadDatas(RATE_GAS,datasVector,1);
	len = datasVector.size();
	if(!datasVector.empty())
		m_rateRet = datasVector[len-1][0].kACharPtr();

	GetRet();
	UpdateData(FALSE);
	return TRUE;
}

void RetDlg::GetRet()
{
	double pumpRet,sysRet,yearRet,preRet,rateRet;
	if(!IsNum(m_pumpRet) || !IsNum(m_sysRet) 
		|| !IsNum(m_yearRet) || !IsNum(m_preRet) 
		|| !IsNum(m_rateRet)) return;
	pumpRet = _tstof(m_pumpRet);
	sysRet = _tstof(m_sysRet);
	yearRet = _tstof(m_yearRet);
	preRet = _tstof(m_preRet);
	rateRet = _tstof(m_rateRet);

	double minValue = 1e100;
	if(minValue >= pumpRet) minValue = pumpRet;
	if (minValue >= sysRet) minValue = sysRet;
	if(minValue >= yearRet) minValue = yearRet;
	if(minValue >= preRet) minValue = preRet;
	if(minValue >= rateRet) minValue = rateRet;
	m_ret.Format(_T("%.2lf"),minValue);
}
// RetDlg 消息处理程序

void RetDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	ArrayVector dataVector;
	AcStringArray dataArray;
	dataArray.append(m_ret);

	dataVector.push_back(dataArray);

	ReportDataHelper::WriteDatas(RET_GAS,dataVector);
	CDialog::OnOK();
}
