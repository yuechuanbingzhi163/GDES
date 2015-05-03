#include "stdafx.h"
#include "DataObjectStateDlg.h"

#include "PropertyDataDlgHelper.h"

#include "../MineGE/DataListHelper.h"
#include "../MineGE/DataHelper.h"

IMPLEMENT_DYNAMIC( DataObjectStateDlg, CDialog )

DataObjectStateDlg::DataObjectStateDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( DataObjectStateDlg::IDD, pParent )
{

}

DataObjectStateDlg::~DataObjectStateDlg()
{
}

void DataObjectStateDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_VALUE_LIST, m_valueList );
    DDX_Control( pDX, IDC_TYPE_NAME_LIST, m_typeList );
    DDX_Control( pDX, IDC_TYPE_NAME_LIST2, m_typeList2 );
}

BEGIN_MESSAGE_MAP( DataObjectStateDlg, CDialog )
    ON_BN_CLICKED( IDC_SHOW_BTN, &DataObjectStateDlg::OnBnClickedShowBtn )
    ON_CBN_SELCHANGE( IDC_TYPE_NAME_LIST, &DataObjectStateDlg::OnCbnSelchangeTypeList )
    ON_BN_CLICKED( IDC_SHOW_BTN2, &DataObjectStateDlg::OnBnClickedShowBtn2 )
END_MESSAGE_MAP()


BOOL DataObjectStateDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    updateTypeList();
    updateTypeList2();
    updateValueList();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void DataObjectStateDlg::OnCbnSelchangeTypeList()
{
    updateValueList();
}

void DataObjectStateDlg::OnBnClickedShowBtn()
{
    int sel = m_typeList.GetCurSel();
    if( sel == CB_ERR ) return;

    CString type;
    m_typeList.GetLBText( sel, type );

    sel = m_valueList.GetCurSel();
    if( sel == CB_ERR ) return;

    CString value;
    m_valueList.GetLBText( sel, value );

    AcDbObjectId objId;
    DataObjectListHelper::GetObjectId( type, value, objId );

    PropertyDataDlgHelper::DisplayAllPropertyDataDlg( objId );
}

void DataObjectStateDlg::OnBnClickedShowBtn2()
{
    int sel = m_typeList2.GetCurSel();
    if( sel == CB_ERR ) return;

    CString type;
    m_typeList2.GetLBText( sel, type );

    AcDbObjectId objId;
    SingleDataObjectHelper::GetObjectId( type, objId );

    PropertyDataDlgHelper::DisplayAllPropertyDataDlg( objId );
}


void DataObjectStateDlg::updateTypeList()
{
    m_typeList.ResetContent();

    //acDocManager->lockDocument(curDoc());	    // 锁定文档
    AcStringArray types;
    DataObjectListHelper::GetAllNames( types );
    //acDocManager->unlockDocument(curDoc()); 	// 解锁文档
    if( types.isEmpty() ) return;

    int len = types.length();
    for( int i = 0; i < len; i++ )
    {
        m_typeList.AddString( types[i].kACharPtr() );
    }
    m_typeList.SetCurSel( 0 );
}

void DataObjectStateDlg::updateTypeList2()
{
    m_typeList2.ResetContent();

    //acDocManager->lockDocument(curDoc());	    // 锁定文档
    AcStringArray types;
    SingleDataObjectHelper::GetAllNames( types );
    //acDocManager->unlockDocument(curDoc()); 	// 解锁文档
    if( types.isEmpty() ) return;

    int len = types.length();
    for( int i = 0; i < len; i++ )
    {
        m_typeList2.AddString( types[i].kACharPtr() );
    }
    m_typeList2.SetCurSel( 0 );
}

void DataObjectStateDlg::updateValueList()
{
    m_valueList.ResetContent();

    int sel = m_typeList.GetCurSel();
    if( sel == CB_ERR ) return;

    CString type;
    m_typeList.GetLBText( sel, type );

    //acDocManager->lockDocument(curDoc());	    // 锁定文档
    AcStringArray values;
    bool ret = DataObjectListHelper::GetObjectNameList( type, values );
    //acDocManager->unlockDocument(curDoc()); 	// 解锁文档
    if( !ret || values.isEmpty() ) return;

    int len = values.length();
    for( int i = 0; i < len; i++ )
    {
        m_valueList.AddString( values[i].kACharPtr() );
    }
    m_valueList.SetCurSel( 0 );
}
