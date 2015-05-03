#include "stdafx.h"
#include "FanMethoChoosedDlg.h"
#include "../MineGE/HelperClass.h"

extern CString ReadIntStringDatas(int intData,CString typeName);
extern int ReadStringIntDatas(CString strData,CString typeName);

// FanMethoChoosedDlg 对话框

IMPLEMENT_DYNAMIC(FanMethoChoosedDlg, PropertyData_DockBarChildDlg)

FanMethoChoosedDlg::FanMethoChoosedDlg(CWnd* pParent /*=NULL*/)
	: PropertyData_DockBarChildDlg(FanMethoChoosedDlg::IDD, pParent)
	, m_fanRank(_T(""))
	, m_ventMethod(_T(""))
	, m_ventWay(_T(""))
{

}

FanMethoChoosedDlg::~FanMethoChoosedDlg()
{
}

void FanMethoChoosedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MEATHOD_COMBO, m_method);
	DDX_Text(pDX, IDC_RANK_EDIT, m_fanRank);
	DDX_Text(pDX, IDC_METHOD_DISP_EDIT, m_ventMethod);
	DDX_Text(pDX, IDC_WAY_DISP_EDIT2, m_ventWay);
}


BEGIN_MESSAGE_MAP(FanMethoChoosedDlg, PropertyData_DockBarChildDlg)
END_MESSAGE_MAP()

void FanMethoChoosedDlg::readPropertyData()
{
	DataHelper::GetPropertyData(m_objId,_T("风机级别"),m_fanRank);
	CString v;
	DataHelper::GetPropertyData( m_objId, _T( "工作方式" ), v );

	int idx = ReadStringIntDatas(v,_T("工作方式"));

	m_method.SetCurSel(idx);

	DataHelper::GetPropertyData(m_objId,_T("通风方式"),m_ventMethod);
	DataHelper::GetPropertyData(m_objId,_T("通风方法"),m_ventWay);

	if (m_fanRank.IsEmpty())
	{
		m_fanRank = _T("1");
	}
	
	UpdateData(FALSE);
}

void FanMethoChoosedDlg::writePropertyData()
{
	UpdateData(TRUE);
	DataHelper::SetPropertyData(m_objId,_T("风机级别"),m_fanRank);
	CString rString = ReadIntStringDatas(m_method.GetCurSel(),_T("工作方式"));
	
	//if (rString.IsEmpty())
	//{
	//	if (_T("压入式") == m_ventWay || _T("长压短抽") == m_ventWay)
	//	{
	//		rString = _T("压入式");
	//	}
	//	else
	//	{
	//		rString = _T("抽出式");
	//	}
	//}

	DataHelper::SetPropertyData( m_objId, _T( "工作方式" ), rString );
	DataHelper::SetPropertyData(m_objId,_T("通风方式"),m_ventMethod);
	DataHelper::SetPropertyData(m_objId,_T("通风方法"),m_ventWay);
}

BOOL FanMethoChoosedDlg::OnInitDialog()
{
	PropertyData_DockBarChildDlg::OnInitDialog();

	m_method.AddString( _T( "压入式" ) );
	m_method.AddString( _T( "抽出式" ) );

	readPropertyData();
	return TRUE;

}
