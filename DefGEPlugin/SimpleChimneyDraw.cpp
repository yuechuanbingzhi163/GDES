#include "StdAfx.h"
#include "SimpleChimneyDraw.h"

#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleChimneyDraw, MineGEDraw, 1 )

SimpleChimneyDraw::SimpleChimneyDraw () : MineGEDraw ()
{
}

SimpleChimneyDraw::~SimpleChimneyDraw ()
{
}

void SimpleChimneyDraw::setAllExtraParamsToDefault()
{
    m_length = 40;
    m_width = 5;
    m_space = 10;
    m_lineWidth = 0;
}

void SimpleChimneyDraw::configExtraParams()
{

}

void SimpleChimneyDraw::updateExtraParams()
{

}

void SimpleChimneyDraw::writeKeyParam( DrawParamWriter& writer )
{
    int len = m_pts.length();
    writer.writeInt( len );
    for( int i = 0; i < len; i++ )
    {
        writer.writePoint( m_pts[i] );
    }
}

void SimpleChimneyDraw::readKeyParam( DrawParamReader& reader )
{
    m_pts.removeAll();

    int len = 0;
    reader.readInt( len );
    for( int i = 0; i < len; i++ )
    {
        AcGePoint3d pt;
        reader.readPoint( pt );
        m_pts.append( pt );
    }
}

void SimpleChimneyDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_length );
    reader.readDouble( m_width );
    reader.readDouble( m_space );
    reader.readDouble( m_lineWidth );
}

void SimpleChimneyDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_length );
    writer.writeDouble( m_width );
    writer.writeDouble( m_space );
    writer.writeDouble( m_lineWidth );
}

static void DrawSegment( AcGiWorldDraw* mode, const AcGePoint3d& spt, const AcGeVector3d& v, double length, double width, double lineWidth )
{
    AcGePoint3d ept = spt + v * length;
    DrawPolyLine( mode, spt, ept, lineWidth );

    AcGeVector3d vv = v;
    vv.rotateBy( PI / 2, AcGeVector3d::kZAxis );
    AcGePoint3d spt1, spt2;
    spt1 = spt + vv * width * 0.5;
    vv.rotateBy( PI, AcGeVector3d::kZAxis );
    spt2 = spt + vv * width * 0.5;
    DrawPolyLine( mode, spt1, spt2, lineWidth );

    AcGePoint3d ept1, ept2;
    ept1 = ept + vv * width * 0.5;
    vv.rotateBy( PI, AcGeVector3d::kZAxis );
    ept2 = ept + vv * width * 0.5;
    DrawPolyLine( mode, ept1, ept2, lineWidth );
}

void SimpleChimneyDraw::drawSegment( AcGiWorldDraw* mode, const AcGePoint3d& spt, const AcGePoint3d& ept )
{
    AcGeVector3d v = ept - spt;
    int n = ( int )( ( v.length() + m_space ) / ( m_length + m_space ) );
    //acutPrintf(_T("\n可绘制的个数:%d"), n);
    v.normalize();

    AcGePoint3d pt = spt;
    for( int i = 0; i < n; i++ )
    {
        DrawSegment( mode, pt, v, m_length, m_width, m_lineWidth );
        pt = pt + v * ( m_length + m_space );
    }
    double ll = ( ept - pt ).length();
    if( ll > m_length * 0.5 ) // 如果有长度的50%，则绘制一小段
    {
        DrawSegment( mode, pt, v, ll, m_width, m_lineWidth );
    }
}

Adesk::Boolean SimpleChimneyDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    int len = m_pts.length();
    if( len < 2 ) return Adesk::kTrue;

    for( int i = 0; i < len - 1; i++ )
    {
        drawSegment( mode, m_pts[i], m_pts[i + 1] );
    }
    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleChimneyDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    int len = m_pts.length();
    for( int i = 0; i < len; i++ )
    {
        m_pts[i].transformBy( xform );
    }
    return Acad::eOk;
}

Acad::ErrorStatus SimpleChimneyDraw::subGetOsnapPoints (
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
    if( osnapMode != AcDb::kOsMaskEnd ) return Acad::eOk;

    if( osnapMode == AcDb::kOsMaskEnd )
    {
        int len = m_pts.length();
        for( int i = 0; i < len; i++ )
        {
            snapPoints.append( m_pts[i] );
        }
    }

    return Acad::eOk;
}

//- Grip points protocol
Acad::ErrorStatus SimpleChimneyDraw::subGetGripPoints (
    AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    int len = m_pts.length();
    for( int i = 0; i < len; i++ )
    {
        gripPoints.append( m_pts[i] );
    }

    return Acad::eOk;
}

Acad::ErrorStatus SimpleChimneyDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );

        m_pts[idx] += offset;
    }
    return Acad::eOk;
}
