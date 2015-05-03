// GasPlanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GasPlanDlg.h"

// GasPlanDlg 对话框

IMPLEMENT_DYNAMIC(GasPlanDlg, GasBaseAcesDlg)

GasPlanDlg::GasPlanDlg(CWnd* pParent /*=NULL*/)
	: GasBaseAcesDlg(GasPlanDlg::IDD, pParent)
	, m_range(TRUE)
	, m_SEE(TRUE)
	, m_EEF(TRUE)
	, m_CP(TRUE)
	, m_CMFA(TRUE)
	, m_CRE(TRUE)
	, m_DEQ(TRUE)
	, m_APS(TRUE)
	, m_TTV(TRUE)
	, m_DFP(TRUE)
	, m_AEP(TRUE)
	, m_YEEF(TRUE)
	, m_YMFA(TRUE)
	, m_YRE(TRUE)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDI_GASSYS_ICON);
}

GasPlanDlg::~GasPlanDlg()
{
}

void GasPlanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_RANGE_CHECK, m_range);
	DDX_Check(pDX, IDC_SEE_CHECK, m_SEE);
	DDX_Check(pDX, IDC_EEF_CHECK, m_EEF);
	DDX_Check(pDX, IDC_CP_CHECK, m_CP);
	DDX_Check(pDX, IDC_CMFA_CHECK, m_CMFA);
	DDX_Check(pDX, IDC_CRE_CHECK, m_CRE);
	DDX_Check(pDX, IDC_DEQ_CHECK, m_DEQ);
	DDX_Check(pDX, IDC_APS_CHECK, m_APS);
	DDX_Check(pDX, IDC_TTV_CHECK, m_TTV);
	DDX_Check(pDX, IDC_DFP_CHECK, m_DFP);
	DDX_Check(pDX, IDC_AEP_CHECK, m_AEP);
	DDX_Check(pDX, IDC_YEEF_CHECK, m_YEEF);
	DDX_Check(pDX, IDC_YMFA_CHECK, m_YMFA);
	DDX_Check(pDX, IDC_YRE_CHECK, m_YRE);
}


BEGIN_MESSAGE_MAP(GasPlanDlg, CDialog)
	ON_BN_CLICKED(IDOK, &GasPlanDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// GasPlanDlg 消息处理程序

static BOOL ShowRetDlg()
{
	ResultDlg retDlg;
	AcStringArray datas;
	datas.append(_T("具有合理完整的抽采达标规划和年度实施计划，能够确保抽、掘、采平衡"));
	datas.append(_T("缺少部分工程规划文件，但基本能够安排抽、掘、采三者的接替关系"));
	datas.append(_T("缺少必要的瓦斯抽采规划和年度实施计划，无法保障抽、掘、采平衡"));
	datas.append(_T("其他"));
	retDlg.setItemDatas(datas);

	IntArray trueIndxs;
	trueIndxs.push_back(0);
	trueIndxs.push_back(1);
	retDlg.setTrueIndex(trueIndxs,RET_GASPLAN_OBJCT_NAME,DESC_GASPUMP_OBJCT_NAME);

	AcStringArray bookMks;
	bookMks.append(_T("GasPlan_Ret"));
	bookMks.append(_T("GasPlan_Method"));
	bookMks.append(_T("GasPlan_Conclusion"));
	bookMks.append(_T("GasPlan_YesOrNo"));
	retDlg.setBookMarks(bookMks);

	if(IDOK == retDlg.DoModal()) 
	{
		return TRUE;
	}
	else return FALSE;
}

void GasPlanDlg::OnBnClickedOk()
{
	AcStringArray dataArray,nameArray;
	nameArray.append(_T("Table_range"));
	nameArray.append(_T("Table_SEE"));
	nameArray.append(_T("Table_DEQ"));
	nameArray.append(_T("Table_EEF"));
	nameArray.append(_T("Table_CP"));
	nameArray.append(_T("Table_CMFA"));
	nameArray.append(_T("Table_CRE"));
	nameArray.append(_T("Table_APS"));
	nameArray.append(_T("Table_TTV"));
	nameArray.append(_T("Table_DFP"));
	nameArray.append(_T("Table_AEP"));
	nameArray.append(_T("Table_YEEF"));
	nameArray.append(_T("Table_YMFA"));
	nameArray.append(_T("Table_YRE"));
	UpdateData(TRUE);
	vector<BOOL> nameVector;
	nameVector.push_back(m_range);
	nameVector.push_back(m_SEE);
	nameVector.push_back(m_DEQ);
	nameVector.push_back(m_EEF);
	nameVector.push_back(m_CP);
	nameVector.push_back(m_CMFA);
	nameVector.push_back(m_CRE);
	nameVector.push_back(m_APS);
	nameVector.push_back(m_TTV);
	nameVector.push_back(m_DFP);
	nameVector.push_back(m_AEP);
	nameVector.push_back(m_YEEF);
	nameVector.push_back(m_YMFA);
	nameVector.push_back(m_YRE);
	for(int i = 0; i < nameVector.size();i++)
	{
		AcString data;
		data.format(_T("%d"),nameVector[i]);
		dataArray.append(data);
	}

	ArrayVector dataVector;
	dataVector.push_back(nameArray);
	dataVector.push_back(dataArray);
	ReportDataHelper::WriteDatas(GAS_PLAN_BOOKMKS_OBJCT_NAME,dataVector);
	//WriteDataToFile( _T("bgpvars.txt"), nameArray, dataArray );
	if(!ShowRetDlg()) return;
	OnOK();
}

BOOL GasPlanDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(GAS_PLAN_BOOKMKS_OBJCT_NAME,datasVector,2);
	if(!datasVector.empty())
	{
		m_range = _ttoi(datasVector[0][1]);
		m_SEE = _ttoi(datasVector[1][1]);
		m_DEQ = _ttoi(datasVector[2][1]);
		m_EEF = _ttoi(datasVector[3][1]);
		m_CP = _ttoi(datasVector[4][1]);
		m_CMFA = _ttoi(datasVector[5][1]);
		m_CRE = _ttoi(datasVector[6][1]);
		m_APS = _ttoi(datasVector[7][1]);
		m_TTV = _ttoi(datasVector[8][1]);
		m_DFP = _ttoi(datasVector[9][1]);
		m_AEP = _ttoi(datasVector[10][1]);
		m_YEEF = _ttoi(datasVector[11][1]);
		m_YMFA = _ttoi(datasVector[12][1]);
		m_YRE = _ttoi(datasVector[13][1]);
	}

	UpdateData(FALSE);
	return TRUE;
}
