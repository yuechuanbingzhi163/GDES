#include "StdAfx.h"
#include "CoalSurface.h"

Adesk::UInt32 CoalSurface::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
	CoalSurface, RcuTagGE,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation,
	Ãº²ã, DEFGEAPP
)

CoalSurface::CoalSurface()
{
	m_width = m_height = 1.0;
}

AcGePoint3d CoalSurface::getInsertPt() const
{
    assertReadEnabled();
    return m_insertPt;
}

void CoalSurface::setInsertPt( const AcGePoint3d& pt )
{
    assertWriteEnabled();
    m_insertPt = pt;
}

double CoalSurface::getWidth() const
{
	assertReadEnabled();
	return m_width;
}

void CoalSurface::setWidth(double w)
{
	assertWriteEnabled();
	m_width = w;
}

double CoalSurface::getHeight() const
{
	assertReadEnabled();
	return m_height;
}

void CoalSurface::setHeight(double h)
{
	assertWriteEnabled();
	m_height = h;
}

void CoalSurface::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint(m_insertPt);
	reader.readDouble(m_width);
	reader.readDouble(m_height);
}

void CoalSurface::writeKeyParam( DrawParamWriter& writer ) const
{
    writer.writePoint(m_insertPt);
	writer.writeDouble(m_width);
	writer.writeDouble(m_height);
}

Acad::ErrorStatus CoalSurface::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = RcuTagGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( CoalSurface::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeItem(m_insertPt);
	pFiler->writeItem(m_width);
	pFiler->writeItem(m_height);

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus CoalSurface::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = RcuTagGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > CoalSurface::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    pFiler->readItem(&m_insertPt);
	pFiler->readItem(&m_width);
	pFiler->readItem(&m_height);

    return ( pFiler->filerStatus () ) ;
}