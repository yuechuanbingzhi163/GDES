#include "StdAfx.h"
#include "DataHelperImpl.h"

#include "FieldHelper.h"

static void DealSpecialString( CString& value )
{
    // 删除首尾空白
    value.Trim();

    // 忽略"null"字符串
    if( value.CompareNoCase( _T( "NULL" ) ) == 0 )
    {
        value = _T( "" );
    }
}

DataHelperImpl::DataHelperImpl ( DataObject* pObj ) : m_pObj( pObj )
{
}

bool DataHelperImpl::getPropertyData( const CString& fieldName, CString& value )
{
    //m_pObj->assertReadEnabled();

    int index = FieldHelper::FindField( m_pObj->getType(), fieldName );
    if( index < 1 ) return false; // 字段不存在

	// 字段返回的索引从1开始
    // 而数据的索引从0开始
    bool ret = m_pObj->getData( index - 1, value );
    if( ret )
    {
        // 屏蔽一些特殊字符
        DealSpecialString( value );
    }
    return ret;
}

bool DataHelperImpl::setPropertyData( const CString& fieldName, const CString& value )
{
    //m_pObj->assertWriteEnabled();

    int index = FieldHelper::FindField( m_pObj->getType(), fieldName );
    if( index < 1 ) return false; // 字段不存在

    // 屏蔽一些字符
    CString vv( value );
    DealSpecialString( vv );

    // 字段返回的索引从1开始
    // 而数据的索引从0开始
    return m_pObj->setData( index - 1, vv );
}
