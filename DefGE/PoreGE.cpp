#include "StdAfx.h"
#include "PoreGE.h"

Adesk::UInt32 PoreGE::kCurrentVersionNumber = 1 ;

ACRX_NO_CONS_DEFINE_MEMBERS( PoreGE, MineGE )

PoreGE::PoreGE()
{

}

PoreGE::PoreGE( const AcGePoint3d& insertPt ) : m_insertPt( insertPt )
{

}

AcGePoint3d PoreGE::getInsertPt() const
{
    assertReadEnabled();
    return m_insertPt;
}

void PoreGE::setInsertPt( const AcGePoint3d& pt )
{
    assertWriteEnabled();
    m_insertPt = pt;
}

void PoreGE::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
}

void PoreGE::writeKeyParam( DrawParamWriter& writer ) const
{
    writer.writePoint( m_insertPt );
}

Acad::ErrorStatus PoreGE::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = MineGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( PoreGE::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeItem( m_insertPt );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus PoreGE::dwgInFields ( AcDbDwgFiler* pFiler )
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
    if ( version > PoreGE::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    pFiler->readItem( &m_insertPt );

    return ( pFiler->filerStatus () ) ;
}