#include "StdAfx.h"
#include "SimpleJointDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleJointDraw, MineGEDraw, 1 )

SimpleJointDraw::SimpleJointDraw () : MineGEDraw ()
{
}

SimpleJointDraw::~SimpleJointDraw ()
{
}

void SimpleJointDraw::setAllExtraParamsToDefault()
{
    m_radius = 20;
	//m_distance = 16;
}

void SimpleJointDraw::configExtraParams()
{

}

void SimpleJointDraw::updateExtraParams()
{

}

void SimpleJointDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_insertPt );
    //writer.writeDouble( m_angle );
}

void SimpleJointDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    //reader.readDouble( m_angle );
}

void SimpleJointDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_radius );
}

void SimpleJointDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_radius );
}

Adesk::Boolean SimpleJointDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;
	drawJoint(mode);
    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleJointDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_insertPt.transformBy( xform );
    return Acad::eOk;
}

Acad::ErrorStatus SimpleJointDraw::subGetOsnapPoints (
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

Acad::ErrorStatus SimpleJointDraw::subGetGripPoints (
    AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds
) const
{
    assertReadEnabled () ;

    gripPoints.append( m_insertPt );

    return Acad::eOk;
}

Acad::ErrorStatus SimpleJointDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
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

void SimpleJointDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
}

void SimpleJointDraw::drawJoint(AcGiWorldDraw* mode)
{
	DrawCircle(mode,m_insertPt,m_radius,false);
}