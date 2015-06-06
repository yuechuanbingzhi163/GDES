#pragma once

#include "../MineGEDraw/BlockDraw.h"

class GPPBlockDraw : public BlockDraw 
{
public:
	ACRX_DECLARE_MEMBERS(GPPBlockDraw) ;

public:
	GPPBlockDraw();
	virtual void regPropertyDataNames(AcStringArray& names) const;
};

#ifdef GASGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GPPBlockDraw)
#endif
