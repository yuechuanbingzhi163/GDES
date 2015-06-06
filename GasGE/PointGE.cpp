#include "StdAfx.h"
#include "PointGE.h"

Adesk::UInt32 PointGE::kCurrentVersionNumber = 1 ;

ACRX_NO_CONS_DEFINE_MEMBERS( PointGE, MineGE )

PointGE::PointGE() : m_angle( 0 )
{

}

PointGE::PointGE( const AcGePoint3d& insertPt ) : m_insertPt( insertPt ), m_angle( 0 )
{

}

AcGePoint3d PointGE::getInsertPt() const
{
    assertReadEnabled();
    return m_insertPt;
}

void PointGE::setInsertPt( const AcGePoint3d& pt )
{
    assertWriteEnabled();
    m_insertPt = pt;
}

void PointGE::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void PointGE::writeKeyParam( DrawParamWriter& writer ) const
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

Acad::ErrorStatus PointGE::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = MineGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( PointGE::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeItem( m_insertPt );
    pFiler->writeItem( m_angle );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus PointGE::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = MineGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > PointGE::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    pFiler->readItem( &m_insertPt );
    pFiler->readItem( &m_angle );

    return ( pFiler->filerStatus () ) ;
}