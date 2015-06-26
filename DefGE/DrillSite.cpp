#include "StdAfx.h"
#include "DrillSite.h"

Adesk::UInt32 DrillSite::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
	DrillSite, RcuGE,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation,
	×ê³¡, DEFGEAPP
)

DrillSite::DrillSite()
{

}

AcGePoint3d DrillSite::getInsertPt() const
{
    assertReadEnabled();
    return m_insertPt;
}

void DrillSite::setInsertPt( const AcGePoint3d& pt )
{
    assertWriteEnabled();
    m_insertPt = pt;
}

//AcGePoint3d DrillSite::getLinkPt() const
//{
//	assertReadEnabled();
//	return m_linkPt;
//}
//
//void DrillSite::setLinkPt( const AcGePoint3d& pt )
//{
//	assertWriteEnabled();
//	m_linkPt = pt;
//}

void DrillSite::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint(m_insertPt);
	//reader.readPoint(m_linkPt);
}

void DrillSite::writeKeyParam( DrawParamWriter& writer ) const
{
    writer.writePoint(m_insertPt);
	//writer.writePoint(m_linkPt);
}

Acad::ErrorStatus DrillSite::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = MineGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( DrillSite::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeItem(m_insertPt);
	//pFiler->writeItem(m_linkPt);

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus DrillSite::dwgInFields ( AcDbDwgFiler* pFiler )
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
    if ( version > DrillSite::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    pFiler->readItem(&m_insertPt);
	//pFiler->readItem(&m_linkPt);

    return ( pFiler->filerStatus () ) ;
}