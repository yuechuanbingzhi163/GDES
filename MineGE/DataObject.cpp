#include "StdAfx.h"
#include "DataObject.h"
#include "FieldHelper.h"

Adesk::UInt32 DataObject::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    DataObject, AcDbObject,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    内部使用数据对象, MINEGEAPP
)

DataObject::DataObject () : AcDbObject ()
{
}

DataObject::~DataObject ()
{
}

Acad::ErrorStatus DataObject::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = AcDbObject::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( DataObject::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    // 写入图元id
    pFiler->writeSoftPointerId( m_objId );

    // 写入类型名称
    pFiler->writeItem( m_type );

    // 写入数据
    int len = m_values.length();
    pFiler->writeInt32( len );
    for( int i = 0; i < len; i++ )
    {
        pFiler->writeString( m_values[i] );
    }

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus DataObject::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = AcDbObject::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > DataObject::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    // 写入图元id
    AcDbSoftPointerId id;
    pFiler->readSoftPointerId( &id );
    m_objId = id;

    // 读取类型名称
    AcString str;
    pFiler->readString( str );
    m_type = str.kACharPtr();

    // 清空并读取数据
    m_values.removeAll();
    Adesk::Int32 len = 0;
    pFiler->readInt32( &len );
    for( int i = 0; i < ( int )len; i++ )
    {
        pFiler->readString( str );
        m_values.append( str );
    }

    return ( pFiler->filerStatus () ) ;
}

bool DataObject::isValidIndex( int pos ) const
{
    assertReadEnabled();

    return ( pos >= 0 && pos < m_values.length() );
}

bool DataObject::getData( int pos, CString& value ) const
{
    assertReadEnabled();

    if( !isValidIndex( pos ) ) return false;
    value = m_values[pos].kACharPtr();
    return true;
}

bool DataObject::setData( int pos, const CString& value )
{
    assertWriteEnabled();

    if( !isValidIndex( pos ) ) return false;

    m_values[pos] = value;

    return true;
}

bool DataObject::setType( const CString& type )
{
    assertWriteEnabled();

    if( type.GetLength() == 0 ) return false;
    m_type = type;
    return true;
}

CString DataObject::getType() const
{
    assertReadEnabled();

    return m_type;
}

void DataObject::initData()
{
    assertWriteEnabled();

    int count = FieldHelper::CountFields( getType() );
    if( count <= 0 ) return;

    m_values.removeAll();
    for( int i = 0; i < count; i++ )
    {
        m_values.append( _T( "" ) ); // 添加空字符串
    }
}

void DataObject::getAllData( AcStringArray& values )
{
    assertReadEnabled();
    values.append( m_values );
}

void DataObject::setAllData( const AcStringArray& values )
{
    assertWriteEnabled();
    clearAll();
    m_values.append( values );
}

void DataObject::addData()
{
    assertWriteEnabled();
    m_values.append( _T( "" ) );
}

void DataObject::removeData( int pos )
{
    assertWriteEnabled();
    if( !isValidIndex( pos ) ) return;
    m_values.removeAt( pos );
}

void DataObject::clearAll()
{
    assertWriteEnabled();
    m_values.removeAll();
}

AcDbObjectId DataObject::getGE() const
{
    assertReadEnabled();
    return m_objId;
}

void DataObject::setGE( const AcDbObjectId& objId )
{
    assertWriteEnabled();
    m_objId = objId;
}