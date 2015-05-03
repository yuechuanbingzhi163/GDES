#include "StdAfx.h"
#include "SimpleWindLibraryDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleWindLibraryDraw, MineGEDraw, 1 )

SimpleWindLibraryDraw::SimpleWindLibraryDraw () : MineGEDraw ()
{
}

SimpleWindLibraryDraw::~SimpleWindLibraryDraw ()
{
}

void SimpleWindLibraryDraw::setAllExtraParamsToDefault()
{
	m_height = 80;
	m_width = 100;
	//m_distance = 16;
}

void SimpleWindLibraryDraw::configExtraParams()
{

}

void SimpleWindLibraryDraw::updateExtraParams()
{

}

void SimpleWindLibraryDraw::writeKeyParam( DrawParamWriter& writer )
{
	writer.writePoint( m_insertPt );
	writer.writeDouble( m_angle );
}

void SimpleWindLibraryDraw::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble( m_angle );
}

void SimpleWindLibraryDraw::readExtraParam( DrawParamReader& reader )
{
	reader.readDouble( m_height );
	reader.readDouble( m_width );
}

void SimpleWindLibraryDraw::writeExtraParam( DrawParamWriter& writer )
{
	writer.writeDouble( m_height );
	writer.writeDouble( m_width );
}

Adesk::Boolean SimpleWindLibraryDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
	assertReadEnabled () ;
	drawWindLibrary(mode);
	return Adesk::kTrue;
}

Acad::ErrorStatus SimpleWindLibraryDraw::subTransformBy( const AcGeMatrix3d& xform )
{
	m_insertPt.transformBy( xform );
	return Acad::eOk;
}

Acad::ErrorStatus SimpleWindLibraryDraw::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d& pickPoint,
	const AcGePoint3d& lastPoint,
	const AcGeMatrix3d& viewXform,
	AcGePoint3dArray& snapPoints,
	AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;
	// 只捕捉1种类型的点：插入点
	if( osnapMode != AcDb::kOsModeCen )
		return Acad::eOk;

	Acad::ErrorStatus es = Acad::eOk;

	if ( osnapMode == AcDb::kOsModeCen )
	{
		snapPoints.append( m_insertPt );
	}

	return es;
}

Acad::ErrorStatus SimpleWindLibraryDraw::subGetGripPoints (
	AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds
	) const
{
	assertReadEnabled () ;

	gripPoints.append( m_insertPt );

	return Acad::eOk;
}

Acad::ErrorStatus SimpleWindLibraryDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
	assertWriteEnabled () ;

	for( int i = 0; i < indices.length(); i++ )
	{
		int idx = indices.at( i );
		// 始节点
		if ( idx == 0 ) m_insertPt += offset;
	}
	return Acad::eOk;
}

void SimpleWindLibraryDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
}

void SimpleWindLibraryDraw::drawWindLibrary(AcGiWorldDraw* mode)
{
	 DrawRect( mode, m_insertPt, m_angle, m_width, m_height, false ); // 不填充
}