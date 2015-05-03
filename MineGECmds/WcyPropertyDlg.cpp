#include "stdafx.h"
#include "WcyPropertyDlg.h"

#include "../MineGE/DataHelper.h"
#include "../MineGE/FieldHelper.h"
#include "../MineGE/PropertyDataUpdater.h"
#include "../MineGE/FuncFieldHelper.h"
#include "../MineGE/config.h"
#include "../MineGE/CustomProperties.h"

IMPLEMENT_DYNAMIC(WcyPropertyDlg, CDialog)

WcyPropertyDlg::WcyPropertyDlg( CWnd* pParent /*=NULL*/,CString func )
: CDialog( WcyPropertyDlg::IDD, pParent )
{
	m_showAll = true; // 默认显示全部数据
	m_func = func;
}

WcyPropertyDlg::~WcyPropertyDlg()
{
}

void WcyPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CACUL_MATHOD_COMBO, m_method);
}


BEGIN_MESSAGE_MAP(WcyPropertyDlg, CDialog)
	ON_BN_CLICKED( IDOK, &WcyPropertyDlg::OnBnClickedOk )
	ON_CBN_SELCHANGE(IDC_CACUL_MATHOD_COMBO, &WcyPropertyDlg::OnCbnSelchangeCaculMathodCombo)
END_MESSAGE_MAP()


// WcyPropertyDlg 消息处理程序

void WcyPropertyDlg::OnCbnSelchangeCaculMathodCombo()
{
	int indx = m_method.GetCurSel();
	switch(indx)
	{
	case 0:
		//通过瓦斯抽采量计算
		m_func = _T("通过瓦斯抽采量计算");
		break;

	case 1:
		//通过残余瓦斯压力反算
		m_func = _T("通过残余瓦斯压力反算");
		break;

	default:
		m_func = _T("");
		break;
	}
}

BOOL WcyPropertyDlg::InitPropGridCtrl()
{
	// 创建, 定位, 显示CMFCPropertyGridCtrl
	CRect rect;
	GetDlgItem( IDC_PROP_POS2 )->GetWindowRect( &rect );
	ScreenToClient( &rect );
	m_propertyDataList.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rect, this, ( UINT ) - 1 );
	//m_propertyDataList.SetAlphabeticMode(true);
	// 获取类型名称
	CString type;
	DataHelper::GetTypeName( m_objId, type );

	AcStringArray funcs,funcFieldsInfo;
	if(m_func.IsEmpty())
	{
		FuncFieldHelper::GetFunctions(funcs);
	}
	else
	{
		funcs.append(m_func);
	}
	int funcNum = funcs.length();
	for (int i = 0; i < funcNum; i++)
	{
		AcStringArray fields;
		FuncFieldHelper::GetFields(funcs[i].kACharPtr(),type,fields);
		if(fields.isEmpty()) continue;
		funcFieldsInfo.append(_T("$"));
		funcFieldsInfo.append(funcs[i]);
		for(int j = 0; j < fields.length(); j++)
		{
			funcFieldsInfo.append(fields[j]);
			//acutPrintf(_T("\n功能:%s\t字段:%s"),funcs[i].kACharPtr(),fields[j].kACharPtr());
		}
	}

	PropertyDataUpdater::BuildPropGridCtrl( &m_propertyDataList, type,funcFieldsInfo );

	// 填充属性数据
	bool ret = PropertyDataUpdater::ReadDataFromGE( &m_propertyDataList, m_objId );

	// 填充数据失败
	if( !ret )
	{
		m_propertyDataList.EnableWindow( FALSE );
		m_propertyDataList.ShowWindow( SW_HIDE );
	}
	return TRUE;
}


BOOL WcyPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_method.AddString( _T( "通过瓦斯抽采量计算" ) );
	m_method.AddString( _T( "通过残余瓦斯压力反算" ) );

	 return InitPropGridCtrl();
	 if(m_func.IsEmpty())
	 {
		 return FALSE;
	 }
	// 创建属性数据控件


	//return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE

}

void WcyPropertyDlg::setMineGE( const AcDbObjectId& objId )
{
	m_objId = objId;
}

void WcyPropertyDlg::addField( const CString& field )
{
	m_fields.append( field );
}

void WcyPropertyDlg::showAllData( bool bFlag )
{
	m_showAll = bFlag;
}

static void HighlightGridProperty(CMFCPropertyGridCtrl* pPropDataList, bool bHighlight)
{
	int nCount = pPropDataList->GetPropertyCount();
	for ( int i = 0; i < nCount; i++ )
	{
		CMFCPropertyGridProperty* pGroup = pPropDataList->GetProperty( i );
		for(int j = 0; j < pGroup->GetSubItemsCount(); j++)
		{
			CMFCPropertyGridProperty* pProp = pGroup->GetSubItem( j );
			if(!pProp->IsEnabled())
			{
				CustomGridProperty* cProp = (CustomGridProperty*)pProp;
				cProp->highlight(bHighlight);
			}
		}
	}
}

void WcyPropertyDlg::OnBnClickedOk()
{
    UpdateData( TRUE ); // 更新控件

    // 更新图元的属性数据
    PropertyDataUpdater::WriteDataToGE( &m_propertyDataList, m_objId );
	

	if(m_func.IsEmpty() || _T("基本信息") == m_func)
	{
		OnOK();
		return;
	}

	//计算功能
	//UpdateData(FALSE);
	//if(!caculate())
	//{
	//	//OnOK();
	//	return;
	//}

	SetTimer(100,500,NULL);
	AfxMessageBox(_T("计算成功!"));

	HighlightGridProperty(&m_propertyDataList, true);

	// 填充属性数据
	bool ret = PropertyDataUpdater::ReadDataFromGE( &m_propertyDataList, m_objId );

	// 填充数据失败
	if( !ret )
	{
		m_propertyDataList.EnableWindow( FALSE );
		m_propertyDataList.ShowWindow( SW_HIDE );
	}

	HighlightGridProperty(&m_propertyDataList, m_objId);
}

void WcyPropertyDlg::OnTimer( UINT_PTR nIDEvent )
{
	CDialog::OnTimer(nIDEvent);
	if (nIDEvent == 100)
	{
		KillTimer(nIDEvent);
		keybd_event(VK_RETURN,0,0,0);
		keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,0);//模拟"回车"命令
	}
}