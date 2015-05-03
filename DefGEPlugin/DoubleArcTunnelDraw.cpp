#include "StdAfx.h"
#include "DoubleArcTunnelDraw.h"

ACRX_CONS_DEFINE_MEMBERS ( DoubleArcTunnelDraw, LinkedGEDraw, 1 )

DoubleArcTunnelDraw::DoubleArcTunnelDraw( void )
{
}

void DoubleArcTunnelDraw::setAllExtraParamsToDefault()
{
    m_width = 40;
}

void DoubleArcTunnelDraw::updateExtraParams()
{
    update();
}

void DoubleArcTunnelDraw::configExtraParams()
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
    reader.readPoint( m_inStartPt );
    reader.readPoint( m_inEndPt );
    reader.readPoint( m_outStartPt );
    reader.readPoint( m_outEndPt );
}

void DoubleArcTunnelDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_width );
    writer.writePoint( m_inStartPt );
    writer.writePoint( m_inEndPt );
    writer.writePoint( m_outStartPt );
    writer.writePoint( m_outEndPt );
}

static void DividArc( const AcGePoint3d& spt, const AcGePoint3d& ept, const AcGePoint3d& thirdPt,
                      int count, AcGePoint3dArray& pts )
{
    pts.append( spt );

    AcGeCircArc3d arc( spt, thirdPt, ept );
    AcGePoint3d cnt = arc.center();
//	double radius = arc.radius();
    AcGeVector3d v1 = spt - cnt;
    AcGeVector3d v2 = ept - cnt;
    int c = ( arc.normal().z > 0 ? 1 : -1 ); // 弧线方向

    double a1 = v1.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
    double a2 = v2.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    double dq = abs( a2 - a1 ) / count;
    //acutPrintf(_T("\na1:%.3f, a2:%.3f dq:%.3f c:%d"), a1, a2, dq, c);
    for( int i = 1; i < count; i++ )
    {
        v1.rotateBy( c * dq, AcGeVector3d::kZAxis );
        pts.append( cnt + v1 );
    }
    pts.append( ept );
}

void DoubleArcTunnelDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    AcGeCircArc3d arc( m_startPt, m_thirdPt, m_endPt );
    AcGePoint3d cnt = arc.center();
    double radius = arc.radius();

    AcGeVector3d v = m_thirdPt - cnt;
    v.normalize();

    pts.append( m_startPt );
    DividArc( m_inStartPt, m_inEndPt, cnt + v * ( radius - m_width * 0.5 ), 90, pts );

    pts.append( m_endPt );
    DividArc( m_outEndPt, m_outStartPt, cnt + v * ( radius + m_width * 0.5 ), 90, pts );
}

AcGeVector3d DoubleArcTunnelDraw::getStartPointInExtendAngle() const
{
    AcGeCircArc3d arc( m_startPt, m_thirdPt, m_endPt );
    AcGePoint3d cnt = arc.center();
    AcGeVector3d v = m_startPt - cnt;

    int c = ( arc.normal().z > 0 ? 1 : -1 );
    v.rotateBy( c * PI / 2, AcGeVector3d::kZAxis );

    return v;
}

AcGeVector3d DoubleArcTunnelDraw::getEndPointInExtendAngle() const
{
    AcGeCircArc3d arc( m_startPt, m_thirdPt, m_endPt );
    AcGePoint3d cnt = arc.center();
    AcGeVector3d v = m_endPt - cnt;

    int c = ( arc.normal().z > 0 ? -1 : 1 );
    v.rotateBy( c * PI / 2, AcGeVector3d::kZAxis );

    return v;
}

void DoubleArcTunnelDraw::dealWithStartPointBoundary( const AcGeRay3d& boundaryLine )
{
    AcGeCircArc3d arc0( m_startPt, m_thirdPt, m_endPt );
    AcGePoint3d cnt = arc0.center();
    double radius = arc0.radius();

    AcGeVector3d v = m_startPt - cnt;
    AcGeVector3d v2 = m_endPt - m_startPt;
    v.normalize();

    AcGeVector3d v3 = v.crossProduct( v2 );
    int c = ( v3.z > 0 ? 1 : -1 );
    v.rotateBy( c * PI / 2, AcGeVector3d::kZAxis );

    AcGePoint3d pt;

    AcGeLine3d line( m_inStartPt, v );
    if( Adesk::kTrue == line.intersectWith( boundaryLine, pt ) )
    {
        //acutPrintf(_T("\n内环相交"));
        m_inStartPt = pt;
    }

    line.set( m_outStartPt, v );
    if( Adesk::kTrue == line.intersectWith( boundaryLine, pt ) )
    {
        //acutPrintf(_T("\n外环相交"));
        m_outStartPt = pt;
    }
}

void DoubleArcTunnelDraw::dealWithEndPointBoundary( const AcGeRay3d& boundaryLine )
{
    AcGeCircArc3d arc0( m_startPt, m_thirdPt, m_endPt );
    AcGePoint3d cnt = arc0.center();
    double radius = arc0.radius();

    AcGeVector3d v = m_endPt - cnt;
    AcGeVector3d v2 = m_startPt - m_endPt;
    v.normalize();

    AcGeVector3d v3 = v.crossProduct( v2 );
    int c = ( v3.z > 0 ? 1 : -1 );
    v.rotateBy( c * PI / 2, AcGeVector3d::kZAxis );

    AcGePoint3d pt;

    AcGeLine3d line( m_inEndPt, v );
    if( Adesk::kTrue == line.intersectWith( boundaryLine, pt ) )
    {
        m_inEndPt = pt;
    }

    line.set( m_outEndPt, v );
    if( Adesk::kTrue == line.intersectWith( boundaryLine, pt ) )
    {
        m_outEndPt = pt;
    }
}

void DoubleArcTunnelDraw::extendByLength( double length )
{
    // 存在错误!!!
    AcGeVector3d v = m_endPt - m_startPt;
    v.normalize();

    m_endPt = m_endPt + v * length; // 更新末点坐标
    update(); // 更新其它参数
}

void DoubleArcTunnelDraw::update()
{
    caclInPoint( m_inStartPt, m_outStartPt );
    caclOutPoint( m_inEndPt, m_outEndPt );
}

void DoubleArcTunnelDraw::caclInPoint( AcGePoint3d& startPt1, AcGePoint3d& startPt2 )
{
    AcGeCircArc3d arc( m_startPt, m_thirdPt, m_endPt );
    AcGePoint3d cenPt = arc.center(); // 圆心
    double radius = arc.radius();

    AcGeVector3d v = m_startPt - cenPt;
    v.normalize();

    startPt1 = cenPt + v * ( radius - m_width / 2 );
    startPt2 = cenPt + v * ( radius + m_width / 2 );
}

void DoubleArcTunnelDraw::caclOutPoint( AcGePoint3d& endPt1, AcGePoint3d& endPt2 )
{
    AcGeCircArc3d arc( m_startPt, m_thirdPt, m_endPt );
    AcGePoint3d cenPt = arc.center(); // 圆心
    double radius = arc.radius();

    AcGeVector3d v = m_endPt - cenPt;
    v.normalize();

    endPt1 = cenPt + v * ( radius - m_width / 2 );
    endPt2 = cenPt + v * ( radius + m_width / 2 );
}

void DoubleArcTunnelDraw::drawArc( AcGiWorldDraw* mode, AcGePoint3d& startPt, AcGePoint3d& endPt, AcGePoint3d& thirdPt )
{
    mode->geometry().circularArc( startPt, thirdPt, endPt );
}

Adesk::Boolean DoubleArcTunnelDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    AcGeCircArc3d arc( m_startPt, m_thirdPt, m_endPt );
    AcGePoint3d cnt = arc.center();
    double radius = arc.radius();
    AcGeVector3d v = m_thirdPt - cnt;
    v.normalize();

    drawArc( mode, m_inStartPt, m_inEndPt, cnt + v * ( radius - m_width * 0.5 ) ); // 绘制外弧
    drawArc( mode, m_outStartPt, m_outEndPt, cnt + v * ( radius + m_width * 0.5 ) ); // 绘制内弧

    return Adesk::kTrue;
}

Acad::ErrorStatus DoubleArcTunnelDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_startPt.transformBy( xform );
    m_endPt.transformBy( xform );
    m_thirdPt.transformBy( xform );
    update();

    return Acad::eOk;
}

Acad::ErrorStatus DoubleArcTunnelDraw::subGetOsnapPoints (
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
        snapPoints.append( m_thirdPt ); // 弧中点
    }

    return es;
}

//- Grip points protocol
Acad::ErrorStatus DoubleArcTunnelDraw::subGetGripPoints (
    AcGePoint3dArray& gripPoints,
    AcDbIntArray& osnapModes,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_startPt );
    gripPoints.append( m_endPt );
    gripPoints.append( m_thirdPt );

    return Acad::eOk;
}

Acad::ErrorStatus DoubleArcTunnelDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );

        if ( idx == 0 )
        {
            m_startPt += offset;
            update();
        }

        if ( idx == 1 )
        {
            m_endPt += offset;
            update();
        }

        if( idx == 2 )
        {
            AcGeCircArc3d arc( m_startPt, m_thirdPt, m_endPt );
            int c = ( arc.normal().z > 0 ? -1 : 1 );

            m_thirdPt += offset;

            if( m_thirdPt == m_startPt || m_thirdPt == m_endPt )
            {
                AcGeVector3d v = m_endPt - m_startPt;
                AcGePoint3d cnt = m_startPt + v * 0.5;
                v.rotateBy( c * PI / 2, AcGeVector3d::kZAxis ); // 按照原来弧线的反方向旋转
                m_thirdPt = cnt + v * 0.5;
            }
            update();
        }
    }
    return Acad::eOk;
}