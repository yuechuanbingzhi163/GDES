#include "StdAfx.h"
#include "DrillGEDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( DrillGEDraw, LinkedGEDraw, 1 )

DrillGEDraw::DrillGEDraw()
{
}

DrillGEDraw::~DrillGEDraw( void )
{
}

void DrillGEDraw::setAllExtraParamsToDefault()
{

}

void DrillGEDraw::configExtraParams()
{

}

void DrillGEDraw::updateExtraParams()
{

}

void DrillGEDraw::readExtraParam( DrawParamReader& reader )
{
	// do nothing
}

void DrillGEDraw::writeExtraParam( DrawParamWriter& writer )
{
	// do nothing
}

void DrillGEDraw::extendByLength( double length )
{
	AcGeVector3d v = m_endPt - m_startPt;
	v.normalize();

	m_endPt = m_endPt + v * length; // 修改末点坐标
}

Adesk::Boolean DrillGEDraw::subWorldDraw( AcGiWorldDraw* mode )
{
	assertReadEnabled () ;

	AcGePoint3dArray pts;
	pts.append( m_startPt );
	pts.append( m_endPt );
	mode->geometry().worldLine( pts.asArrayPtr() );
	//DrawPolyLine( mode, m_startPt, m_endPt, 10 );

	return Adesk::kTrue;
}

//----- AcDbEntity protocols
Acad::ErrorStatus DrillGEDraw::subTransformBy( const AcGeMatrix3d& xform )
{
	m_startPt.transformBy( xform );
	m_endPt.transformBy( xform );

	return Acad::eOk;
}

//- Osnap points protocol
Acad::ErrorStatus DrillGEDraw::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d& pickPoint,
	const AcGePoint3d& lastPoint,
	const AcGeMatrix3d& viewXform,
	AcGePoint3dArray& snapPoints,
	AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;

	// 只捕捉1种类型的点：端点
	if( osnapMode != AcDb::kOsModeEnd )
		return Acad::eOk;

	Acad::ErrorStatus es = Acad::eOk;

	if ( osnapMode == AcDb::kOsModeEnd )
	{
		snapPoints.append( m_startPt );
		snapPoints.append( m_endPt );
	}

	return es;
}

//- Grip points protocol
Acad::ErrorStatus DrillGEDraw::subGetGripPoints (
	AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;
	//----- This method is never called unless you return eNotImplemented
	//----- from the new getGripPoints() method below (which is the default implementation)

	gripPoints.append( m_startPt );
	gripPoints.append( m_endPt );
	return Acad::eOk;
}

Acad::ErrorStatus DrillGEDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
	assertWriteEnabled () ;
	//----- This method is never called unless you return eNotImplemented
	//----- from the new moveGripPointsAt() method below (which is the default implementation)

	for( int i = 0; i < indices.length(); i++ )
	{
		int idx = indices.at( i );
		// 始节点
		if ( idx == 0 ) m_startPt += offset;

		if ( idx == 1 ) m_endPt += offset;
	}
	return Acad::eOk;
}

Acad::ErrorStatus DrillGEDraw::subGetGeomExtents( AcDbExtents& extents ) const
{
	assertReadEnabled();

	extents.addPoint( m_startPt );
	extents.addPoint( m_endPt );

	return Acad::eOk;
}