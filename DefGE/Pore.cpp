#include "StdAfx.h"
#include "Pore.h"

Adesk::UInt32 Pore::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
	Pore, RcuGE,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation,
	×ê¿×, DEFGEAPP
)

Pore::Pore()
{

}

AcGePoint3d Pore::getInsertPt() const
{
    assertReadEnabled();
    return m_insertPt;
}

void Pore::setInsertPt( const AcGePoint3d& pt )
{
    assertWriteEnabled();
    m_insertPt = pt;
}

void Pore::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
}

void Pore::writeKeyParam( DrawParamWriter& writer ) const
{
    writer.writePoint( m_insertPt );
}

Acad::ErrorStatus Pore::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = RcuGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( Pore::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeItem( m_insertPt );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus Pore::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = RcuGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > Pore::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    pFiler->readItem( &m_insertPt );

    return ( pFiler->filerStatus () ) ;
}