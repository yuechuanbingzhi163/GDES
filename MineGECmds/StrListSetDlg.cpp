#include "stdafx.h"

#include "StrListSetDlg.h"
#include "InputDlg.h"

#include "../MineGE/MineGE.h"

IMPLEMENT_DYNAMIC( StrListSetDlg, CDialog )

StrListSetDlg::StrListSetDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( StrListSetDlg::IDD, pParent )
{

}

StrListSetDlg::~StrListSetDlg()
{
}

void StrListSetDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_STR_LIST, m_strListBox );
}


BEGIN_MESSAGE_MAP( StrListSetDlg, CDialog )
    ON_BN_CLICKED( IDC_STR_ADD, &StrListSetDlg::OnBnClickedStrAdd )
    ON_BN_CLICKED( IDC_STR_DEL, &StrListSetDlg::OnBnClickedStrDel )
    ON_BN_CLICKED( IDOK, &StrListSetDlg::OnBnClickedOk )
END_MESSAGE_MAP()

static bool IsStringEmpty( const CString& str )
{
    return ( str.GetLength() == 0 );
}

bool StrListSetDlg::isStrExistInListBox( const CString& str )
{
    return ( m_strListBox.FindString( 0, str ) != LB_ERR );
}

void StrListSetDlg::OnBnClickedStrAdd()
{
    InputDlg dlg;
    if( IDOK != dlg.DoModal() ) return;

    if( IsStringEmpty( dlg.m_str ) )
    {
        MessageBox( _T( "字符串不允许为空" ) );
    }
    else if( isStrExistInListBox( dlg.m_str ) )
    {
        CString msg;
        msg.Format( _T( "字符串\"%s\"已存在" ), dlg.m_str );
        MessageBox( msg );
    }
    else
    {
        m_strListBox.AddString( dlg.m_str );
    }
}

void StrListSetDlg::OnBnClickedStrDel()
{
    int sel = m_strListBox.GetCurSel();
    if( sel != LB_ERR )
    {
        m_strListBox.DeleteString( sel );
    }
}

void StrListSetDlg::OnBnClickedOk()
{
    m_strList.removeAll();
    int count = m_strListBox.GetCount();
    for( int i = 0; i < count; i++ )
    {
        CString text;
        m_strListBox.GetText( i, text );
        m_strList.append( text );
    }
    MessageBox( _T( "保存成功!" ) );
    OnOK(); // 关闭对话框
}

BOOL StrListSetDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    int len = m_strList.length();
    for( int i = 0; i < len; i++ )
    {
        m_strListBox.AddString( m_strList[i].kACharPtr() );
    }
    if( len > 0 )
    {
        m_strListBox.SetCurSel( 0 );
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
