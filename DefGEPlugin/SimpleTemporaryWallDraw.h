#pragma once

#include "SimpleWallDraw.h"

// ”¿æ√µ≤∑Á«Ω
class SimpleTemporaryWallDraw : public SimpleWallDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleTemporaryWallDraw) ;

public:
	SimpleTemporaryWallDraw () ;
	virtual ~SimpleTemporaryWallDraw () ;

	virtual void setAllExtraParamsToDefault();
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleTemporaryWallDraw)
#endif
