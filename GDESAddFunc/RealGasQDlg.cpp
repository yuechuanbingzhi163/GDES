#include "stdafx.h"
#include "RealGasQDlg.h"
#include "Caculate.h"
#include "config.h"


// RealGasQDlg 对话框

IMPLEMENT_DYNAMIC(RealGasQDlg, GasBaseAcesDlg)

RealGasQDlg::RealGasQDlg(CWnd* pParent /*=NULL*/)
	: GasBaseAcesDlg(RealGasQDlg::IDD, pParent)
	, m_lastGas(_T(""))
	, m_maxGas(_T(""))
	, m_gasCon(_T(""))
	, m_maxQ(_T(""))
	, m_k(_T(""))
	,m_yearRet(_T(""))
{

}

RealGasQDlg::~RealGasQDlg()
{
}

void RealGasQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_YEAR_ABS_GAS_EDIT, m_lastGas);
	DDX_Text(pDX, IDC_YEAR_MAXQ_EDIT, m_maxGas);
	DDX_Text(pDX, IDC_YEAR_CONCENTRA_GAS_EDIT, m_gasCon);
	DDX_Text(pDX, IDC_YEAR_MINE_MAXQ_EDIT, m_maxQ);
	DDX_Text(pDX, IDC_YEAR_K_EDIT, m_k);
	DDX_Text(pDX, IDC_YEAR_RET_EDIT, m_yearRet);
}


BEGIN_MESSAGE_MAP(RealGasQDlg, CDialog)
	ON_BN_CLICKED(IDOK, &RealGasQDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL RealGasQDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(YEAR_REAL_GAS,datasVector,1);
	m_k = _T("1.4");
	if(!datasVector.empty())
	{
		m_lastGas = datasVector[0][0].kACharPtr();
		m_maxGas = datasVector[1][0].kACharPtr();
		m_gasCon = datasVector[2][0].kACharPtr();
		m_maxQ = datasVector[3][0].kACharPtr();
		m_k = datasVector[4][0].kACharPtr();
		m_yearRet = datasVector[5][0].kACharPtr();
	}


	UpdateData(FALSE);

	//SetToolTip(IDC_YEAR_MAXQ_EDIT,_T("最大抽采吨煤量"));
	return TRUE;
}

// RealGasQDlg 消息处理程序

void RealGasQDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	ArrayVector dataVector;
	AcStringArray dataArray;
	dataArray.append(m_lastGas);
	dataArray.append(m_maxGas);
	dataArray.append(m_gasCon);
	dataArray.append(m_maxQ);
	dataArray.append(m_k);


	CString strYearRet;
	if(!Calculate::MineGasRealCacul(dataArray,strYearRet))
	{
		AfxMessageBox(_T("数据错误!"));
	}

	dataArray.append(strYearRet);
	dataVector.push_back(dataArray);

	ReportDataHelper::WriteDatas(YEAR_REAL_GAS,dataVector);

	m_yearRet = strYearRet;
	UpdateData(FALSE);
}
