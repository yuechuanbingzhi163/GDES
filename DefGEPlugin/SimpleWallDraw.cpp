#include "StdAfx.h"
#include "SimpleWallDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleWallDraw, MineGEDraw, 1 )

SimpleWallDraw::SimpleWallDraw () : MineGEDraw ()
{
}

SimpleWallDraw::~SimpleWallDraw ()
{
}

void SimpleWallDraw::setAllExtraParamsToDefault()
{
    m_height = 80;
    m_width = 40;
    needFill = true;
}

void SimpleWallDraw::configExtraParams()
{

}

void SimpleWallDraw::updateExtraParams()
{

}

void SimpleWallDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

void SimpleWallDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void SimpleWallDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_width );
    reader.readDouble( m_height );
    reader.readBoolean( needFill );
}

void SimpleWallDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_width );
    writer.writeDouble( m_height );
    writer.writeBoolean( needFill );
}

Adesk::Boolean SimpleWallDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    // 绘制矩形块，并填充
    DrawRect( mode, m_insertPt, m_angle, m_width, m_height, needFill );

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleWallDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_insertPt.transformBy( xform ); // 变化插入点

    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis );
    v.transformBy( xform );

    m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis ); // 变换后的旋转角度

    return Acad::eOk;
}

//- Osnap points protocol
Acad::ErrorStatus SimpleWallDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // 只捕捉1种类型的点：中点
    if( osnapMode != AcDb::kOsModeCen ) return Acad::eOk;

    if( osnapMode == AcDb::kOsModeCen )
    {
        snapPoints.append( m_insertPt );
    }

    return Acad::eOk;
}

//- Grip points protocol
Acad::ErrorStatus SimpleWallDraw::subGetGripPoints ( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // 插入点作为夹点
    gripPoints.append( m_insertPt );

    return Acad::eOk;
}

Acad::ErrorStatus SimpleWallDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        // 插入点
        if ( idx == 0 ) m_insertPt += offset;
    }
    return Acad::eOk;
}

static AcGePoint3d CaclPt( const AcGePoint3d& insertPt, const AcGeVector3d& v1, double width, const AcGeVector3d& v2, double height )
{
    return ( insertPt + v1 * width / 2 + v2 * height / 2 );
}

void SimpleWallDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    v1.rotateBy( m_angle, AcGeVector3d::kZAxis );
    v2.rotateBy( m_angle + PI / 2, AcGeVector3d::kZAxis );

    pts.append( CaclPt( m_insertPt, v1, m_width, v2, m_height ) );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( m_insertPt, v1, m_width, v2, m_height ) );

    v2.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( m_insertPt, v1, m_width, v2, m_height ) );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( m_insertPt, v1, m_width, v2, m_height ) );
}