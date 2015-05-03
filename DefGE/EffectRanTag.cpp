#include "StdAfx.h"
#include "EffectRanTag.h"
#include "../ArxHelper/HelperClass.h"

Adesk::UInt32 EffectRanTagGE::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
						 EffectRanTagGE, TagGE,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						 AcDbProxyEntity::kNoOperation,
						 ÓÐÐ§Éä³Ì, DEFGEAPP
						 )

						 EffectRanTagGE::EffectRanTagGE()
{
}

EffectRanTagGE::~EffectRanTagGE ()
{
}

EffectRanTagGE::EffectRanTagGE( AcGePoint3d& insertPt,double angle,double minDistan,double maxDistan,double diatance ):m_insertPt(insertPt),m_angle(angle),m_minDistan(minDistan),m_maxDistan(maxDistan),m_diatance(diatance)
{

}

Acad::ErrorStatus EffectRanTagGE::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es = TagGE::dwgOutFields ( pFiler ) ;
	if ( es != Acad::eOk )
		return ( es ) ;
	//----- Object version number needs to be saved first
	if ( ( es = pFiler->writeUInt32 ( EffectRanTagGE::kCurrentVersionNumber ) ) != Acad::eOk )
		return ( es ) ;

	return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus EffectRanTagGE::dwgInFields ( AcDbDwgFiler* pFiler )
{
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es = TagGE::dwgInFields ( pFiler ) ;
	if ( es != Acad::eOk )
		return ( es ) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0 ;
	if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
		return ( es ) ;
	if ( version > EffectRanTagGE::kCurrentVersionNumber )
		return ( Acad::eMakeMeProxy ) ;

	return ( pFiler->filerStatus () ) ;
}

void EffectRanTagGE::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble( m_angle );
	reader.readDouble(m_minDistan);
	reader.readDouble(m_maxDistan);
	reader.readDouble(m_diatance);
}

void EffectRanTagGE::writeKeyParam( DrawParamWriter& writer ) const
{
	writer.writePoint( m_insertPt );
	writer.writeDouble( m_angle );
	writer.writeDouble(m_minDistan);
	writer.writeDouble(m_maxDistan);
	writer.writeDouble(m_diatance);
}
