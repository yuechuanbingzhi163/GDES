#pragma once

#include "MineGEDraw.h"

// 一个空的Draw实现(特殊用途)
class MINEGE_DRAW_DLLIMPEXP NullGEDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(NullGEDraw);

public:
	NullGEDraw(void);
	~NullGEDraw(void);

	virtual void setAllExtraParamsToDefault() {}
	virtual void configExtraParams() {}
	virtual void updateExtraParams() {}

	virtual void readKeyParam(DrawParamReader& reader) {}
	virtual void writeKeyParam(DrawParamWriter& writer) {}

	virtual void readExtraParam(DrawParamReader& reader) {}
	virtual void writeExtraParam(DrawParamWriter& writer) {}
};


#ifdef MINEGEDRAW_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(NullGEDraw)
#endif
