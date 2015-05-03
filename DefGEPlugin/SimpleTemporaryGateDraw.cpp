#include "StdAfx.h"
#include "SimpleTemporaryGateDraw.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleTemporaryGateDraw, SimpleGateDraw, 1 )

SimpleTemporaryGateDraw::SimpleTemporaryGateDraw () : SimpleGateDraw ()
{
}

SimpleTemporaryGateDraw::~SimpleTemporaryGateDraw ()
{
}

void SimpleTemporaryGateDraw::setAllExtraParamsToDefault()
{
    SimpleGateDraw::setAllExtraParamsToDefault();
    m_offset = 10;
}

void SimpleTemporaryGateDraw::configExtraParams()
{
    SimpleGateDraw::configExtraParams();
}

void SimpleTemporaryGateDraw::updateExtraParams()
{
    SimpleGateDraw::updateExtraParams();
}

void SimpleTemporaryGateDraw::readExtraParam( DrawParamReader& reader )
{
    SimpleGateDraw::readExtraParam( reader );
    reader.readDouble( m_offset );
}

void SimpleTemporaryGateDraw::writeExtraParam( DrawParamWriter& writer )
{
    SimpleGateDraw::writeExtraParam( writer );
    writer.writeDouble( m_offset );
}

Adesk::Boolean SimpleTemporaryGateDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    drawArc( mode, m_insertPt, m_angle, 0, m_radius );
    drawLine( mode, m_insertPt, m_angle, 0, m_offset, m_length );

    return Adesk::kTrue;
}
