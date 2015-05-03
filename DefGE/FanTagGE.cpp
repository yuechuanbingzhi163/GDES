#include "StdAfx.h"
#include "FanTagGE.h"
#include "../ArxHelper/HelperClass.h"

Adesk::UInt32 FanTagGE::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
						 FanTagGE, TagGE,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						 AcDbProxyEntity::kNoOperation,
						 ·ç»ú±êÇ©, DEFGEAPP
						 )

FanTagGE::FanTagGE()
{
}

FanTagGE::~FanTagGE ()
{
}

FanTagGE::FanTagGE( AcGePoint3d& insertPt,CString name,CString way,double q,double h ):m_insertPt(insertPt),m_name(name),m_way(way),m_q(q),m_h(h)
{

}

Acad::ErrorStatus FanTagGE::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es = TagGE::dwgOutFields ( pFiler ) ;
	if ( es != Acad::eOk )
		return ( es ) ;
	//----- Object version number needs to be saved first
	if ( ( es = pFiler->writeUInt32 ( FanTagGE::kCurrentVersionNumber ) ) != Acad::eOk )
		return ( es ) ;

	return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus FanTagGE::dwgInFields ( AcDbDwgFiler* pFiler )
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
	if ( version > FanTagGE::kCurrentVersionNumber )
		return ( Acad::eMakeMeProxy ) ;

	return ( pFiler->filerStatus () ) ;
}

void FanTagGE::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble( m_q );
	reader.readDouble(m_h);
	reader.readString(m_name);
	reader.readString(m_way);
}

void FanTagGE::writeKeyParam( DrawParamWriter& writer ) const
{
	writer.writePoint( m_insertPt );
	writer.writeDouble( m_q );
	writer.writeDouble( m_h );
	writer.writeString(m_name);
	writer.writeString(m_way);
}
