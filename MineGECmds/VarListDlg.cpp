#include "stdafx.h"

#include "VarListDlg.h"
#include "IntListSetDlg.h"
#include "StrListSetDlg.h"
#include "ObjListSetDlg.h"
#include "InputDlg.h"

#include "../MineGE/MineGE.h"
#include "../MineGE/FieldType.h"
#include "../MineGE/FieldHelper.h"
#include "../MineGE/DataListHelper.h"

IMPLEMENT_DYNAMIC( VarListDlg, CDialog )

VarListDlg::VarListDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( VarListDlg::IDD, pParent )
    , m_lt( 0 )
{

}

VarListDlg::~VarListDlg()
{
}

void VarListDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Radio( pDX, IDC_RADIO1, m_lt );
    DDX_Control( pDX, IDC_VAR_NAME_LIST, m_varList );
}


BEGIN_MESSAGE_MAP( VarListDlg, CDialog )
    ON_BN_CLICKED( IDC_SET_VAR, &VarListDlg::OnBnClickedSetVar )
    ON_BN_CLICKED( IDC_ADD_VAR, &VarListDlg::OnBnClickedAddVar )
    ON_BN_CLICKED( IDC_DEL_VAR, &VarListDlg::OnBnClickedDelVar )
    ON_BN_CLICKED( IDC_RADIO1, &VarListDlg::OnBnClickedRadio1 )
    ON_BN_CLICKED( IDC_RADIO2, &VarListDlg::OnBnClickedRadio2 )
    ON_BN_CLICKED( IDC_RADIO3, &VarListDlg::OnBnClickedRadio3 )
END_MESSAGE_MAP()


static bool IsStringEmpty( const CString& str )
{
    return ( str.GetLength() == 0 );
}

// VarListDlg 消息处理程序
void VarListDlg::OnBnClickedRadio1()
{
    UpdateData( TRUE );
    fillVarList();
}

void VarListDlg::OnBnClickedRadio2()
{
    UpdateData( TRUE );
    fillVarList();
}

void VarListDlg::OnBnClickedRadio3()
{
    UpdateData( TRUE );
    fillVarList();
}

void VarListDlg::OnBnClickedSetVar()
{
    CString varName = getSelVarName();
    if( varName.GetLength() == 0 )
    {
        MessageBox( _T( "请在数据列表中选择一项!" ) );
        return;
    }

    if( m_lt == LT_STRING )
    {
        StrListSetDlg slsd;
        StringListHelper::GetStringList( varName, slsd.m_strList );
        if( IDOK == slsd.DoModal() )
        {
            // 清空所有数据
            StringListHelper::RemoveStringList( varName );
            if( !slsd.m_strList.isEmpty() )
            {
                // 增加一个新的链表数据
                StringListHelper::AddStringList( varName, slsd.m_strList );
            }
        }
    }
    else if( m_lt == LT_INT )
    {
        IntListSetDlg ilsd;
        IntStrListHelper::GetIntStrList( varName, ilsd.m_intList, ilsd.m_intStrList );
        if( IDOK == ilsd.DoModal() )
        {
            // 清空所有数据
            IntStrListHelper::RemoveIntStrList( varName );
            if( !ilsd.m_intValue.IsEmpty() )
            {
                // 增加一个新的链表数据
                IntStrListHelper::AddIntStrList( varName, ilsd.m_intList, ilsd.m_intStrList );
            }
        }
    }
    else if( m_lt == LT_OBJECT )
    {
        AcStringArray values;
        DataObjectListHelper::GetObjectNameList( varName, values );

        ObjListSetDlg olsd;
        olsd.m_strList.append( values );
        if( IDOK == olsd.DoModal() )
        {
            AcStringArray& leftValues = olsd.m_strList;
            if( leftValues.isEmpty() )
            {
                DataObjectListHelper::RemoveAllObject( varName );
            }
            else
            {
                int len = leftValues.length();
                for( int i = 0; i < len; i++ )
                {
                    CString value = leftValues[i].kACharPtr();
                    int index = values.find( value );
                    if( index != -1 ) // 已存在，不变
                    {
                        values.removeAt( index );
                    }
                    else
                    {
                        // 增加数据对象
                        DataObjectListHelper::AddObject( varName, value );
                    }
                }

                // 删除多余的数据对象
                len = values.length();
                for( int i = 0; i < len; i++ )
                {
                    DataObjectListHelper::RemoveObject( varName, values[i].kACharPtr() );
                }
            }
        }
    }
}

void VarListDlg::OnBnClickedAddVar()
{
    InputDlg dlg;
    if( IDOK == dlg.DoModal() )
    {
        if( IsStringEmpty( dlg.m_str ) )
        {
            MessageBox( _T( "变量名称不允许为空" ) );
        }
        else if( isFieldExistInListBox( dlg.m_str ) )
        {
            CString msg;
            msg.Format( _T( "变量%s已存在于列表中!" ), dlg.m_str );
            MessageBox( msg );
        }
        else
        {
            // 选中添加的最后一项
            int index = m_varList.AddString( dlg.m_str );
            if( index != CB_ERR ) m_varList.SetCurSel( index );
        }
    }
}

void VarListDlg::OnBnClickedDelVar()
{
    int sel = m_varList.GetCurSel();
    if( sel == CB_ERR )
    {
        MessageBox( _T( "请在列表中选择一项" ) );
    }
    else
    {
        CString varName;
        m_varList.GetLBText( sel, varName );
        m_varList.DeleteString( sel );
        if( m_lt == LT_STRING )
        {
            StringListHelper::RemoveStringList( varName );
        }
        else if( m_lt == LT_INT )
        {
            IntStrListHelper::RemoveIntStrList( varName );
        }
        else if( m_lt == LT_OBJECT )
        {
            // 删除数据
            DataObjectListHelper::RemoveAllObject( varName );

            // 注销类型
            FieldHelper::RemoveAllFields( varName );
        }
    }
}

BOOL VarListDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_lt = LT_STRING;
    fillVarList();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void VarListDlg::fillVarList()
{
    m_varList.ResetContent();   // 清空列表

    AcStringArray names;
    switch( m_lt )
    {
    case LT_STRING:
        StringListHelper::GetAllNames( names );
        break;

    case LT_INT:
        IntStrListHelper::GetAllNames( names );
        break;

    case LT_OBJECT:
        DataObjectListHelper::GetAllNames( names );
        break;
    }

    int len = names.length();
    for( int i = 0; i < len; i++ )
    {
        m_varList.AddString( names[i].kACharPtr() );
    }
    if( len > 0 )
    {
        m_varList.SetCurSel( 0 );
    }
}

CString VarListDlg::getSelVarName()
{
    CString varName;
    int sel = m_varList.GetCurSel();
    if( CB_ERR != sel )
    {
        m_varList.GetLBText( sel, varName );
    }
    return varName;
}

bool VarListDlg::isFieldExistInListBox( const CString& varName )
{
    return ( m_varList.FindString( 0, varName ) != CB_ERR );
}
