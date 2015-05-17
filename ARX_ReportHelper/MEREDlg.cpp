// MEREDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MEREDlg.h"


// MEREDlg 对话框

IMPLEMENT_DYNAMIC(MEREDlg, GasBaseAcesDlg)

MEREDlg::MEREDlg(CWnd* pParent /*=NULL*/)
	: GasBaseAcesDlg(MEREDlg::IDD, pParent)
	, m_yesOrNo(TRUE)
	, m_des(_T(""))
{

}

MEREDlg::~MEREDlg()
{
}

void MEREDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MERE_METHOD_COMBO, m_methodCombBox);
	DDX_Check(pDX, IDC_MERE_YESNO_CHECK, m_yesOrNo);
	DDX_Text(pDX, IDC_DES_EDIT, m_des);
}


BEGIN_MESSAGE_MAP(MEREDlg, CDialog)
	ON_BN_CLICKED(IDOK, &MEREDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL MEREDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();
	SetWindowText(m_objName);
	m_methodCombBox.AddString(_T("现场核查"));
	m_methodCombBox.AddString(_T("资料审查"));
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(m_objName,datasVector,2);
	if(datasVector.empty()) 
	{
		m_methodCombBox.SetCurSel(0);
		UpdateData(FALSE);
		return TRUE;
	}
	m_methodCombBox.SetWindowText(datasVector[1][1]);
	m_yesOrNo = _ttoi(datasVector[3][1]);
	//m_methodCombBox.SetCurSel(0);
	UpdateData(FALSE);
	return TRUE;
}

// MEREDlg 消息处理程序

void MEREDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CString strMethod;
	m_methodCombBox.GetWindowText(strMethod);
	writeDatasToFile(m_yesOrNo,strMethod);
	OnOK();
}

void MEREDlg::setObjectName( const CString& objName )
{
	m_objName = objName;
}

void MEREDlg::setBookMarks( const AcStringArray& bookMks )
{
	m_bookMks.removeAll();
	m_bookMks.append(bookMks);
}

void MEREDlg::writeDatasToFile(BOOL yesOrNo,const CString& strMethod)
{
	CString strIsTrue;
	strIsTrue.Format(_T("%d"),yesOrNo);
	
	CString temp = _T("经");
	temp.Append(strMethod);
	temp.Append(_T("认为"));
	CString strRet;
	strRet.Append(m_objName);
	if(!yesOrNo)  strRet.Append(_T("不"));
	strRet.Append(_T("满足评判要求"));

	CString strConlusion = strRet;
	strRet = temp + strRet;
	strRet.Append(_T("。"));
	AcStringArray datas;
	datas.append(strRet);
	datas.append(strMethod);
	datas.append(strConlusion);
	datas.append(strIsTrue);

	ArrayVector dataVector;
	dataVector.push_back(m_bookMks);
	dataVector.push_back(datas);
	ReportDataHelper::WriteDatas(m_objName,dataVector);
}

void MEREDlg::setDesText( const CString& des )
{
	m_des = des;
	//UpdateData(FALSE);
}