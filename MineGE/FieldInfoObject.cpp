#include "StdAfx.h"
#include "FieldInfoObject.h"

Adesk::UInt32 FieldInfoObject::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    FieldInfoObject, AcDbObject,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation, FIELDINFOOBJECT,
    MINEGEAPP
    | Product Desc:     A description for your object
    | Company:          Your company name
    | WEB Address:      Your company WEB site address
)

    FieldInfoObject::FieldInfoObject () : AcDbObject ()
{
}

FieldInfoObject::~FieldInfoObject ()
{
}

Acad::ErrorStatus FieldInfoObject::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = AcDbObject::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( FieldInfoObject::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeInt16( m_info.m_dt );

    pFiler->writeInt32( m_info.m_minValue2 );
    pFiler->writeInt32( m_info.m_maxValue2 );

    pFiler->writeInt32( m_info.m_minValue );
    pFiler->writeInt32( m_info.m_maxValue );

    pFiler->writeInt16( m_info.m_lt );
    pFiler->writeString( ( LPCTSTR )m_info.m_varName );

    pFiler->writeBool( m_info.m_enable );
    pFiler->writeString( ( LPCTSTR )m_info.m_descr );
    pFiler->writeBool( m_info.m_showInTooltip );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus FieldInfoObject::dwgInFields ( AcDbDwgFiler* pFiler )
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
    if ( version > FieldInfoObject::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    short iValue;
    pFiler->readInt16( &iValue );
    m_info.m_dt = ( DATA_TYPE )iValue;

    long nValue;
    pFiler->readInt32( &nValue );
    m_info.m_minValue2 = ( int )nValue;
    pFiler->readInt32( &nValue );
    m_info.m_maxValue2 = ( int )nValue;

    double dValue;
    pFiler->readDouble( &dValue );
    m_info.m_minValue = nValue;
    pFiler->readDouble( &dValue );
    m_info.m_maxValue = nValue;

    pFiler->readInt16( &iValue );
    m_info.m_lt = ( LIST_TYPE )iValue;

    AcString str;
    pFiler->readString( str );
    m_info.m_varName = str.kACharPtr();

    bool bValue;
    pFiler->readBool( &bValue );
    m_info.m_enable = bValue;

    pFiler->readString( str );
    m_info.m_descr = str.kACharPtr();

    pFiler->readBool( &bValue );
    m_info.m_showInTooltip = bValue;

    return ( pFiler->filerStatus () ) ;
}