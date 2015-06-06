#pragma once

#include "../MineGEDraw/BlockDraw.h"

class GOPBlockDraw : public BlockDraw 
{
public:
	ACRX_DECLARE_MEMBERS(GOPBlockDraw) ;

public:
	virtual void regPropertyDataNames(AcStringArray& names) const;
	// 尝试在数据末尾添加单位无效
	// 原因:块的属性定义不能识别转上下标等特殊字符串
	//virtual void readPropertyDataFromGE(const AcStringArray& values);

protected:
	GOPBlockDraw();
};

class SmallGOPBlockDraw : public GOPBlockDraw {

public:
	ACRX_DECLARE_MEMBERS(SmallGOPBlockDraw) ;

public:
	SmallGOPBlockDraw();
};

class MidGOPBlockDraw : public GOPBlockDraw {

public:
	ACRX_DECLARE_MEMBERS(MidGOPBlockDraw) ;

public:
	MidGOPBlockDraw();
};

class LargeGOPBlockDraw : public GOPBlockDraw {

public:
	ACRX_DECLARE_MEMBERS(LargeGOPBlockDraw) ;

public:
	LargeGOPBlockDraw();
};

class LargerGOPBlockDraw : public GOPBlockDraw {

public:
	ACRX_DECLARE_MEMBERS(LargerGOPBlockDraw) ;

public:
	LargerGOPBlockDraw();
};

#ifdef GASGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GOPBlockDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SmallGOPBlockDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MidGOPBlockDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(LargeGOPBlockDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(LargerGOPBlockDraw)
#endif
