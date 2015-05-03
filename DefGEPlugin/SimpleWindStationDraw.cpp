#include "StdAfx.h"
#include "SimpleWindStationDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleWindStationDraw, SimpleWallDraw, 1 )

SimpleWindStationDraw::SimpleWindStationDraw () : SimpleWallDraw ()
{
}

SimpleWindStationDraw::~SimpleWindStationDraw ()
{
}

void SimpleWindStationDraw::setAllExtraParamsToDefault()
{
    SimpleWallDraw::setAllExtraParamsToDefault();
    m_width = m_height = 80;
    needFill = false;

    m_lableAngle = PI * 0.25;
    m_lableLength = 180;
    m_hBaseLength = 300;
    m_textHeight = 30;
}

void SimpleWindStationDraw::readExtraParam( DrawParamReader& reader )
{
    SimpleWallDraw::readExtraParam( reader );
    reader.readDouble( m_lableAngle );
    reader.readDouble( m_lableLength );
    reader.readDouble( m_hBaseLength );
    reader.readDouble( m_textHeight );
}

void SimpleWindStationDraw::writeExtraParam( DrawParamWriter& writer )
{
    SimpleWallDraw::writeExtraParam( writer );
    writer.writeDouble( m_lableAngle );
    writer.writeDouble( m_lableLength );
    writer.writeDouble( m_hBaseLength );
    writer.writeDouble( m_textHeight );
}

void SimpleWindStationDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "测试断面面积" ) );
    names.append( _T( "测试风速" ) );
    names.append( _T( "测试风量" ) );
}

void SimpleWindStationDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    S = values[0].kACharPtr();
    V = values[1].kACharPtr();
    Q = values[2].kACharPtr();
}

static AcGePoint3d CaclLeftBottomPt( const AcGePoint3d& pt, double angle, double width, double height )
{
    AcGeVector3d v( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );
    v2.rotateBy( angle + PI / 2, AcGeVector3d::kZAxis );
    if( v.x > 0 ) // 位于第1和4象限
    {
        v.rotateBy( PI, AcGeVector3d::kZAxis );
        v2.rotateBy( PI, AcGeVector3d::kZAxis );
    }
    return ( pt + v * width / 2 + v2 * height / 2 );
}

static void DrawV( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double angle, double height )
{
    // 额外绘制一个"V"
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );
    if( v.x < 0 )
    {
        DrawMText( mode, insertPt, angle + PI, _T( "V" ), height * 0.8 );
    }
    else
    {
        DrawMText( mode, insertPt, angle, _T( "V" ), height * 0.8 );
    }
}

static void CaclLabelPts( const AcGePoint3d& insertPt, double angle, double length, double hBaseLength, AcGePoint3d& pt1, AcGePoint3d& pt2, int& dir )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );

    pt1 = insertPt + v * length;

    dir = ( v.x > 0 ? 1 : -1 ); // 第1和第4象限，x轴正方向;第2和第3象限，x轴反方向

    pt2 = pt1 + AcGeVector3d::kXAxis * dir * hBaseLength;
}

static void DrawLabelText( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double height, const CString& S, const CString& V, const CString& Q )
{
    double s = _tstof( S );
    double v = _tstof( V );
    // q = s*v*60
    double q = _tstof( Q );

    // {\\H0.7x;\\S2^;}表示上标xx^2
    CString str;

    str.Format( _T( "Q = %.2f m%s/min" ), s * v * 60, MakeUpperText( _T( "3" ) ) ); // m^3/min
    DrawMText( mode, insertPt, 0, str, height, AcDbMText::kBottomLeft );

    AcGeVector3d vec( AcGeVector3d::kYAxis );

    double gap = 10;
    double offset = height + gap;

    // 绘制V
    str.Format( _T( "V = %.2f m/s" ), v ); // m/s
    DrawMText( mode, insertPt + vec * offset, 0, str, height, AcDbMText::kBottomLeft );

    // 绘制S
    str.Format( _T( "S = %.2f m%s" ), s, MakeUpperText( _T( "2" ) ) ); // m^2
    DrawMText( mode, insertPt + vec * offset * 2, 0, str, height, AcDbMText::kBottomLeft );
}

Adesk::Boolean SimpleWindStationDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    // 绘制一个矩形框
    DrawRect( mode, m_insertPt, m_angle, m_width, m_height, false ); // 不填充

    // 绘制文字:V
    //DrawV(mode, m_insertPt, m_angle, m_height);

    // 绘制文字标注,显示S、V、Q
    AcGePoint3d pt1, pt2;
    int dir;
    CaclLabelPts( m_insertPt, m_lableAngle, m_lableLength, m_hBaseLength, pt1, pt2, dir );
    DrawLine( mode, m_insertPt, pt1 );
    DrawLine( mode, pt1, pt2 );
    DrawLabelText( mode, ( dir > 0 ? pt1 : pt2 ), m_textHeight, S, V, Q );

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleWindStationDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // 插入点作为夹点
    gripPoints.append( m_insertPt );

    AcGePoint3d pt1, pt2;
    int dir;
    CaclLabelPts( m_insertPt, m_lableAngle, m_lableLength, m_hBaseLength, pt1, pt2, dir );
    gripPoints.append( pt1 );
    gripPoints.append( pt2 );

    return Acad::eOk;
}

Acad::ErrorStatus SimpleWindStationDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        // 插入点
        if ( idx == 0 ) m_insertPt += offset;
        if( idx == 1 )
        {
            AcGePoint3d pt1, pt2;
            int dir;
            CaclLabelPts( m_insertPt, m_lableAngle, m_lableLength, m_hBaseLength, pt1, pt2, dir );

            pt1 += offset; // 偏移拉伸
            AcGeVector3d v = pt1 - m_insertPt;
            m_lableLength = v.length();
            m_lableAngle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
        }
        if( idx == 2 )
        {
            AcGePoint3d pt1, pt2;
            int dir;
            CaclLabelPts( m_insertPt, m_lableAngle, m_lableLength, m_hBaseLength, pt1, pt2, dir );

            pt2.x += offset.x; // 只考虑水平方向
            AcGeVector3d v = pt2 - pt1;
            m_hBaseLength = v.length();
        }
    }
    return Acad::eOk;
}