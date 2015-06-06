#pragma once

#include "../MineGEDraw/BlockDraw.h"

class WGFPBlockDraw : public BlockDraw 
{
public:
	ACRX_DECLARE_MEMBERS(WGFPBlockDraw) ;

public:
	WGFPBlockDraw();
	virtual void regPropertyDataNames(AcStringArray& names) const;
};

class TGFPBlockDraw : public BlockDraw 
{
public:
	ACRX_DECLARE_MEMBERS(TGFPBlockDraw) ;

public:
	TGFPBlockDraw();
	virtual void regPropertyDataNames(AcStringArray& names) const;
};

#ifdef GASGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(WGFPBlockDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TGFPBlockDraw)
#endif
