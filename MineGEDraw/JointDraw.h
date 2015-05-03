#pragma once

#include "MineGEDraw.h"

// 分支类图元的绘制效果抽象基类
class MINEGE_DRAW_DLLIMPEXP JointDraw : public MineGEDraw {

public:
	ACRX_DECLARE_MEMBERS(JointDraw) ;

public:
	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

public:
	virtual void extendByLength(double length);                              // 处理延伸问题，延伸length(默认不处理)

protected:
	JointDraw();

public:
	AcGePoint3d m_inserPt;   // 插入点
} ;

#ifdef MINEGEDRAW_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(JointDraw)
#endif
