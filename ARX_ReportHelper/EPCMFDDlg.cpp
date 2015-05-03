// EPCMFDDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EPCMFDDlg.h"


// EPCMFDDlg 对话框

IMPLEMENT_DYNAMIC(EPCMFDDlg, GasBaseAcesDlg)

EPCMFDDlg::EPCMFDDlg(CWnd* pParent /*=NULL*/)
	: GasBaseAcesDlg(EPCMFDDlg::IDD, pParent)
	, m_DDSP(TRUE)
	, m_DCSEL(TRUE)
	, m_SFCR(TRUE)
	, m_CEME(TRUE)
	, m_MFCD(TRUE)
	, m_DPCRD(TRUE)
	, m_CES(TRUE)
	, m_EROM(TRUE)
	, m_ETPG(TRUE)
	, m_SM(TRUE)
	, m_BL(TRUE)
{

}

EPCMFDDlg::~EPCMFDDlg()
{
}

void EPCMFDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_DDSP_CHECK, m_DDSP);
	DDX_Check(pDX, IDC_DCSEL_CHECK, m_DCSEL);
	DDX_Check(pDX, IDC_SFCR_CHECK, m_SFCR);
	DDX_Check(pDX, IDC_CEME_CHECK, m_CEME);
	DDX_Check(pDX, IDC_MFCD_CHECK, m_MFCD);
	DDX_Check(pDX, IDC_DPCRD_CHECK, m_DPCRD);
	DDX_Check(pDX, IDC_CES_CHECK, m_CES);
	DDX_Check(pDX, IDC_EROM_CHECK, m_EROM);
	DDX_Check(pDX, IDC_ETPG_CHECK, m_ETPG);
	DDX_Check(pDX, IDC_SM_CHECK, m_SM);
	DDX_Check(pDX, IDC_BL_CHECK, m_BL);
}


BEGIN_MESSAGE_MAP(EPCMFDDlg, CDialog)
	ON_BN_CLICKED(IDOK, &EPCMFDDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_DDSP_CHECK, &EPCMFDDlg::OnBnClickedDdspCheck)
	ON_BN_CLICKED(IDC_MFCD_CHECK, &EPCMFDDlg::OnBnClickedMfcdCheck)
END_MESSAGE_MAP()


static BOOL ShowRetDlg()
{
	ResultDlg retDlg;
	AcStringArray datas;
	datas.append(_T("具有完善的抽采达标工艺方案设计，各采掘工作面抽采施工设计完善合理"));
	datas.append(_T("缺少部分管理文件，能基本满足规定要求"));
	datas.append(_T("缺少必要的设计施工文件，瓦斯抽采管理不完善"));
	datas.append(_T("其他"));
	retDlg.setItemDatas(datas);

	IntArray trueIndxs;
	trueIndxs.push_back(0);
	trueIndxs.push_back(1);
	retDlg.setTrueIndex(trueIndxs,RET_EPCMFD_OBJCT_NAME,DESC_EPCMFD_OBJCT_NAME);

	AcStringArray bookMks;
	bookMks.append(_T("EPCMFD_Ret"));
	bookMks.append(_T("EPCMFD_Method"));
	bookMks.append(_T("EPCMFD_Conclusion"));
	bookMks.append(_T("EPCMFD_YesOrNo"));
	retDlg.setBookMarks(bookMks);

	if(IDOK == retDlg.DoModal()) 
	{
		return TRUE;
	}
	else return FALSE;
}

void EPCMFDDlg::OnBnClickedOk()
{
	AcStringArray dataArray,nameArray;
	nameArray.append(_T("EPCMFD_DDSP"));
	nameArray.append(_T("EPCMFD_DCSEL"));
	nameArray.append(_T("EPCMFD_SFCR"));
	nameArray.append(_T("EPCMFD_CEME"));
	nameArray.append(_T("EPCMFD_MFCD"));
	nameArray.append(_T("EPCMFD_DPCRD"));
	nameArray.append(_T("EPCMFD_CES"));
	nameArray.append(_T("EPCMFD_EROM"));
	nameArray.append(_T("EPCMFD_BL"));
	nameArray.append(_T("EPCMFD_ETPG"));
	nameArray.append(_T("EPCMFD_SM"));
	UpdateData(TRUE);
	vector<BOOL> nameVector;
	nameVector.push_back(m_DDSP);
	nameVector.push_back(m_DCSEL);
	nameVector.push_back(m_SFCR);
	nameVector.push_back(m_CEME);
	nameVector.push_back(m_MFCD);
	nameVector.push_back(m_DPCRD);
	nameVector.push_back(m_CES);
	nameVector.push_back(m_EROM);
	nameVector.push_back(m_BL);
	nameVector.push_back(m_ETPG);
	nameVector.push_back(m_SM);
	for(int i = 0; i < nameVector.size();i++)
	{
		AcString data;
		data.format(_T("%d"),nameVector[i]);
		dataArray.append(data);
	}

	ArrayVector dataVector;
	dataVector.push_back(nameArray);
	dataVector.push_back(dataArray);
	ReportDataHelper::WriteDatas(EPCMFD_BOOKMKS_OBJCT_NAME,dataVector);
	if(!ShowRetDlg()) return;
	OnOK();
}

void EPCMFDDlg::OnBnClickedDdspCheck()
{
	UpdateData(TRUE);

	m_DCSEL = m_DDSP;
	m_SFCR = m_DDSP;
	m_CEME = m_DDSP;
	SetEnableDDSP(m_DDSP);
	UpdateData(FALSE);
}

void EPCMFDDlg::OnBnClickedMfcdCheck()
{
	UpdateData(TRUE);

	m_DPCRD = m_MFCD;
	m_CES = m_MFCD;
	m_EROM = m_MFCD;
	m_BL = m_MFCD;
	m_ETPG = m_MFCD;
	m_SM = m_MFCD;
	SetEnableMFCD(m_MFCD);
	UpdateData(FALSE);
}

BOOL EPCMFDDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(EPCMFD_BOOKMKS_OBJCT_NAME,datasVector,2);
	if(!datasVector.empty())
	{
		m_DDSP = _ttoi(datasVector[0][1]);
		m_DCSEL = _ttoi(datasVector[1][1]);
		m_SFCR = _ttoi(datasVector[2][1]);
		m_CEME = _ttoi(datasVector[3][1]);
		m_MFCD = _ttoi(datasVector[4][1]);
		m_DPCRD = _ttoi(datasVector[5][1]);
		m_CES = _ttoi(datasVector[6][1]);
		m_EROM = _ttoi(datasVector[7][1]);
		m_BL = _ttoi(datasVector[8][1]);
		m_ETPG = _ttoi(datasVector[9][1]);
		m_SM = _ttoi(datasVector[10][1]);
	}
	SetEnableDDSP(m_DDSP);
	SetEnableMFCD(m_MFCD);
	UpdateData(FALSE);
	return TRUE;
}

void EPCMFDDlg::SetEnableDDSP( BOOL isTrue )
{
	GetDlgItem(IDC_DCSEL_CHECK)->EnableWindow(isTrue);
	GetDlgItem(IDC_SFCR_CHECK)->EnableWindow(isTrue);
	GetDlgItem(IDC_CEME_CHECK)->EnableWindow(isTrue);
}

void EPCMFDDlg::SetEnableMFCD( BOOL isTrue )
{
	GetDlgItem(IDC_DPCRD_CHECK)->EnableWindow(isTrue);
	GetDlgItem(IDC_CES_CHECK)->EnableWindow(isTrue);
	GetDlgItem(IDC_EROM_CHECK)->EnableWindow(isTrue);
	GetDlgItem(IDC_BL_CHECK)->EnableWindow(isTrue);
	GetDlgItem(IDC_ETPG_CHECK)->EnableWindow(isTrue);
	GetDlgItem(IDC_SM_CHECK)->EnableWindow(isTrue);
}