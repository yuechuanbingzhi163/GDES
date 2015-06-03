#include "stdafx.h"
#include "RateGasDlg.h"
#include "Caculate.h"
#include "config.h"

// RateGasDlg 对话框

IMPLEMENT_DYNAMIC(RateGasDlg, GasBaseAcesDlg)

RateGasDlg::RateGasDlg(CWnd* pParent /*=NULL*/)
	: GasBaseAcesDlg(RateGasDlg::IDD, pParent)
	, m_relativeGas(_T(""))
	, m_absGas(_T(""))
	, m_ret(_T(""))
{

}

RateGasDlg::~RateGasDlg()
{
}

void RateGasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RATE_RELATIVE_GAS_EDIT, m_relativeGas);
	DDX_Text(pDX, IDC_RATE_ABS_GAS_EDIT, m_absGas);
	DDX_Text(pDX, IDC_RATE_RET_EDIT, m_ret);
}


BEGIN_MESSAGE_MAP(RateGasDlg, CDialog)
	ON_BN_CLICKED(IDOK, &RateGasDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// RateGasDlg 消息处理程序

void RateGasDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	ArrayVector dataVector;
	AcStringArray dataArray;
	dataArray.append(m_relativeGas);
	dataArray.append(m_absGas);

	CString strRet;
	if(!Calculate::MineRateGasCacul(dataArray,strRet))
	{
		AfxMessageBox(_T("数据错误!"));
	}

	dataArray.append(strRet);
	dataVector.push_back(dataArray);

	ReportDataHelper::WriteDatas(RATE_GAS,dataVector);

	m_ret = strRet;
	UpdateData(FALSE);
}

BOOL RateGasDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(RATE_GAS,datasVector,1);
	if(!datasVector.empty())
	{
		m_relativeGas = datasVector[0][0].kACharPtr();
		m_absGas = datasVector[1][0].kACharPtr();
		m_ret = datasVector[2][0].kACharPtr();
	}

	UpdateData(FALSE);
	return TRUE;

}