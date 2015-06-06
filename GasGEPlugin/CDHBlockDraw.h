#pragma once

#include "../MineGEDraw/BlockDraw.h"

class CDHBlockDraw : public BlockDraw 
{
public:
	ACRX_DECLARE_MEMBERS(CDHBlockDraw) ;

public:
	CDHBlockDraw();
	virtual void regPropertyDataNames(AcStringArray& names) const;
};

#ifdef GASGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CDHBlockDraw)
#endif
