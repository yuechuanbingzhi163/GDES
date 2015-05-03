#include "StdAfx.h"
#include "SimpleWallCasementDraw.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleWallCasementDraw, SimpleCasementDraw, 1 )

SimpleWallCasementDraw::SimpleWallCasementDraw () : SimpleCasementDraw ()
{
}

SimpleWallCasementDraw::~SimpleWallCasementDraw ()
{
}

void SimpleWallCasementDraw::setAllExtraParamsToDefault()
{
    SimpleCasementDraw::setAllExtraParamsToDefault();
    m_width = 25;
    m_radius = 0;
    m_offset = 0;
}
