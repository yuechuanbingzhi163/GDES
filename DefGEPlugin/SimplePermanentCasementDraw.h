#pragma once

#include "SimpleCasementDraw.h"

// 永久风门墙调风窗
class SimplePermanentCasementDraw : public SimpleCasementDraw {

public:
	ACRX_DECLARE_MEMBERS(SimplePermanentCasementDraw) ;

public:
	SimplePermanentCasementDraw () ;
	virtual ~SimplePermanentCasementDraw () ;

	virtual void setAllExtraParamsToDefault();
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimplePermanentCasementDraw)
#endif
