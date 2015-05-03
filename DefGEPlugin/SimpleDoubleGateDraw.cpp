#include "StdAfx.h"
#include "SimpleDoubleGateDraw.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleDoubleGateDraw, SimpleGateDraw, 1 )

SimpleDoubleGateDraw::SimpleDoubleGateDraw () : SimpleGateDraw ()
{
}

SimpleDoubleGateDraw::~SimpleDoubleGateDraw ()
{
}

void SimpleDoubleGateDraw::setAllExtraParamsToDefault()
{
    SimpleGateDraw::setAllExtraParamsToDefault();
    m_gap = 10;
    //m_radius = 32;
}

void SimpleDoubleGateDraw::configExtraParams()
{
    SimpleGateDraw::configExtraParams();
}

void SimpleDoubleGateDraw::updateExtraParams()
{
    SimpleGateDraw::updateExtraParams();
}

void SimpleDoubleGateDraw::readExtraParam( DrawParamReader& reader )
{
    SimpleGateDraw::readExtraParam( reader );
    reader.readDouble( m_gap );
}

void SimpleDoubleGateDraw::writeExtraParam( DrawParamWriter& writer )
{
    SimpleGateDraw::writeExtraParam( writer );
    writer.writeDouble( m_gap );
}

Adesk::Boolean SimpleDoubleGateDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    drawArc( mode, m_insertPt, m_angle, m_gap, m_radius );
    drawLine( mode, m_insertPt, m_angle, m_gap, 0, m_length );

    drawArc( mode, m_insertPt, m_angle + PI, m_gap, m_radius );
    drawLine( mode, m_insertPt, m_angle + PI, m_gap, 0, m_length );

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

void SimpleDoubleGateDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    v1.rotateBy( m_angle, AcGeVector3d::kZAxis );
    v2.rotateBy( m_angle + PI / 2, AcGeVector3d::kZAxis );

    double width = CaclWidth( m_radius, m_dTWidth ) + m_gap;

    pts.append( CaclPt( m_insertPt, v1, width, v2, m_length ) );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( m_insertPt, v1, width, v2, m_length ) );

    v2.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( m_insertPt, v1, width, v2, m_length ) );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( m_insertPt, v1, width, v2, m_length ) );
}