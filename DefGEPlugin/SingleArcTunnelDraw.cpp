#include "StdAfx.h"
#include "DoubleArcTunnelDraw.h"
#include <cmath>

// 使用ACRX_CONS_DEFINE_MEMBERS即可满足需要
// 不需要dxf、dwg类的宏
// 版本号随便写一个即可
ACRX_CONS_DEFINE_MEMBERS ( DoubleArcTunnelDraw, LinkedGEDraw, 1 )

DoubleArcTunnelDraw::DoubleArcTunnelDraw()
{
}

DoubleArcTunnelDraw::~DoubleArcTunnelDraw( void )
{
}

void DoubleArcTunnelDraw::setAllExtraParamsToDefault()
{
    m_width = 40;
}

void DoubleArcTunnelDraw::configExtraParams()
{

}

void DoubleArcTunnelDraw::updateExtraParams()
{

}
void DoubleArcTunnelDraw::readKeyParam( DrawParamReader& reader )
{
    LinkedGEDraw::readKeyParam( reader );
    reader.readPoint( m_thirdPt );
}

void DoubleArcTunnelDraw::writeKeyParam( DrawParamWriter& writer )
{
    LinkedGEDraw::writeKeyParam( writer );
    writer.writePoint( m_thirdPt );
}

void DoubleArcTunnelDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_width );
}

void DoubleArcTunnelDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_width );
}

Adesk::Boolean DoubleArcTunnelDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    AcGiSubEntityTraits& traits = mode->subEntityTraits();
    AcDb::LineWeight lw = traits.lineWeight();

    traits.setLineWeight( ( AcDb::LineWeight )( int )m_width );
    mode->geometry().circularArc( m_startPt, m_thirdPt, m_endPt );

    traits.setLineWeight( lw );

    return Adesk::kTrue;
}

Acad::ErrorStatus DoubleArcTunnelDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    AcGeCircArc3d arc( m_startPt, m_thirdPt, m_endPt );
    arc.transformBy( xform );
    return Acad::eOk;
}

Acad::ErrorStatus DoubleArcTunnelDraw::subGetOsnapPoints(
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled();

    // 增加捕捉1种类型的点：中点
    if( osnapMode != AcDb::kOsModeMid
            && osnapMode != AcDb::kOsModeEnd )
        return Acad::eOk;

    if ( osnapMode == AcDb::kOsModeEnd )
    {
        snapPoints.append( m_startPt );
        snapPoints.append( m_endPt );
        snapPoints.append( m_thirdPt );
    }

    return Acad::eOk;
}

Acad::ErrorStatus DoubleArcTunnelDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_startPt );
    gripPoints.append( m_endPt );
    gripPoints.append( m_thirdPt );

    return Acad::eOk;
}

Acad::ErrorStatus DoubleArcTunnelDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        if ( idx == 0 )
        {
            m_startPt += offset;
        }

        if ( idx == 1 )
        {
            m_endPt += offset;
        }

        // 弧的中点
        if ( idx == 2 )
        {
            m_thirdPt += offset;
        }
    }
    return Acad::eOk;
}

AcGeVector3d DoubleArcTunnelDraw::getStartPointInExtendAngle() const
{
    AcGeCircArc3d arc( m_startPt, m_thirdPt, m_endPt );
    AcGePoint3d cenPt = arc.center();
    AcGeVector3d v = m_startPt - cenPt;
    AcGeVector3d v2 = m_endPt - m_startPt;
    AcGeVector3d v3 = v.crossProduct( v2 );
    int c = ( v3.z > 0 ? 1 : -1 );
    v.rotateBy( c * PI / 2, AcGeVector3d::kZAxis );
    return v;
}

AcGeVector3d DoubleArcTunnelDraw::getEndPointInExtendAngle() const
{
    AcGeCircArc3d arc( m_startPt, m_thirdPt, m_endPt );
    AcGePoint3d cenPt = arc.center();
    AcGeVector3d v = m_endPt - cenPt;
    AcGeVector3d v2 = m_startPt - m_endPt;
    AcGeVector3d v3 = v.crossProduct( v2 );
    int c = ( v3.z > 0 ? 1 : -1 );
    v.rotateBy( c * PI / 2, AcGeVector3d::kZAxis );
    return v;
}

void DoubleArcTunnelDraw::extendByLength( double length )
{
    AcGeCircArc3d arc( m_startPt, m_thirdPt, m_endPt );
    AcGePoint3d cenPt = arc.center();
    double radius = arc.radius();
    AcGeVector3d v = m_endPt - cenPt;
    AcGeVector3d v2 = m_startPt - m_endPt;
    AcGeVector3d v3 = v.crossProduct( v2 );
    int c = ( v3.z > 0 ? 1 : -1 );

    v.rotateBy( c * length / radius, AcGeVector3d::kZAxis );

    m_endPt = cenPt + v; // 修改圆弧的末点
}