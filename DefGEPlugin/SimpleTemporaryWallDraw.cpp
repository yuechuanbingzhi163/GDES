#include "StdAfx.h"
#include "SimpleTemporaryWallDraw.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleTemporaryWallDraw, SimpleWallDraw, 1 )

SimpleTemporaryWallDraw::SimpleTemporaryWallDraw () : SimpleWallDraw ()
{
}

SimpleTemporaryWallDraw::~SimpleTemporaryWallDraw ()
{
}

void SimpleTemporaryWallDraw::setAllExtraParamsToDefault()
{
    SimpleWallDraw::setAllExtraParamsToDefault();
    needFill = false;
}
