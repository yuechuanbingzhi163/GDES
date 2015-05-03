#include "StdAfx.h"
#include "SimpleBalanceGateDraw.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleBalanceGateDraw, SimpleGateDraw, 1 )

SimpleBalanceGateDraw::SimpleBalanceGateDraw () : SimpleGateDraw ()
{
}

SimpleBalanceGateDraw::~SimpleBalanceGateDraw ()
{
}

void SimpleBalanceGateDraw::setAllExtraParamsToDefault()
{
    SimpleGateDraw::setAllExtraParamsToDefault();
    m_gap = 50; // 圆半径40+偏移10
}

void SimpleBalanceGateDraw::configExtraParams()
{
    SimpleGateDraw::configExtraParams();
}

void SimpleBalanceGateDraw::updateExtraParams()
{
    SimpleGateDraw::updateExtraParams();
}

void SimpleBalanceGateDraw::readExtraParam( DrawParamReader& reader )
{
    SimpleGateDraw::readExtraParam( reader );
    reader.readDouble( m_gap );
}

void SimpleBalanceGateDraw::writeExtraParam( DrawParamWriter& writer )
{
    SimpleGateDraw::writeExtraParam( writer );
    writer.writeDouble( m_gap );
}

void SimpleBalanceGateDraw::drawParabola( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double angle, double gap, double radius, bool clockWise )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle + PI / 2, AcGeVector3d::kZAxis );

    drawArc( mode, insertPt + v * radius, angle, gap, radius, clockWise );

    v.rotateBy( PI, AcGeVector3d::kZAxis );
    drawArc( mode, insertPt + v * radius, angle + PI, gap, radius, clockWise );
}

Adesk::Boolean SimpleBalanceGateDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    // 1) 绘制2个抛物线,间距gap
    /*
    drawParabola(mode, m_insertPt, m_angle, m_gap, m_radius, false);
    drawLine(mode, m_insertPt, m_angle, m_gap, 0, m_length+m_radius);

    drawParabola(mode, m_insertPt, m_angle+PI, m_gap, m_radius, true);
    drawLine(mode, m_insertPt, m_angle+PI, m_gap, 0, m_length+m_radius);*/

    // 2) 绘制1个抛物线, 间距gap=0
    drawParabola( mode, m_insertPt, m_angle, 0, m_radius, false );
    drawLine( mode, m_insertPt, m_angle, 0, 0, m_length + m_radius );

    return Adesk::kTrue;
}


static AcGePoint3d CaclPt( const AcGePoint3d& insertPt, const AcGeVector3d& v1, double width, const AcGeVector3d& v2, double height )
{
    return ( insertPt + v1 * width + v2 * height );
}

static double CaclWidth( double w, double dw )
{
    double width = w;
    if( w > dw / 2 )
    {
        width = sqrt( w * w - dw * dw / 4 );
    }
    return width;
}

void SimpleBalanceGateDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    /*AcGeVector3d v1(AcGeVector3d::kXAxis), v2(AcGeVector3d::kXAxis);
    v1.rotateBy(m_angle, AcGeVector3d::kZAxis);
    v2.rotateBy(m_angle+PI/2, AcGeVector3d::kZAxis);

    double width = CaclWidth(m_radius, m_dTWidth)+m_gap;

    pts.append(CaclPt(m_insertPt, v1, width, v2, m_length));

    v1.rotateBy(PI, AcGeVector3d::kZAxis);
    pts.append(CaclPt(m_insertPt, v1, width, v2, m_length));

    v2.rotateBy(PI, AcGeVector3d::kZAxis);
    pts.append(CaclPt(m_insertPt, v1, width, v2, m_length));

    v1.rotateBy(PI, AcGeVector3d::kZAxis);
    pts.append(CaclPt(m_insertPt, v1, width, v2, m_length));*/
}