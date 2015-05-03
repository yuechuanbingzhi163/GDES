#include "StdAfx.h"
#include "SimpleTemporaryCasementDraw.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleTemporaryCasementDraw, SimpleCasementDraw, 1 )

SimpleTemporaryCasementDraw::SimpleTemporaryCasementDraw () : SimpleCasementDraw ()
{
}

SimpleTemporaryCasementDraw::~SimpleTemporaryCasementDraw ()
{
}

void SimpleTemporaryCasementDraw::setAllExtraParamsToDefault()
{
    SimpleCasementDraw::setAllExtraParamsToDefault();
    m_offset = 10;
}
