#include "StdAfx.h"
#include "RockGate.h"

Adesk::UInt32 RockGate::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
	RockGate, RcuGE,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation,
	й╞це, DEFGEAPP
)

RockGate::RockGate()
{
}

AcGePoint3d RockGate::getInsertPt() const
{
    assertReadEnabled();
    return m_insertPt;
}

void RockGate::setInsertPt( const AcGePoint3d& pt )
{
    assertWriteEnabled();
    m_insertPt = pt;
}

void RockGate::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint(m_insertPt);
}

void RockGate::writeKeyParam( DrawParamWriter& writer ) const
{
    writer.writePoint(m_insertPt);
}

Acad::ErrorStatus RockGate::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = RcuGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( RockGate::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeItem(m_insertPt);

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus RockGate::dwgInFields ( AcDbDwgFiler* pFiler )
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
    if ( version > RockGate::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    pFiler->readItem(&m_insertPt);

    return ( pFiler->filerStatus () ) ;
}