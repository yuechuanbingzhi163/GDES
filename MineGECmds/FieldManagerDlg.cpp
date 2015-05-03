#include "stdafx.h"
#include "FieldManagerDlg.h"

#include "InputDlg.h"

#include "../MineGE/FieldHelper.h"
#include "../MineGE/FieldInfoHelper.h"
//#include "../MineGE/TypeNameMapHelper.h"

#include "../ArxHelper/HelperClass.h"

IMPLEMENT_DYNAMIC( FieldManagerDlg, CDialog )

FieldManagerDlg::FieldManagerDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( FieldManagerDlg::IDD, pParent )
{

}

FieldManagerDlg::~FieldManagerDlg()
{
}

void FieldManagerDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_TYPE_COMBO2, m_typeCombo );
    DDX_Control( pDX, IDC_FIELD_LIST, m_fieldListBox );
}

BEGIN_MESSAGE_MAP( FieldManagerDlg, CDialog )
    ON_BN_CLICKED( IDC_ADD_BTN, &FieldManagerDlg::OnBnClickedAddBtn )
    ON_BN_CLICKED( IDC_DEL_BTN, &FieldManagerDlg::OnBnClickedDelBtn )
    ON_CBN_SELCHANGE( IDC_TYPE_COMBO2, &FieldManagerDlg::OnCbnSelchangeTypeCombo2 )
    ON_BN_CLICKED( IDC_APPLY_BTN, &FieldManagerDlg::OnBnClickedApplyBtn )
    ON_LBN_SELCHANGE( IDC_FIELD_LIST, &FieldManagerDlg::OnLbnSelchangeFieldList )
    ON_WM_CLOSE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL FieldManagerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

	m_namesEn.RemoveAll();
	m_namesZh.RemoveAll();

    // 显示字段信息子对话框
    CRect rect;
    GetDlgItem( IDC_FIELD_INFO_POS )->GetWindowRect( &rect );
    ScreenToClient( &rect );
    fidlg.Create( IDD_FIELD_INFO_DLG, this );
    fidlg.MoveWindow( &rect );
    fidlg.ShowWindow( SW_SHOW );

    fillContent(); // 填充数据

    return TRUE;
}

#include <fstream>
// 需要包含<fstream>才能使用
#include "AcFStream.h"

static CString GetAppPathDir()
{
	TCHAR szModulePath[_MAX_PATH];
	GetModuleFileName( _hdllInstance, szModulePath, _MAX_PATH );

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	_tsplitpath( szModulePath, drive, dir, NULL, NULL );

	TCHAR szPath[_MAX_PATH] = {0};
	_tmakepath( szPath, drive, dir, NULL, NULL );

	return CString( szPath );
}

static CString BuildPath( const CString& dir, const CString& fileName )
{
	CString path;
	path.Format( _T( "%s%s" ), dir, fileName );
	return path;
}

void FieldManagerDlg::classNameTrans()
{
	CString dataDirName = _T( "Datas\\" );
	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T( "瓦斯抽采-图元类名-翻译.txt" ) );
	
	AcIfstream inFile( fileName );
	if( !inFile ) 
	{
		acutPrintf(_T("\n文件打开失败"));
		return;
	}

	while( !inFile.eof() )
	{
		ACHAR enClassName[_MAX_PATH],zhClassName[_MAX_PATH];
		inFile >> enClassName >> zhClassName;
		if(inFile.fail()) break;

		CString nameEn,nameZh;
		nameEn.Format(_T("%s"),enClassName);
		nameZh.Format(_T("%s"),zhClassName);
		m_namesEn.Add(nameEn);
		m_namesZh.Add(nameZh);
		//acutPrintf(_T("\n%s\t%s"), strType, typeName);
	}
	inFile.close();

}
void FieldManagerDlg::OnClose()
{
    clearFieldInfoArray();
    CDialog::OnClose();
}

void FieldManagerDlg::OnBnClickedAddBtn()
{
    InputDlg dlg;
    if( dlg.DoModal() != IDOK ) return;

    // 注意m_lastSel的更新问题
    if( m_fieldListBox.GetCount() == 0 ) setLastSelIndex( 0 );

    if( !isValidField( dlg.m_str ) )
    {
        CString msg;
        msg.Format( _T( "非法的字段\n[%s]" ), dlg.m_str );
        MessageBox( msg );
        return;
    }
    if( isFieldExistInListBox( dlg.m_str ) )
    {
        CString msg;
        msg.Format( _T( "字段[%s]已存在!" ), dlg.m_str );
        MessageBox( msg );
        return;
    }

    // 添加字段到listbox中
    int index = m_fieldListBox.AddString( dlg.m_str );
    if( index != LB_ERR )
    {
        m_fieldListBox.SetCurSel( index );
        // ***创建新的字段信息，并记录***
        m_infoes.append( new FieldInfo() );

        // 切换selection, 保存上次selection所在位置的字段信息
        updateFieldInfo();
    }
}

void FieldManagerDlg::OnBnClickedDelBtn()
{
    if( m_fieldListBox.GetCount() == 0 ) return;

    if( IDCANCEL == MessageBox(
                _T( "删除字段的同时也会删除所有相应类型图元的字段和数据\n\n确定要删除字段吗？【请慎重操作!!!】" ),
                _T( "提示框" ),
                MB_ICONEXCLAMATION | MB_ICONWARNING | MB_OKCANCEL ) ) return;

    int index = m_fieldListBox.GetCurSel();
    if( index == LB_ERR ) return;

    m_fieldListBox.DeleteString( index );

    // ****同时删除对应的字段信息****
    delete m_infoes[index];
    m_infoes.removeAt( index );

    // 注意m_lastSel的更新问题
    if( m_fieldListBox.GetCount() == 0 ) setLastSelIndex( LB_ERR );
}

void FieldManagerDlg::OnCbnSelchangeTypeCombo2()
{
    // 不保存之前所做的修改***???
	//确实不保存2015/1/1,hd

	// 清空字段列表
    clearFieldListBox();

    // 填充字段
    fillFieldListBox( getSelType() );

    // 更新字段信息对话框的显示
    showFieldInfo();

    // Invalidate()将整个对话框都失效
    // 而InvalidateRect仅将IDC_DRAW_POS所在矩形块失效
    // 从而提高了效率
    CRect rect;
    GetDlgItem( IDC_DRAW_POS )->GetWindowRect( &rect );
    ScreenToClient( &rect );
    InvalidateRect( &rect );
    // 更新绘图区的显示
    // 不调用UpdateWindow()也能够触发WM_PAINT消息，从而调用OnPaint()方法
    //UpdateWindow();
}

void FieldManagerDlg::OnLbnSelchangeFieldList()
{
    // 切换之前更新上次选择的字段信息
    // 并检查字段信息的有效性
    updateFieldInfo();
}

/*
 * 方法：
 *	  1) 得到原始的字段链表L1和字段列表框中的字段链表L2
 *    2) 遍历链表L2,判断字段是否存在于L1中
 *       a) 如果存在，则该字段保持不变，并从L1中移除该字段；
 *       b) 如果不存在，则转第(3)步
 *    3) 该字段是新添加的字段，则执行"增加字段"操作Add
 *    4) 完成遍历L2，最后剩下的L1的元素就是需要删除的字段
 *    5) 执行"删除字段"操作Remove
 */
void FieldManagerDlg::OnBnClickedApplyBtn()
{
    if( m_fieldListBox.GetCount() > 0 )
    {
        // 切换之前更新上次选择的字段信息
        // 并检查字段信息的有效性
        if( !updateFieldInfo() ) return;
    }

    // 选择的图元类型
    CString selType = getSelType();
    if( selType.GetLength() == 0 ) return;

    AcStringArray fields;
    FieldHelper::GetAllFields( selType, fields );
    if( fields.isEmpty() && m_fieldListBox.GetCount() == 0 )
    {
        MessageBox( _T( "没有字段可更新" ) );
        return;
    }

    // "剩余"字段
    // 与m_infoes应该是一一对应的
    AcStringArray leftFields;
    for( int i = 0; i < m_fieldListBox.GetCount(); i++ )
    {
        CString text;
        m_fieldListBox.GetText( i, text );
        leftFields.append( text );
    }

    int len = leftFields.length();
    for( int i = 0; i < len; i++ )
    {
        CString field = leftFields[i].kACharPtr();
        int index = fields.find( field );
        if( index >= 0 ) // 已存在，不变
        {
            fields.removeAt( index );
        }
        else
        {
            // 增加字段
            FieldHelper::AddField( selType, field );
        }
        // 默认属性设置不需要添加到词典中
        if( m_infoes[i]->isDefault() )
        {
            FieldInfoHelper::RemoveFieldInfo( selType, field );
        }
        else
        {
            // 新增的字段
            if( index < 0 || !FieldInfoHelper::FindFieldInfo( selType, field ) )
            {
                FieldInfoHelper::AddFieldInfo( selType, field, *m_infoes[i] );
            }
            else
            {
                // 更新已有的字段信息
                FieldInfoHelper::WriteFieldInfo( selType, field, *m_infoes[i] );
            }
        }
    }

    // 删除字段
    len = fields.length();
    for( int i = 0; i < len; i++ )
    {
        FieldHelper::RemoveField( selType, fields[i].kACharPtr() );
    }

    MessageBox( _T( "字段信息更新成功!" ) );
}

bool FieldManagerDlg::updateFieldInfo()
{
    fidlg.UpdateData( TRUE );
    bool ret = fidlg.validateFieldInfo();
    if( ret )
    {
        if( getLastSelIndex() != LB_ERR )
        {
            fidlg.writeToFieldInfo( *m_infoes[getLastSelIndex()] );
        }
        showFieldInfo();
    }
    else
    {
        m_fieldListBox.SetCurSel( getLastSelIndex() );
    }
    return ret;
}

CString FieldManagerDlg::getSelType()
{
    CString selType;
    int sel = m_typeCombo.GetCurSel();
    if( sel != CB_ERR )
    {
        //m_typeCombo.GetLBText( sel, selType );
		selType = m_namesEn.GetAt(sel);
    }
    return selType;
}

CString FieldManagerDlg::getSelField()
{
    CString selField;
    int sel = m_fieldListBox.GetCurSel();
    if( sel != LB_ERR )
    {
        m_fieldListBox.GetText( sel, selField );
    }
    return selField;
}

void FieldManagerDlg::clearFieldInfoArray()
{
    // 清空字段信息(注意释放内存)
    int len = m_infoes.length();
    for( int i = 0; i < len; i++ )
    {
        delete m_infoes[i];
    }
    m_infoes.removeAll();
}

void FieldManagerDlg::clearFieldListBox()
{
    m_fieldListBox.ResetContent();

    // 清空字段信息
    clearFieldInfoArray();
}

void FieldManagerDlg::fillFieldListBox( const CString& type )
{
    setLastSelIndex( LB_ERR ); // 记录listbox切换之前的索引位置

    AcStringArray fields;
    FieldHelper::GetAllFields( type, fields );
    if( fields.isEmpty() ) return;
	
    int len = fields.length();
    for( int i = 0; i < len; i++ )
    {
        CString field = fields[i].kACharPtr();
        m_fieldListBox.AddString( field );

        // 初始为默认设置
        FieldInfo* pInfo = new FieldInfo();
        // 读取字段信息
        FieldInfoHelper::ReadFieldInfo( type, field, *pInfo );
        m_infoes.append( pInfo );
    }
    m_fieldListBox.SetCurSel( 0 );
}

bool FieldManagerDlg::isFieldExistInListBox( const CString& field )
{
    return ( m_fieldListBox.FindString( 0, field ) != LB_ERR );
}

bool FieldManagerDlg::isValidField( const CString& field )
{
    CString tempStr( field );
    tempStr.Trim();

    // 排除空字符串
    if( tempStr.GetLength() == 0 ) return false;

    // 排除非法字符，例如
    // {. * & @ # $ % ...}
    if( tempStr.Find( _T( '.' ) ) >= 0 ) return false;

    return true;
}

void FieldManagerDlg::showFieldInfo()
{
    int sel = m_fieldListBox.GetCurSel();
    if( sel != LB_ERR )
    {
        FieldInfo* pInfo = m_infoes[sel];
        fidlg.readFromFieldInfo( *pInfo );
        fidlg.UpdateData( FALSE );

        setLastSelIndex( sel ); // 记录"上一次"的索引位置
    }
}

void FieldManagerDlg::fillContent()
{
    // 填充所有的图元及数据类型
    m_typeCombo.ResetContent();
    m_fieldListBox.ResetContent();

    //AcStringArray types;
    //FieldHelper::GetAllRegTypes( types );
    //if( types.isEmpty() ) return;

	classNameTrans();

	if( m_namesEn.IsEmpty() || m_namesZh.IsEmpty() ) return;
    int len = m_namesEn.GetCount();
    //acutPrintf(_T("\n总个数:%d"), len);
    for( int i = 0; i < len; i++ )
    {
        m_typeCombo.AddString( m_namesZh[i] );
        //acutPrintf(_T("\n%d：%s"), i, types[i].kACharPtr());
    }

    // 定位第1个图元
    m_typeCombo.SetCurSel( 0 );

    // 填充第1个类型所包含的所有字段
    fillFieldListBox( m_namesEn[0] );

    // 显示第1个字段信息
    showFieldInfo();
}

void FieldManagerDlg::OnPaint()
{
    CPaintDC dc( this ); // device context for painting

    CRect rect;
    GetDlgItem( IDC_DRAW_POS )->GetWindowRect( &rect );
    ScreenToClient( &rect );

    CString type = getSelType();
    if( type.GetLength() != 0 )
    {
        CString name( type );
        //TypeNameMapHelper::GetTypeNameMap(type, name);
        ArxClassHelper::GetDxfName( type, name );
        CString text;
        text.Format( _T( "DXF名称: %s" ), name );
        dc.DrawText( text, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
    }

    // 后续可以参考CustomProperties.cpp中的
    // void DateTimeProperty::OnDrawDescription( CDC* pDC, CRect rect ) 代码实现
    // 要求：字体清晰，醒目(对比效果好)
}

void FieldManagerDlg::setLastSelIndex( int sel )
{
    m_lastSel = sel;
}

int FieldManagerDlg::getLastSelIndex() const
{
    return m_lastSel;
}