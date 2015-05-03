#include "stdafx.h"
#include "CheakFanDlg.h"
#include "../MineGE/HelperClass.h"

// CheakFanDlg 对话框

IMPLEMENT_DYNAMIC(CheakFanDlg, CDialog)

CheakFanDlg::CheakFanDlg(CWnd* pParent /*=NULL*/,AcDbObjectId fanId)
	: CDialog(CheakFanDlg::IDD, pParent),m_fanId(fanId)
	, m_fantype(_T(""))
	, m_minQ(_T(""))
	, m_maxQ(_T(""))
	, m_minH(_T(""))
	, m_maxH(_T(""))
{

}

CheakFanDlg::~CheakFanDlg()
{
}

void CheakFanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FAN_TYPE_EDIT, m_fantype);
	DDX_Text(pDX, IDC_MIN_Q_EDIT, m_minQ);
	DDX_Text(pDX, IDC_MAX_Q_EDIT2, m_maxQ);
	DDX_Text(pDX, IDC_MIN_H_EDIT2, m_minH);
	DDX_Text(pDX, IDC_MAX_H_EDIT3, m_maxH);
}


BEGIN_MESSAGE_MAP(CheakFanDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CheakFanDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CheakFanDlg::readPropertyData()
{
	CString randQ,randH;
	DataHelper::GetPropertyData(m_fanId,_T("理论风量(m3/s)"),m_thearyQ);
	DataHelper::GetPropertyData(m_fanId,_T("理论风压(Pa)"),m_thearyH);
	DataHelper::GetPropertyData(m_fanId,_T("风量范围"),randQ);
	DataHelper::GetPropertyData(m_fanId,_T("风压范围"),randH);
	DataHelper::GetPropertyData(m_fanId,_T("风机型号"),m_fantype);
	TCHAR *token;
	TCHAR *sep = _T("~");
	token = _tcstok((LPTSTR)(LPCTSTR)randQ,sep);
	m_minQ = token;
	sep = _T("(");
	token = _tcstok(NULL,sep);
	m_maxQ = token;

	sep = _T("~");
	token = _tcstok((LPTSTR)(LPCTSTR)randH,sep);
	m_minH = token;
	sep = _T("(");
	token = _tcstok(NULL,sep);
	m_maxH = token;

	UpdateData(FALSE);
}

void CheakFanDlg::writePropertyData()
{
	UpdateData(TRUE);

	DataHelper::SetPropertyData(m_fanId,_T("风机型号"),m_fantype);
	if(m_minQ.IsEmpty() || m_maxQ.IsEmpty() || m_minH.IsEmpty() || m_maxH.IsEmpty()) return;
	DataHelper::SetPropertyData(m_fanId,_T("风量范围"),m_minQ + _T("~") + m_maxQ+_T("(m3/min)"));
	DataHelper::SetPropertyData(m_fanId,_T("风压范围"),m_minH + _T("~") + m_maxH + _T("(Pa)"));
}

void CheakFanDlg::OnBnClickedOk()
{
	GetDlgItem(IDC_MIN_Q_EDIT)->GetWindowText(m_minQ);
	GetDlgItem(IDC_MAX_Q_EDIT2)->GetWindowText(m_maxQ);
	GetDlgItem(IDC_MIN_H_EDIT2)->GetWindowText(m_minH);
	GetDlgItem(IDC_MAX_H_EDIT3)->GetWindowText(m_maxH);

	double q = _tstof(m_thearyQ)*60;
	double h = _tstof(m_thearyH);
	double minQ = _tstof(m_minQ);
	double maxQ = _tstof(m_maxQ);
	double minH = _tstof(m_minH);
	double maxH = _tstof(m_maxH);
	//acutPrintf(_T("\nq:%lf\nh:%lf\nminQ:%lf\nmaxQ:%lf\nminH:%lf\nmaxH:%lf"),q,h,minQ,maxQ,minH,maxH);
	//acutPrintf(_T("\n风量:%.2lfm^3/min\t风压:%.2lfPa"),q,h);
	if(q <= maxQ && q >= minQ && h <= maxH && h >= minH && minQ >= 0 \
		&& maxQ > 0 && minH >= 0 && maxH > 0)
	{
		//CString qStr;
		//qStr.Format(_T("%.2lf"),q);
		AfxMessageBox(_T("风机型号合理"),MB_OKCANCEL | MB_ICONINFORMATION); 
		//OnOK();
	}

	else
	{
		//CString qStr;
		//qStr.Format(_T("%.2lf"),q);
		AfxMessageBox(_T("风机型号不合理"),MB_OKCANCEL | MB_ICONINFORMATION); 
	}
	writePropertyData();
	OnOK();
}

BOOL CheakFanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	readPropertyData();
	return TRUE;
}
