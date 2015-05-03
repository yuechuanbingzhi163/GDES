// SelfManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SelfManageDlg.h"


// SelfManageDlg 对话框

IMPLEMENT_DYNAMIC(SelfManageDlg, GasBaseAcesDlg)

SelfManageDlg::SelfManageDlg(CWnd* pParent /*=NULL*/)
	: GasBaseAcesDlg(SelfManageDlg::IDD, pParent)
	, m_GDES(TRUE)
	, m_GDJR(TRUE)
	, m_GDMAR(TRUE)
	, m_DPIA(TRUE)
	, m_AFDRM(TRUE)
	, m_TFMS(TRUE)
{

}

SelfManageDlg::~SelfManageDlg()
{
}

void SelfManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_GDES_CHECK, m_GDES);
	DDX_Check(pDX, IDC_GDJR_CHECK, m_GDJR);
	DDX_Check(pDX, IDC_GDMAR_CHECK, m_GDMAR);
	DDX_Check(pDX, IDC_DPIA_CHECK, m_DPIA);
	DDX_Check(pDX, IDC_AFDRM_CHECK, m_AFDRM);
	DDX_Check(pDX, IDC_TFMS_CHECK, m_TFMS);
}


BEGIN_MESSAGE_MAP(SelfManageDlg, CDialog)
	ON_BN_CLICKED(IDOK, &SelfManageDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL SelfManageDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(SM_BOOKMKS_OBJCT_NAME,datasVector,2);
	if(!datasVector.empty())
	{
		m_GDES = _ttoi(datasVector[0][1]);
		m_GDJR = _ttoi(datasVector[1][1]);
		m_GDMAR = _ttoi(datasVector[2][1]);
		m_DPIA = _ttoi(datasVector[3][1]);
		m_AFDRM = _ttoi(datasVector[4][1]);
		m_TFMS = _ttoi(datasVector[5][1]);
	}

	UpdateData(FALSE);
	return TRUE;
}

static BOOL ShowRetDlg()
{
	ResultDlg retDlg;
	AcStringArray datas;
	datas.append(_T("具有完善的瓦斯抽采达标自我评价体系和瓦斯抽采管理制度"));
	datas.append(_T("缺少瓦斯抽采达标自我评价体系等相关管理文件，抽采管理不达标"));
	datas.append(_T("其他"));
	retDlg.setItemDatas(datas);

	IntArray trueIndxs;
	trueIndxs.push_back(0);
	retDlg.setTrueIndex(trueIndxs,RET_SM_OBJCT_NAME,DESC_SM_OBJCT_NAME);

	AcStringArray bookMks;
	bookMks.append(_T("SM_Ret"));
	bookMks.append(_T("SM_Method"));
	bookMks.append(_T("SM_Conclusion"));
	bookMks.append(_T("SM_YesOrNo"));
	retDlg.setBookMarks(bookMks);

	if(IDOK == retDlg.DoModal()) 
	{
		return TRUE;
	}
	else return FALSE;
}

void SelfManageDlg::OnBnClickedOk()
{
	AcStringArray dataArray,nameArray;
	nameArray.append(_T("SM_GDES"));
	nameArray.append(_T("SM_GDJR"));
	nameArray.append(_T("SM_GDMAR"));
	nameArray.append(_T("SM_DPIA"));
	nameArray.append(_T("SM_AFDRM"));
	nameArray.append(_T("SM_TFMS"));
	UpdateData(TRUE);
	vector<BOOL> nameVector;
	nameVector.push_back(m_GDES);
	nameVector.push_back(m_GDJR);
	nameVector.push_back(m_GDMAR);
	nameVector.push_back(m_DPIA);
	nameVector.push_back(m_AFDRM);
	nameVector.push_back(m_TFMS);
	for(int i = 0; i < nameVector.size();i++)
	{
		AcString data;
		data.format(_T("%d"),nameVector[i]);
		dataArray.append(data);
	}

	ArrayVector dataVector;
	dataVector.push_back(nameArray);
	dataVector.push_back(dataArray);
	ReportDataHelper::WriteDatas(SM_BOOKMKS_OBJCT_NAME,dataVector);
	if(!ShowRetDlg()) return;
	OnOK();
}
