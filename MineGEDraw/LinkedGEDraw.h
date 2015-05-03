#pragma once

#include "MineGEDraw.h"

// 分支类图元的绘制效果抽象基类
class MINEGE_DRAW_DLLIMPEXP LinkedGEDraw : public MineGEDraw {

public:
	ACRX_DECLARE_MEMBERS(LinkedGEDraw) ;

public:
	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	// 处理分支类图元连接点闭合的虚方法
public:
	/* 
	* 对于直线巷道默认即可;
	* 但对于弧线巷道、折线巷道、硐室这类图元;
	* 它们在边界处的倾角方向可能是有变化的
	* 结果：方向向量(零向量表示不处理闭合)
	*/
	virtual AcGeVector3d getStartPointInExtendAngle() const;       // 获取始端的"向内"的延伸方向角(默认为直线倾角)
	virtual AcGeVector3d getEndPointInExtendAngle() const;         // 获取末端的"向内"的延伸方向角(默认为直线倾角的反方向)

	virtual void dealWithStartPointBoundary(const AcGeRay3d& boundaryLine);  // 处理始端与边界线的相交(默认不处理)
	virtual void dealWithEndPointBoundary(const AcGeRay3d& boundaryLine); 	 // 处理末端与边界线的相交(默认不处理)

	virtual void reverse();                                                  // 始末点反向时，其它参数也应反向变化
	virtual void extendByLength(double length);                              // 处理延伸问题，延伸length(默认不处理)

protected:
	LinkedGEDraw();

public:
	AcGePoint3d m_startPt, m_endPt;   // 始末点坐标
} ;

#ifdef MINEGEDRAW_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(LinkedGEDraw)
#endif
