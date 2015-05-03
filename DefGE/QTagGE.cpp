#include "StdAfx.h"
#include "QTagGE.h"
#include "../ArxHelper/HelperClass.h"

Adesk::UInt32 QTagGE::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
						 QTagGE, TagGE,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						 AcDbProxyEntity::kNoOperation,
						 ·çÁ¿±êÇ©, DEFGEAPP
						 )

						 QTagGE::QTagGE()
{
}

QTagGE::~QTagGE ()
{
}

QTagGE::QTagGE( AcGePoint3d& insertPt,double q ):m_insertPt(insertPt),m_q(q)
{

}

Acad::ErrorStatus QTagGE::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es = TagGE::dwgOutFields ( pFiler ) ;
	if ( es != Acad::eOk )
		return ( es ) ;
	//----- Object version number needs to be saved first
	if ( ( es = pFiler->writeUInt32 ( QTagGE::kCurrentVersionNumber ) ) != Acad::eOk )
		return ( es ) ;

	return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus QTagGE::dwgInFields ( AcDbDwgFiler* pFiler )
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
	if ( version > QTagGE::kCurrentVersionNumber )
		return ( Acad::eMakeMeProxy ) ;

	return ( pFiler->filerStatus () ) ;
}

void QTagGE::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble( m_q );
}

void QTagGE::writeKeyParam( DrawParamWriter& writer ) const
{
	writer.writePoint( m_insertPt );
	writer.writeDouble( m_q );
}
