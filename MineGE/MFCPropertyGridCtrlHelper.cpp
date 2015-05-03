#include "StdAfx.h"
#include "MFCPropertyGridCtrlHelper.h"
#include "CustomProperties.h"
#include "FuncFieldHelper.h"

#define bool2BOOL(b) (b?1:0)

MFCPropertyGridCtrlHelper::MFCPropertyGridCtrlHelper( CMFCPropertyGridCtrl* pWndPropList )
    : m_pWndPropList( NULL )
{
    if( pWndPropList != 0 )
    {
        m_pWndPropList = pWndPropList;
		//acutPrintf(_T("\n类型:%s"),m_type);
        m_pWndPropList->EnableHeaderCtrl( TRUE, _T( "属性" ), _T( "值" ) );
        m_pWndPropList->EnableDescriptionArea();
        m_pWndPropList->SetVSDotNetLook();
        m_pWndPropList->MarkModifiedProperties();
		//m_pWndPropList->SetAlphabeticMode(false);
		m_pWndPropList->SetShowDragContext();
	}
}

static void SetAndAddToPropertyList( CMFCPropertyGridProperty *pGroup, CMFCPropertyGridProperty* pProp, bool bEnable, bool bEditable, bool isBoolType = false )
{
    pProp->Enable( bool2BOOL( bEnable ) );
    if( !isBoolType ) pProp->AllowEdit( bool2BOOL( bEditable ) );
	pGroup->AddSubItem(pProp);
	//acutPrintf(_T("\n属性名:%s"),pProp->GetName());
    //pPropList->AddProperty( pProp );
}

void MFCPropertyGridCtrlHelper::addStringProperty( CMFCPropertyGridProperty *pGroup, const CString& name, const CString& value, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( pGroup );
    // VT_BSTR
    CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty( name, ( COleVariant )value, descr );
	SetAndAddToPropertyList( pGroup,pProp, bEnable, true );
}

void MFCPropertyGridCtrlHelper::addIntProperty( CMFCPropertyGridProperty *pGroup, const CString& name, int value, int nMinValue, int nMaxValue, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( pGroup );
    // VT_I4
    if( value < nMinValue || value > nMaxValue ) value = nMinValue;
    CMFCPropertyGridProperty* pProp = new CIntProp( name, ( long )value, nMinValue, nMaxValue, descr );
    pProp->EnableSpinControl( TRUE, nMinValue, nMaxValue ); // 使用spin contrl(微调控件)
    SetAndAddToPropertyList( pGroup, pProp, bEnable, true );
}

void MFCPropertyGridCtrlHelper::addDoubleProperty( CMFCPropertyGridProperty *pGroup, const CString& name, double value, double dMinValue, double dMaxValue, unsigned short precise, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( pGroup );
    // VT_R8
    if( value < dMinValue || value > dMaxValue ) value = dMinValue;
    CMFCPropertyGridProperty* pProp = new CNumericProp( name, value, dMinValue, dMaxValue, precise, descr );
    SetAndAddToPropertyList( pGroup, pProp, bEnable, true );
}

void MFCPropertyGridCtrlHelper::addBoolProperty( CMFCPropertyGridProperty *pGroup, const CString& name, bool value, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( pGroup );

    
    // VT_BOOL
 //   COleVariant v((short)value, VT_BOOL);
 //   CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(name, v, descr);
	//SetAndAddToPropertyList(pGroup, pProp, bEnable, false, true); // 针对bool类型，bEditable参数无效
    
    // 用IntStrProp模拟
    // 主要是为了使用汉字(是/否)，替代默认的bool英文(true/false)
    AcDbIntArray intValues;
    intValues.append( 1 );
    intValues.append( 0 );
    AcStringArray strValues;
    strValues.append( _T( "是" ) );
    strValues.append( _T( "否" ) );
    IntStrProp* pProp = new IntStrProp( name, ( value ? 1 : 0 ), intValues, strValues, descr );
    SetAndAddToPropertyList( pGroup, pProp, bEnable, false ); // 鼠标不可编辑
}

void MFCPropertyGridCtrlHelper::addDateTimeProperty( CMFCPropertyGridProperty *pGroup, const CString& name, const COleDateTime& value, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( pGroup );

    DateTimeProperty* pProp = new DateTimeProperty( name, ( COleVariant )value, descr );
    SetAndAddToPropertyList( pGroup, pProp, bEnable, true );
}

void MFCPropertyGridCtrlHelper::addStringPropertyList( CMFCPropertyGridProperty *pGroup, const CString& name, const CString& value, const AcStringArray& values, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( pGroup );
    if( values.isEmpty() ) return;

    CString	v = value;
    if( !values.contains( value ) )
    {
        v = values.first().kACharPtr(); // 如果不存在，默认使用列表中的第1个
    }

    CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty( name, ( COleVariant )v, descr );
    int len = values.length();
    for( int i = 0; i < len; i++ )
    {
        pProp->AddOption( values[i].kACharPtr() );
    }
    SetAndAddToPropertyList( pGroup, pProp, bEnable, false ); // 鼠标不可编辑
}

void MFCPropertyGridCtrlHelper::addIntPropertList( CMFCPropertyGridProperty *pGroup, const CString& name, int value, const AcDbIntArray& intValues, const AcStringArray& strValues, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( pGroup );
    if( intValues.isEmpty() || strValues.isEmpty() || intValues.length() != strValues.length() ) return;
	if( !intValues.contains( value ) )
    {
        value = intValues[0]; // 如果不存在，默认使用列表中的第1个
    }

    IntStrProp* pProp = new IntStrProp( name, value, intValues, strValues, descr );
    //COleVariant v = pProp->GetValue();
    //assert(v.vt == VT_INT); // false
    //assert(v.vt == VT_I2); // true
    SetAndAddToPropertyList( pGroup, pProp, bEnable, false ); // 鼠标不可编辑
}

void MFCPropertyGridCtrlHelper::addDataObjectPropertyList( CMFCPropertyGridProperty *pGroup, const CString& name, const CString& value, const AcStringArray& values, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( pGroup );
    if( values.isEmpty() ) return;

    CString	v = value;
    if( !values.contains( value ) )
    {
        //v = values.first().kACharPtr(); // 如果不存在，默认使用列表中的第1个
        v = _T( "" ); // 空白
    }

    DataObjectProperty* pProp = new DataObjectProperty( name, v, values, descr );
    SetAndAddToPropertyList( pGroup, pProp, bEnable, false ); // 鼠标不可编辑
}

// 点击类属性, 有一个可以点击的"..."按钮
// 程序可以考虑采用类似委托的方法(也可称为回调)更新数据
// 例如：井筒附加属性, 点击按钮弹出对话框进行设置
// 按钮相当于一个"计算器"
// 目前的打算:
//     1) 这类属性不在"字段管理对话框"和"字段信息对话框"中显示
//     2) 只可以通过函数来创建, 可能是在MFCPropertyGridCtrlHelper类中增加一个方法
// 详细的用法示例参见PropertyDataUpdater.cpp中的BuildPropList2()的注释部分
// 由于尚处于试验阶段，暂时不考虑在项目中使用ClickProp
class CustomClickProp : public CMFCPropertyGridProperty
{
public:
    CustomClickProp( const CString& strName, const CString& value, ClickPropModifyCallBack cpmcb, LPCTSTR lpszDescr = NULL );

protected:
    virtual BOOL HasButton() const
    {
        return TRUE;
    }
    virtual void OnClickButton( CPoint point );

private:
    ClickPropModifyCallBack m_cpmcb;
};

CustomClickProp::CustomClickProp( const CString& strName, const CString& value, ClickPropModifyCallBack cpmcb, LPCTSTR lpszDescr )
    : CMFCPropertyGridProperty( strName, ( COleVariant )value, lpszDescr ), m_cpmcb( cpmcb )
{
    //AllowEdit(FALSE);
}

void CustomClickProp::OnClickButton( CPoint /*point*/ )
{
    //AfxMessageBox(_T("Show your dialog here..."));
    CString newValue;
    bool ret = ( *m_cpmcb )( GetValue(), newValue );
    if( ret ) SetValue( ( COleVariant )newValue );
}

void MFCPropertyGridCtrlHelper::addClickProperty( CMFCPropertyGridProperty *pGroup, const CString& name, const CString& value, ClickPropModifyCallBack cpmcb, LPCTSTR descr, bool bEnable )
{
    ASSERT_VALID( m_pWndPropList );
    ASSERT_VALID( cpmcb != 0 );

    CustomClickProp* pProp = new CustomClickProp( name, ( COleVariant )value, cpmcb, descr );
    SetAndAddToPropertyList( pGroup, pProp, bEnable, false ); // 鼠标不可编辑
}
