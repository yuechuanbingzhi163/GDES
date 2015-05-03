#include "stdafx.h"
#include "IntListSetDlg.h"

#include "../MineGE/MineGE.h"
#include "../ArxHelper/HelperClass.h"

IMPLEMENT_DYNAMIC( IntListSetDlg, CDialog )

IntListSetDlg::IntListSetDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( IntListSetDlg::IDD, pParent )
    , m_intValue( _T( "" ) )
    , m_strValue( _T( "" ) )
{

}

IntListSetDlg::~IntListSetDlg()
{
}

void IntListSetDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_INT_LIST, m_list );
    DDX_Text( pDX, IDC_INT, m_intValue );
    DDX_Text( pDX, IDC_INT_STR, m_strValue );
}


BEGIN_MESSAGE_MAP( IntListSetDlg, CDialog )
    ON_BN_CLICKED( IDC_INT_STR_ADD, &IntListSetDlg::OnBnClickedIntStrAdd )
    ON_BN_CLICKED( IDC_INT_STR_DEL, &IntListSetDlg::OnBnClickedIntStrDel )
    ON_BN_CLICKED( IDOK, &IntListSetDlg::OnBnClickedOk )
END_MESSAGE_MAP()


// IntListSetDlg 消息处理程序

BOOL IntListSetDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_list.SetExtendedStyle( m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
    CRect rect;
    m_list.GetClientRect( &rect );
    double w = rect.Width() * 0.5;
    m_list.InsertColumn( 0, _T( "整数" ), LVCFMT_LEFT, w );
    m_list.InsertColumn( 1, _T( "字符串" ), LVCFMT_LEFT, w );

    int len = m_intList.length();
    for ( int i = 0; i < len; i++ )
    {
        CString str;
        str.Format( _T( "%d" ), m_intList[i] );
        m_list.InsertItem( i, str );
        m_list.SetItemData( i, i );
        m_list.SetItemText( i, 1, m_intStrList[i].kACharPtr() );
    }
    if( len > 0 ) m_list.SetHotItem( 0 );

    GetDlgItem( IDC_INT )->SetFocus();

    return TRUE;
}

static bool IsStringEmpty( const CString& str )
{
    return ( str.GetLength() == 0 );
}

void IntListSetDlg::OnBnClickedIntStrAdd()
{
    UpdateData( TRUE );
    if( IsStringEmpty( m_intValue ) )
    {
        MessageBox( _T( "请输入一个整数" ) );
        getEdit( IDC_INT )->SetFocus();
        getEdit( IDC_INT )->SetSel( 0, -1 );
        return;
    }
    else
    {
        if( !ArxUtilHelper::IsInteger( m_intValue ) )
        {
            MessageBox( _T( "非法的整数\n格式：[+/-][0-9]\n例如10, +20, -1等" ) );
            getEdit( IDC_INT )->SetFocus();
            getEdit( IDC_INT )->SetSel( 0, -1 );
            return;
        }
    }

    if( IsStringEmpty( m_strValue ) )
    {
        MessageBox( _T( "字符串不能为空" ) );
        getEdit( IDC_INT_STR )->SetFocus();
        getEdit( IDC_INT_STR )->SetSel( 0, -1 );
        return;
    }

    if( isValueExist( m_intValue, true ) )
    {
        CString msg;
        msg.Format( _T( "整数: %s 已存在于列表中!" ), m_intValue );
        MessageBox( msg );
        getEdit( IDC_INT )->SetFocus();
        getEdit( IDC_INT )->SetSel( 0, -1 );
        return;
    }

    if( isValueExist( m_strValue, false ) )
    {
        CString msg;
        msg.Format( _T( "字符串: %s 已存在于列表中!" ), m_strValue );
        MessageBox( msg );
        getEdit( IDC_INT_STR )->SetFocus();
        getEdit( IDC_INT_STR )->SetSel( 0, -1 );
        return;
    }

    // 增加一项
    int index = m_list.GetItemCount();
    m_list.InsertItem( index, m_intValue );
    m_list.SetItemData( index, index );
    m_list.SetItemText( index, 1, m_strValue );
    getEdit( IDC_INT )->SetFocus();
    getEdit( IDC_INT )->SetSel( 0, -1 );
}

void IntListSetDlg::OnBnClickedIntStrDel()
{
    int sel = m_list.GetHotItem();
    if( sel != -1 )
    {
        m_list.DeleteItem( sel );
    }
}

CEdit* IntListSetDlg::getEdit( long id )
{
    return ( CEdit* )GetDlgItem( id );
}

bool IntListSetDlg::isValueExist( const CString& value, bool isIntType )
{
    bool ret = false;
    int nCount = m_list.GetItemCount();
    for ( int i = 0; i < nCount; i++ )
    {
        int subItem = 0;
        if( !isIntType ) subItem = 1;
        CString str = m_list.GetItemText( i, subItem );

        if( str.CompareNoCase( value ) == 0 )
        {
            ret = true;
            break;
        }
    }
    return ret;
}

void IntListSetDlg::OnBnClickedOk()
{
    // 可有可无???
    UpdateData( TRUE );

    m_intList.removeAll();
    m_intStrList.removeAll();

    int nCount = m_list.GetItemCount();
    for( int i = 0; i < nCount; i++ )
    {
        CString intValue = m_list.GetItemText( i, 0 );
        CString strValue = m_list.GetItemText( i, 1 );
        m_intList.append( _ttoi( intValue ) );
        m_intStrList.append( strValue );
    }
    MessageBox( _T( "保存成功!" ) );
    OnOK();  // 关闭对话框
}
