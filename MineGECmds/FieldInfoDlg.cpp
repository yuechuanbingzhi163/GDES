#include "stdafx.h"
#include "FieldInfoDlg.h"

#include "../MineGE/DataListHelper.h"

#include "../ArxHelper/HelperClass.h"

IMPLEMENT_DYNAMIC( FieldInfoDlg, CDialog )

FieldInfoDlg::FieldInfoDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( FieldInfoDlg::IDD, pParent )
    , m_dt( 0 )
    , m_unEnable( FALSE )
    , m_dMinValue( 0 )
    , m_dMaxValue( 0 )
    , m_nMinValue( 0 )
    , m_nMaxValue( 0 )
    , m_descr( _T( "" ) )
    , m_lt( 0 )
    , m_showInTooltip( FALSE )
{

}

FieldInfoDlg::~FieldInfoDlg()
{
}

void FieldInfoDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Radio( pDX, IDC_RADIO1, m_dt );
    DDX_Text( pDX, IDC_MIN_VALUE, m_dMinValue );
    DDX_Text( pDX, IDC_MAX_VALUE, m_dMaxValue );
    DDX_Check( pDX, IDC_UNENABLE, m_unEnable );
    DDX_Text( pDX, IDC_MIN_VALUE2, m_nMinValue );
    DDX_Text( pDX, IDC_MAX_VALUE2, m_nMaxValue );
    DDX_Text( pDX, IDC_DESCR, m_descr );
    DDX_Radio( pDX, IDC_RADIO7, m_lt );
    DDX_Control( pDX, IDC_VAR_NAME_LIST, m_varList );
    DDX_Check( pDX, IDC_SHOW_IN_TOOLTIP, m_showInTooltip );
}


BEGIN_MESSAGE_MAP( FieldInfoDlg, CDialog )
    ON_BN_CLICKED( IDC_RADIO1, &FieldInfoDlg::OnBnClickedRadio1 )
    ON_BN_CLICKED( IDC_RADIO2, &FieldInfoDlg::OnBnClickedRadio2 )
    ON_BN_CLICKED( IDC_RADIO3, &FieldInfoDlg::OnBnClickedRadio3 )
    ON_BN_CLICKED( IDC_RADIO4, &FieldInfoDlg::OnBnClickedRadio4 )
    ON_BN_CLICKED( IDC_SET_DEF, &FieldInfoDlg::OnBnClickedSetDef )
    ON_BN_CLICKED( IDC_RADIO5, &FieldInfoDlg::OnBnClickedRadio5 )
    ON_BN_CLICKED( IDC_RADIO7, &FieldInfoDlg::OnBnClickedRadio7 )
    ON_BN_CLICKED( IDC_RADIO8, &FieldInfoDlg::OnBnClickedRadio8 )
    ON_BN_CLICKED( IDC_RADIO9, &FieldInfoDlg::OnBnClickedRadio9 )
    ON_BN_CLICKED( IDC_DISPLAY, &FieldInfoDlg::OnBnClickedDisplay )
    ON_CBN_SELCHANGE( IDC_VAR_NAME_LIST, &FieldInfoDlg::OnCbnSelchangeVarNameList )
    ON_BN_CLICKED( IDC_RADIO6, &FieldInfoDlg::OnBnClickedRadio6 )
END_MESSAGE_MAP()


// FieldInfoDlg 消息处理程序
BOOL FieldInfoDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void FieldInfoDlg::OnBnClickedRadio1()
{
    UpdateData( TRUE );
    displayControls();
}

void FieldInfoDlg::OnBnClickedRadio2()
{
    UpdateData( TRUE );
    displayControls();
}

void FieldInfoDlg::OnBnClickedRadio3()
{
    UpdateData( TRUE );
    displayControls();
}

void FieldInfoDlg::OnBnClickedRadio4()
{
    UpdateData( TRUE );
    displayControls();
}

void FieldInfoDlg::OnBnClickedRadio5()
{
    UpdateData( TRUE );
    displayControls();
}

void FieldInfoDlg::OnBnClickedRadio6()
{
    UpdateData( TRUE );
    displayControls();
    fillVarList();
}

void FieldInfoDlg::OnBnClickedSetDef()
{
    FieldInfo info;
    readFromFieldInfo( info );
    UpdateData( FALSE ); // 更新到界面
}

void FieldInfoDlg::OnBnClickedRadio7()
{
    UpdateData( TRUE );
    fillVarList();
}

void FieldInfoDlg::OnBnClickedRadio8()
{
    UpdateData( TRUE );
    fillVarList();
}

void FieldInfoDlg::OnBnClickedRadio9()
{
    UpdateData( TRUE );
    fillVarList();
}

void FieldInfoDlg::OnCbnSelchangeVarNameList()
{
    m_varName = getSelVarName();
}

// 默认列数
#define DEF_COL_COUNT 5
// 字符串列表中的元素最多个数
// 如果超过了限制，需要分列显示
#define LIST_ITEM_MAX_COUNT 10
static void FormatStringListMsg( const AcStringArray& strList, const CString& name, CString& msg )
{
    msg.Format( _T( "【%s】数据列表:\n" ), name );
    int len = strList.length();
    if( len <= LIST_ITEM_MAX_COUNT )
    {
        for( int i = 0; i < len; i++ )
        {
            msg.AppendFormat( _T( "%s\n" ), strList[i].kACharPtr() );
        }
    }
    else
    {
        for( int i = 0; i < len; i++ )
        {
            msg.AppendFormat( _T( "%s\t" ), strList[i].kACharPtr() );
            if( ( i + 1 ) % DEF_COL_COUNT == 0 )
            {
                msg.Append( _T( "\n" ) );
            }
        }
    }
}

static void FormatIntListMsg( const AcDbIntArray& intList, const AcStringArray& strList, const CString& name, CString& msg )
{
    msg.Format( _T( "【%s】数据列表:\n" ), name );

    int len = intList.length();
    for( int i = 0; i < len; i++ )
    {
        msg.AppendFormat( _T( "%d\t%s\n" ), intList[i], strList[i].kACharPtr() );
    }
}

void FieldInfoDlg::OnBnClickedDisplay()
{
    if( m_varName.GetLength() == 0 )
    {
        MessageBox( _T( "请在数据列表中选择一项!" ) );
        return;
    }

    CString msg;
    AcStringArray strList;
    switch( m_lt )
    {
    case LT_STRING:
    {
        StringListHelper::GetStringList( m_varName, strList );
        FormatStringListMsg( strList, m_varName, msg );
    }
    break;

    case LT_INT:
    {
        AcDbIntArray intList;
        IntStrListHelper::GetIntStrList( m_varName, intList, strList );
        FormatIntListMsg( intList, strList, m_varName, msg );
    }
    break;

    case LT_OBJECT:
    {
        DataObjectListHelper::GetObjectNameList( m_varName, strList );
        FormatStringListMsg( strList, m_varName, msg );
    }
    break;
    }
    MessageBox( msg ); // 弹出消息对话框，显示数据列表
}

void FieldInfoDlg::displayControls()
{
    GetDlgItem( IDC_MIN_VALUE )->EnableWindow( FALSE );
    GetDlgItem( IDC_MAX_VALUE )->EnableWindow( FALSE );
    GetDlgItem( IDC_MIN_VALUE2 )->EnableWindow( FALSE );
    GetDlgItem( IDC_MAX_VALUE2 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RADIO7 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RADIO8 )->EnableWindow( FALSE );
    GetDlgItem( IDC_RADIO9 )->EnableWindow( FALSE );
    GetDlgItem( IDC_VAR_NAME_LIST )->EnableWindow( FALSE );
    GetDlgItem( IDC_DISPLAY )->EnableWindow( FALSE );

    switch( m_dt )
    {
    case DT_STRING:
    case DT_BOOL:
    case DT_DATE:
        break;
    case DT_INT:
        GetDlgItem( IDC_MIN_VALUE2 )->EnableWindow( TRUE );
        GetDlgItem( IDC_MAX_VALUE2 )->EnableWindow( TRUE );
        break;
    case DT_NUMERIC:
        GetDlgItem( IDC_MIN_VALUE )->EnableWindow( TRUE );
        GetDlgItem( IDC_MAX_VALUE )->EnableWindow( TRUE );
        break;
    case DT_LIST:
        GetDlgItem( IDC_RADIO7 )->EnableWindow( TRUE );
        GetDlgItem( IDC_RADIO8 )->EnableWindow( TRUE );
        GetDlgItem( IDC_RADIO9 )->EnableWindow( TRUE );
        GetDlgItem( IDC_VAR_NAME_LIST )->EnableWindow( TRUE );
        GetDlgItem( IDC_DISPLAY )->EnableWindow( TRUE );
        break;
    }
}

void FieldInfoDlg::readFromFieldInfo( const FieldInfo& info )
{
    m_dt          = info.m_dt;
    m_nMinValue   = info.m_minValue2;
    m_nMaxValue   = info.m_maxValue2;
    m_dMinValue   = info.m_minValue;
    m_dMaxValue   = info.m_maxValue;
    m_lt          = info.m_lt;
    m_varName     = info.m_varName;
    m_unEnable    = info.m_enable ? FALSE : TRUE;
    m_descr       = info.m_descr;
    m_descr.Trim();            // 去掉空白
    m_showInTooltip = info.m_showInTooltip; // false->FALSE(0)

    displayControls();         // 屏蔽部分控件

    m_varList.ResetContent();  // 清空列表
    if( m_dt == DT_LIST )
    {
        fillVarList();         // 填充列表
    }
}

void FieldInfoDlg::writeToFieldInfo( FieldInfo& info )
{
    info.m_dt = ( DATA_TYPE )m_dt;
    info.m_minValue2 = m_nMinValue;
    info.m_maxValue2 = m_nMaxValue;
    info.m_minValue = m_dMinValue;
    info.m_maxValue = m_dMaxValue;
    info.m_lt = ( LIST_TYPE )m_lt;
    info.m_varName = m_varName;
    info.m_enable = m_unEnable ? false : true;
    info.m_descr = m_descr.Trim();
    info.m_showInTooltip = m_showInTooltip;

    // 修正属性设置
    // 不正确的属性全部"清零"
    info.revise();
}

bool FieldInfoDlg::validDataformat( const CString& str, bool isIntType )
{
    if( isIntType )
    {
        if( !ArxUtilHelper::IsInteger( str ) )
        {
            MessageBox( _T( "非法的整数!\n格式:[+/-][0-9]\n例如+1024 -20, 108, 001" ) );
            return false;
        }
    }
    else
    {
        if( !ArxUtilHelper::IsNumeric( str ) )
        {
            MessageBox( _T( "非法的浮点数!\n格式:[+/-][0-9].[0-9]\n例如+10.24, -20, 10.78, 0.0098" ) );
            return false;
        }
    }
    return true;
}

bool FieldInfoDlg::validMinMaxValue( long id, bool isIntType )
{
    CEdit* pEdit = ( CEdit* )GetDlgItem( id );
    CString str;
    pEdit->GetWindowText( str );
    if( !validDataformat( str, isIntType ) )
    {
        pEdit->SetFocus();
        pEdit->SetSel( 0, -1 );
        return false;
    }
    return true;
}

bool FieldInfoDlg::compareMinMaxValue( long id1, long id2, bool isIntType )
{
    CString str1;
    CEdit* pEdit1 = ( CEdit* )GetDlgItem( id1 );
    pEdit1->GetWindowText( str1 );

    CString str2;
    CEdit* pEdit2 = ( CEdit* )GetDlgItem( id2 );
    pEdit2->GetWindowText( str2 );

    bool ret;
    if( isIntType )
    {
        int minValue, maxValue;
        ArxUtilHelper::StringToInt( str1, minValue );
        ArxUtilHelper::StringToInt( str2, maxValue );
        ret = ( minValue <= maxValue );
    }
    else
    {
        double minValue, maxValue;
        ArxUtilHelper::StringToDouble( str1, minValue );
        ArxUtilHelper::StringToDouble( str2, maxValue );
        ret = ( minValue <= maxValue );
    }
    if( !ret )
    {
        pEdit1->SetFocus();
        pEdit1->SetSel( 0, -1 );
    }

    return ret;
}

bool FieldInfoDlg::checkEmptyValue( long id )
{
    CString str;
    CEdit* pEdit = ( CEdit* )GetDlgItem( id );
    pEdit->GetWindowText( str );
    if( str.GetLength() == 0 )
    {
        MessageBox( _T( "请输入数据" ) );
        return false;
    }
    else
    {
        return true;
    }
}

bool FieldInfoDlg::validateFieldInfo()
{
    if( m_dt == DT_INT )
    {
        if( !checkEmptyValue( IDC_MIN_VALUE2 ) ) return false;
        if( !checkEmptyValue( IDC_MAX_VALUE2 ) ) return false;
        if( !validMinMaxValue( IDC_MIN_VALUE2, true ) ) return false;
        if( !validMinMaxValue( IDC_MAX_VALUE2, true ) ) return false;
        if( !compareMinMaxValue( IDC_MIN_VALUE2, IDC_MAX_VALUE2, true ) )
        {
            MessageBox( _T( "错误!应保证最大值>=最小值" ) );
            return false;
        }
    }
    else if( m_dt == DT_NUMERIC )
    {
        if( !checkEmptyValue( IDC_MIN_VALUE ) ) return false;
        if( !checkEmptyValue( IDC_MAX_VALUE ) ) return false;
        if( !validMinMaxValue( IDC_MIN_VALUE, false ) ) return false;
        if( !validMinMaxValue( IDC_MAX_VALUE, false ) ) return false;
        if( !compareMinMaxValue( IDC_MIN_VALUE, IDC_MAX_VALUE, false ) )
        {
            MessageBox( _T( "错误!应保证最大值>=最小值" ) );
            return false;
        }
    }
    else if( m_dt == DT_LIST )
    {
        if( m_varList.GetCount() == 0 )
        {
            MessageBox( _T( "数据列表中没有可供选择项!" ) );
            return false;
        }

        if( m_varName.GetLength() == 0 || m_varList.GetCurSel() == CB_ERR )
        {
            MessageBox( _T( "请在列表中选择一个数据名称!" ) );
            return false;
        }
    }
    return true;
}

void FieldInfoDlg::fillVarList()
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

    if( names.isEmpty() ) return;

    int index = names.find( m_varName );
    if( index < 0 ) 
	{
		m_varName = _T( "" ); // 清空
		//index = 0; //设置默认是第一项
	}

    int len = names.length();
    for( int i = 0; i < len; i++ )
    {
        m_varList.AddString( names[i].kACharPtr() );
    }
    m_varList.SetCurSel( index );
}

CString FieldInfoDlg::getSelVarName()
{
    CString varName;
    int sel = m_varList.GetCurSel();
    if( CB_ERR != sel )
    {
        m_varList.GetLBText( sel, varName );
    }
    return varName;
}
