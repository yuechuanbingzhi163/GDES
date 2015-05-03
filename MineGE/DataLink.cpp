#include "StdAfx.h"
#include "DataLink.h"

#include "DataHelper.h"
#include "DataHelperImpl.h"

#include "FieldHelper.h"
#include "FieldType.h"

#include "../ArxHelper/HelperClass.h"

static resbuf* GoToListEnd( resbuf* pHead, const CString& field )
{
    resbuf* pTemp = pHead;
    while( pTemp != 0 )
    {
        // 存在相同的字段名称
        if( field.CompareNoCase( pTemp->resval.rstring ) == 0 ) break;
        bool isBreak = false;
        // 循环3次，跳到下一个字段节点
        for( int i = 0; i < 3; i++ )
        {
            if( pTemp->rbnext == 0 )
            {
                isBreak = true;
                break;
            }
            pTemp = pTemp->rbnext;
        }
        if( isBreak ) break;
    }
    return pTemp;
}

static void LinkGenericData( resbuf*& pHead, const CString& field, short rtype, void* pValue )
{
    if( field.GetLength() == 0 ) return;

    resbuf* pTemp = 0;
    if( pHead == 0 )
    {
        pHead = acutBuildList( RTSTR, field, RTSHORT, rtype, RTLONG_PTR, ( long )pValue, 0 );
    }
    else
    {
        pTemp = GoToListEnd( pHead, field );
        if( pTemp->rbnext == 0 )
        {
            pTemp->rbnext = acutBuildList( RTSTR, field, RTSHORT, rtype, RTLONG_PTR, ( long )pValue, 0 );
        }
    }
}

static resbuf* SetData( resbuf* pStart, const CString& value )
{
    resbuf* pTypeNode = pStart->rbnext;
    resbuf* pValueNode = pTypeNode->rbnext;

    short rtype = pTypeNode->resval.rint;
    long ptr = pValueNode->resval.mnLongPtr;
    switch( rtype )
    {
    case DT_STRING:
    {
        CString* pValue = ( CString* )ptr;
        *pValue = value;
    }
    break;

    case DT_INT:
    {
        int iv = 0;
        ArxUtilHelper::StringToInt( value, iv );
        int* pValue = ( int* )ptr;
        *pValue = iv;
    }
    break;

    case DT_NUMERIC:
    {
        double dv = 0;
        ArxUtilHelper::StringToDouble( value, dv );
        double* pValue = ( double* )ptr;
        *pValue = dv;
    }
    break;

    case DT_BOOL:
    {
        int iv = 0;
        bool bv = false;
        if( ArxUtilHelper::StringToInt( value, iv ) )
        {
            bv = ( iv != 0 ); // 等于0为假，不等于0为真
        }
        bool* pValue = ( bool* )ptr;
        *pValue = bv;
    }
    break;

    case DT_DATE:
    {
        COleDateTime dt;
        if( ( value.GetLength() == 0 ) || !dt.ParseDateTime( value ) )
        {
            // 当前时间
            dt = COleDateTime::GetCurrentTime();
        }
        COleDateTime* pValue = ( COleDateTime* )ptr;
        *pValue = dt;
    }
    break;
    }
    return pValueNode->rbnext;
}

static resbuf* GetData( resbuf* pStart, CString& value )
{
    resbuf* pTypeNode = pStart->rbnext;
    resbuf* pValueNode = pTypeNode->rbnext;

    short rtype = pTypeNode->resval.rint;
    long ptr = pValueNode->resval.mnLongPtr;
    switch( rtype )
    {
    case DT_STRING:
        value = *( ( CString* )ptr );
        break;

    case DT_INT:
        ArxUtilHelper::IntToString( *( ( int* )ptr ), value );
        break;

    case DT_NUMERIC:
        ArxUtilHelper::DoubleToString( *( ( double* )ptr ), value );
        break;

    case DT_BOOL:
    {
        bool bv = *( bool* )ptr;
        value.Format( _T( "%d" ), ( bv ? -1 : 0 ) ); // -1表示真, 0表示假
    }
    break;

    case DT_DATE:
    {
        COleDateTime dt = *( COleDateTime* )ptr;
        value = ( COleVariant )dt;
    }
    break;
    }
    return pValueNode->rbnext;
}

DataLink::DataLink() : m_pDatasToFileds( 0 )
{

}

DataLink::~DataLink( void )
{
    if( m_pDatasToFileds != NULL )
    {
        acutRelRb( m_pDatasToFileds );
    }
}

void DataLink::setDataSource( const AcDbObjectId& objId )
{
    // 只允许关联一次
    if( !m_objId.isNull() | objId.isNull() ) return;
    m_objId = objId;
}

AcDbObjectId DataLink::getDataSource() const
{
    return m_objId;
}

void DataLink::linkStringData( const CString& field, CString* pValue )
{
    LinkGenericData( m_pDatasToFileds, field, DT_STRING, pValue );
}

void DataLink::linkIntData( const CString& field, int* pValue )
{
    LinkGenericData( m_pDatasToFileds, field, DT_INT, pValue );
}

void DataLink::linkDoubleData( const CString& field, double* pValue )
{
    LinkGenericData( m_pDatasToFileds, field, DT_NUMERIC, pValue );
}

void DataLink::linkBoolData( const CString& field, bool* pValue )
{
    LinkGenericData( m_pDatasToFileds, field, DT_BOOL, pValue );
}

void DataLink::linkDateTimeData( const CString& field, COleDateTime* pValue )
{
    LinkGenericData( m_pDatasToFileds, field, DT_DATE, pValue );
}

void DataLink::initDatas()
{
    if( m_pDatasToFileds == 0 )
    {
        regDatas();
    }
}

bool DataLink::checkFields()
{
    // 获取类型名称
    CString type;
    if( !DataHelper::GetTypeName( m_objId, type ) ) return false;

    // 获取字段
    AcStringArray fields;
    FieldHelper::GetAllFields( type, fields );
    if( fields.isEmpty() ) return false;

    // 判断字段是否合理
    resbuf* pTemp = m_pDatasToFileds;
    while( pTemp != 0 )
    {
        // 判断字段是否存在
        if( !fields.contains( pTemp->resval.rstring ) )
        {
            // 测试用
            acutPrintf( _T( "\n类型:%s\t字段:[%s]不存在\n" ), type, pTemp->resval.rstring );
            break;
        }
        for( int i = 0; i < 3; i++ ) pTemp = pTemp->rbnext;
    }

    // 如果所有字段都存在，那么pTemp=0
    return ( pTemp == 0 );
}

static bool UpdateData_Helper( const AcDbObjectId& dObjId, resbuf* pTemp, bool save )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDb::OpenMode om = ( save ? ( AcDb::kForWrite ) : ( AcDb::kForRead ) );
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, dObjId, om ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    DataObject* pDO = DataObject::cast( pObj );
    if( pDO == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    // 委托DataHelperImpl来操纵属性数据
    DataHelperImpl dh( pDO );
    while( pTemp != 0 )
    {
        CString field = pTemp->resval.rstring;
        if( save )
        {
            CString value;
            pTemp = GetData( pTemp, value );
            dh.setPropertyData( field, value );
        }
        else
        {
            CString value;
            bool ret = dh.getPropertyData( field, value );
            if( !ret )
            {
                for( int i = 0; i < 3; i++ ) pTemp = pTemp->rbnext;
            }
            else
            {
                pTemp = SetData( pTemp, value );
            }
        }
    }
    actrTransactionManager->endTransaction();

    return true;
}

bool DataLink::updateData( bool save )
{
    // 获取数据对象
    AcDbObjectId dObjId;
    if( !DataHelper::GetDataObject( m_objId, dObjId ) ) return false;

    // 如果字段没有初始化，则初始化
    initDatas();

    // 检查字段是否合理
    //if(!checkFields()) return false;
    checkFields();

    // 更新数据
    return UpdateData_Helper( dObjId, m_pDatasToFileds, save );
}