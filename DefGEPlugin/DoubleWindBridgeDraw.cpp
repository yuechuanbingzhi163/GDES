#include "StdAfx.h"
#include "DoubleWindBridgeDraw.h"
#include "DoubleTunnelDraw_ConfigDlg.h"
#include "DrawTool.h"
#include <cmath>

ACRX_CONS_DEFINE_MEMBERS ( DoubleWindBridgeDraw, MineGEDraw, 1 )

DoubleWindBridgeDraw::DoubleWindBridgeDraw()
{
}

void DoubleWindBridgeDraw::setAllExtraParamsToDefault()
{
    m_width = 40;
    m_sideLength = 120;
    m_sideLength2 = 30;
    m_sideAngle = PI / 6;
    m_sideOffset = 20;
    m_linewidth = 4;
}

void DoubleWindBridgeDraw::configExtraParams()
{

}

void DoubleWindBridgeDraw::updateExtraParams()
{
}

void DoubleWindBridgeDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

void DoubleWindBridgeDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void DoubleWindBridgeDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_width );
    reader.readDouble( m_sideLength );
    reader.readDouble( m_sideLength2 );
    reader.readDouble( m_sideAngle );
    reader.readDouble( m_sideOffset );
    reader.readDouble( m_linewidth );
}

void DoubleWindBridgeDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_width );
    writer.writeDouble( m_sideLength );
    writer.writeDouble( m_sideLength2 );
    writer.writeDouble( m_sideAngle );
    writer.writeDouble( m_sideOffset );
    writer.writeDouble( m_linewidth );
}

AcGePoint3d DoubleWindBridgeDraw::caclSidePt( const AcGePoint3d& pt, const AcGeVector3d& baseV, bool clockwise ) const
{
    int c = ( clockwise ? -1 : 1 );

    // 计算侧边中点坐标
    AcGeVector3d v = baseV;
    v.rotateBy( c * PI / 2, AcGeVector3d::kZAxis );
    return pt + v * ( m_width / 2 + m_sideOffset );
}

void DoubleWindBridgeDraw::drawOneSide( AcGiWorldDraw* mode,
                                        const AcGeVector3d& baseV,
                                        const AcGePoint3d& pt,
                                        bool clockwise )
{
    // 计算侧边中点坐标
    AcGePoint3d spt = caclSidePt( pt, baseV, clockwise );

    // 绘制一半的侧边
    AcGePoint3d ept = spt + baseV * m_sideLength / 2;
    DrawPolyLine( mode, spt, ept, m_linewidth );

    int c = ( clockwise ? -1 : 1 );

    // 绘制倾斜的侧边
    AcGeVector3d v2 = baseV;
    v2.rotateBy( c * m_sideAngle, AcGeVector3d::kZAxis );
    DrawPolyLine( mode, ept, ept + v2 * m_sideLength2, m_linewidth );

    // 绘制另一半的侧边
    AcGeVector3d v3 = baseV;
    v3.rotateBy( PI, AcGeVector3d::kZAxis );
    ept = spt + v3 * m_sideLength / 2;
    DrawPolyLine( mode, spt, ept, m_linewidth );

    // 绘制另外一条倾斜的侧边
    v3.rotateBy( -1 * c * m_sideAngle, AcGeVector3d::kZAxis );
    DrawPolyLine( mode, ept, ept + v3 * m_sideLength2, m_linewidth );
}

void DoubleWindBridgeDraw::drawSide( AcGiWorldDraw* mode )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis );

    drawOneSide( mode, v, m_insertPt, false );
    drawOneSide( mode, v, m_insertPt, true );
}

Adesk::Boolean DoubleWindBridgeDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    // 绘制风桥
    drawSide( mode );

    return Adesk::kTrue;
}

Acad::ErrorStatus DoubleWindBridgeDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_insertPt.transformBy( xform );
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis );
    v.transformBy( xform );

    m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    return Acad::eOk;
}

Acad::ErrorStatus DoubleWindBridgeDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    Acad::ErrorStatus es = Acad::eOk;

    if( osnapMode == AcDb::kOsModeCen )
    {
        snapPoints.append( m_insertPt );
    }
    return es;
}

Acad::ErrorStatus DoubleWindBridgeDraw::subGetGripPoints ( AcGePoint3dArray& gripPoints,
        AcDbIntArray& osnapModes,
        AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_insertPt );

    return Acad::eOk;
}

Acad::ErrorStatus DoubleWindBridgeDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        if ( idx == 0 )
        {
            AcGeVector3d v( AcGeVector3d::kXAxis );
            v.rotateBy( m_angle, AcGeVector3d::kZAxis );

            m_insertPt += v.dotProduct( offset ) * v; // 计算偏移offset在angle方向上的投影
        }
    }
    return Acad::eOk;
}
