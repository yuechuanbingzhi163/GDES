#include "stdafx.h"
#include "PropertyDataDlg2.h"

#include "../MineGE/HelperClass.h"

IMPLEMENT_DYNAMIC( PropertyDataDlg2, CDialog )

PropertyDataDlg2::PropertyDataDlg2( CWnd* pParent /*=NULL*/ )
    : CDialog( PropertyDataDlg2::IDD, pParent )
{

}

PropertyDataDlg2::~PropertyDataDlg2()
{
}

void PropertyDataDlg2::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_PROP_LIST, m_propertyDataList );
}


BEGIN_MESSAGE_MAP( PropertyDataDlg2, CDialog )
    ON_BN_CLICKED( IDOK, &PropertyDataDlg2::OnBnClickedOk )
END_MESSAGE_MAP()


// PropertyDataDlg2 消息处理程序

void PropertyDataDlg2::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    OnOK();
}

BOOL PropertyDataDlg2::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_propertyDataList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

    // 获取类型名称
    CString type;
    DataHelper::GetTypeName( m_objId, type );

    // 获取字段
    AcStringArray fileds;
    if( m_showAll )
    {
        FieldHelper::GetAllFields( type, fileds );
    }
    else
    {
        fileds.append( m_fields );
    }

    // 创建属性数据控件
    PropertyDataUpdater::BuildPropGridCtrl( &m_propertyDataList, type, fileds );

    // 填充属性数据
    bool ret = PropertyDataUpdater::ReadDataFromGE( &m_propertyDataList, m_objId );

    // 填充数据失败
    if( !ret )
    {
        m_propertyDataList.EnableWindow( FALSE );
        m_propertyDataList.ShowWindow( SW_HIDE );
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void PropertyDataDlg2::setMineGE( const AcDbObjectId& objId )
{
    m_objId = objId;
}

void PropertyDataDlg2::addField( const CString& field )
{
    m_fields.append( field );
}

void PropertyDataDlg2::showAllData( bool bFlag )
{
    m_showAll = bFlag;
}