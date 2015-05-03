#include "stdafx.h"
#include "InspectionDataDlg.h"


IMPLEMENT_DYNAMIC(InspectionDataDlg, GasBaseAcesDlg)

InspectionDataDlg::InspectionDataDlg(CWnd* pParent /*=NULL*/)
	: GasBaseAcesDlg(InspectionDataDlg::IDD, pParent)
	, m_DWB(TRUE)
	, m_PST(TRUE)
	, m_CT(TRUE)
	, m_AR(TRUE)
	, m_OID(TRUE)
{

}

InspectionDataDlg::~InspectionDataDlg()
{
}

void InspectionDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_DWB_CHECK, m_DWB);
	DDX_Check(pDX, IDC_PST_CHECK, m_PST);
	DDX_Check(pDX, IDC_CT_CHECK, m_CT);
	DDX_Check(pDX, IDC_AR_CHECK, m_AR);
	DDX_Check(pDX, IDC_OID_CHECK, m_OID);
}


BEGIN_MESSAGE_MAP(InspectionDataDlg, CDialog)
	ON_BN_CLICKED(IDC_DWB_CHECK, &InspectionDataDlg::OnBnClickedDwbCheck)
	ON_BN_CLICKED(IDOK, &InspectionDataDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL InspectionDataDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(INSPECT_BOOKMKS_OBJCT_NAME,datasVector,2);
	if(!datasVector.empty())
	{
		m_DWB = _ttoi(datasVector[0][1]);
		m_PST = _ttoi(datasVector[1][1]);
		m_CT = _ttoi(datasVector[2][1]);
		m_AR = _ttoi(datasVector[3][1]);
		m_OID = _ttoi(datasVector[4][1]);
	}
	SetEnableDWB(m_DWB);
	UpdateData(FALSE);
	return TRUE;
}

static BOOL ShowRetDlg()
{
	ResultDlg retDlg;
	AcStringArray datas;
	datas.append(_T("抽采竣工验收资料真实有效"));
	datas.append(_T("抽采工程竣工验收资料不真实"));
	datas.append(_T("缺少必要的抽采工程竣工验收资料"));
	datas.append(_T("其他"));
	retDlg.setItemDatas(datas);

	IntArray trueIndxs;
	trueIndxs.push_back(0);
	retDlg.setTrueIndex(trueIndxs,RET_INSPECT_OBJCT_NAME,DESC_INSPECT_OBJCT_NAME);

	AcStringArray bookMks;
	bookMks.append(_T("InsData_Ret"));
	bookMks.append(_T("InsData_Method"));
	bookMks.append(_T("InsData_Conclusion"));
	bookMks.append(_T("InsData_YesOrNo"));
	retDlg.setBookMarks(bookMks);

	if(IDOK == retDlg.DoModal()) 
	{
		return TRUE;
	}
	else return FALSE;
}

void InspectionDataDlg::OnBnClickedDwbCheck()
{
	UpdateData(TRUE);
	m_PST = m_DWB;
	m_CT = m_DWB;
	m_AR = m_DWB;
	SetEnableDWB(m_DWB);
	UpdateData(FALSE);
}

void InspectionDataDlg::OnBnClickedOk()
{
	AcStringArray dataArray,nameArray;
	nameArray.append(_T("InsData_DWB"));
	nameArray.append(_T("InsData_PST"));
	nameArray.append(_T("InsData_CT"));
	nameArray.append(_T("InsData_AR"));
	nameArray.append(_T("InsData_OID"));
	UpdateData(TRUE);
	vector<BOOL> nameVector;
	nameVector.push_back(m_DWB);
	nameVector.push_back(m_PST);
	nameVector.push_back(m_CT);
	nameVector.push_back(m_AR);
	nameVector.push_back(m_OID);
	for(int i = 0; i < nameVector.size();i++)
	{
		AcString data;
		data.format(_T("%d"),nameVector[i]);
		dataArray.append(data);
	}
	ArrayVector dataVector;
	dataVector.push_back(nameArray);
	dataVector.push_back(dataArray);
	ReportDataHelper::WriteDatas(INSPECT_BOOKMKS_OBJCT_NAME,dataVector);

	if(!ShowRetDlg()) return;
	OnOK();}

void InspectionDataDlg::SetEnableDWB( BOOL isTrue )
{
	GetDlgItem(IDC_PST_CHECK)->EnableWindow(isTrue);
	GetDlgItem(IDC_CT_CHECK)->EnableWindow(isTrue);
	GetDlgItem(IDC_AR_CHECK)->EnableWindow(isTrue);
}