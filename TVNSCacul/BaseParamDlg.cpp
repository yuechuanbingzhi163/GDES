#include "stdafx.h"
#include "BaseParamDlg.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

extern CString ReadIntStringDatas(int intData,CString typeName);
extern int ReadStringIntDatas(CString strData,CString typeName);

IMPLEMENT_DYNAMIC(BaseParamDlg, CDialog)

BaseParamDlg::BaseParamDlg(CWnd* pParent /*=NULL*/,AcDbObjectId objId)
: CDialog(BaseParamDlg::IDD, pParent)
, m_name(_T(""))
, m_district(_T(""))
{
	m_objId = objId;
}

BaseParamDlg::~BaseParamDlg()
{
}

void BaseParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME_EDIT, m_name);
	DDX_Text(pDX, IDC_DISTRICT_EDIT, m_district);
	DDX_Control(pDX, IDC_METHODS_COMBO, m_method);
	DDX_Control(pDX, IDC_WAYS_COMBO, m_way);
}


BEGIN_MESSAGE_MAP(BaseParamDlg, CDialog)
	ON_BN_CLICKED( IDOK, &BaseParamDlg::OnBnClickedOk )
END_MESSAGE_MAP()

void BaseParamDlg::readPropertyData()
{
	//AcDbObjectId objId;
	//SingleDataObjectHelper::GetObjectId( _T( "基本参数" ), objId );

	DataHelper::GetPropertyData( m_objId, _T( "矿井名称" ), m_name );
	DataHelper::GetPropertyData( m_objId, _T( "所属采(盘)区" ), m_district );


	m_method.AddString( _T( "压入式" ) );
	m_method.AddString( _T( "抽出式" ) );
	m_method.AddString( _T( "长抽短压" ) );
	m_method.AddString( _T( "长压短抽" ) );

	
	CString v1;
	DataHelper::GetPropertyData( m_objId, _T( "通风方法" ), v1 );
	
	int idx1 = ReadStringIntDatas(v1,_T("通风方法"));
	m_method.SetCurSel( idx1 );

	m_way.AddString( _T( "单机单风筒" ) );
	m_way.AddString( _T( "辅助风筒" ) );
	m_way.AddString( _T( "间断风筒" ) );
	//m_way.AddString( _T( "单机双列" ) );
	m_way.AddString( _T( "串联局扇" ) );
	m_way.AddString( _T( "混合通风" ) );
	m_way.AddString( _T( "风柜通风" ) );
	m_way.AddString( _T( "风库通风" ) );
	m_way.AddString( _T( "钻孔通风" ) );

	CString v2;
	DataHelper::GetPropertyData( m_objId, _T( "通风方式" ), v2 );

	int idx2 = ReadStringIntDatas(v2,_T("通风方式"));
	m_way.SetCurSel( idx2 );
	
	//acutPrintf(_T("\nReading:%d\t%s"),idx2,v2);
	UpdateData( FALSE );
}

void BaseParamDlg::writePropertyData()
{
	UpdateData( TRUE );

	//AcDbObjectId objId;
	//SingleDataObjectHelper::GetObjectId( _T( "基本参数" ), objId );

	DataHelper::SetPropertyData( m_objId, _T( "矿井名称" ), m_name );
	DataHelper::SetPropertyData( m_objId, _T( "所属采(盘)区" ), m_district );

	CString rString1 = ReadIntStringDatas(m_method.GetCurSel(),_T("通风方法"));
	DataHelper::SetPropertyData( m_objId, _T( "通风方法" ), rString1 );

	CString rString2 = ReadIntStringDatas(m_way.GetCurSel(),_T("通风方式"));
	DataHelper::SetPropertyData( m_objId, _T( "通风方式" ), rString2 );
	
	//acutPrintf(_T("\nWriting:%d\t%s"),m_way.GetCurSel(),rString2);

}

BOOL BaseParamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	readPropertyData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE

}

void BaseParamDlg::OnBnClickedOk()
{
	writePropertyData();
	OnOK();
}
