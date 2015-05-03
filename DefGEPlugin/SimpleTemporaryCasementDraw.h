#pragma once

#include "SimpleCasementDraw.h"

// 临时风门墙调风窗
class SimpleTemporaryCasementDraw : public SimpleCasementDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleTemporaryCasementDraw) ;

public:
	SimpleTemporaryCasementDraw () ;
	virtual ~SimpleTemporaryCasementDraw () ;

	virtual void setAllExtraParamsToDefault();
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleTemporaryCasementDraw)
#endif
