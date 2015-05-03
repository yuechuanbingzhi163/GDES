#pragma once

#include "dlimexp.h"

#include <map>
#include <deque>

class MineGEDraw;
class MineGEDraw_Jig;

struct MineGEDrawInfo
{
	MineGEDraw* pDraw;
	MineGEDraw_Jig* pJig;

	bool operator==(const MineGEDrawInfo& drawInfo)
	{
		return (pDraw == drawInfo.pDraw) && (pJig == drawInfo.pJig);
	}
};

class MINEGE_DRAW_DLLIMPEXP MineGEDrawManager
{
public:
	// 注册绘制效果
	void regGEDraw(const CString& geType, MineGEDraw* pDraw, MineGEDraw_Jig* pJig);

	// 反注册绘制效果
	void unRegGEDraw(const CString& geType, const CString& realGEDrawName);

	// 指定当前的图元类型的绘制效果
	bool setCurrentGEDraw(const CString& geType, const CString& realGEDrawName);

	// 获取图元类型的当前绘制效果
	MineGEDraw* getCurrentGEDraw(const CString& geType);

	MineGEDraw* getGEDraw(const CString& geType, const CString& realGEDrawName);

	// 获取图元类型的所有绘制效果	
	void getAllGEDrawsByGEType(const CString& geType, AcStringArray& drawList);

	// 获取图元类型的绘制jig
	MineGEDraw_Jig* getGEDraw_Jig(const CString& geType, const CString& realGEDrawName);

private:
	typedef std::deque<MineGEDrawInfo> MineGEDrawInfoList;

	struct DrawListInfo
	{
		MineGEDraw* m_defDraw; // 默认绘制效果
		MineGEDrawInfoList m_drawInfoList;
	};

	typedef std::map<CString, DrawListInfo> GEDrawMap;
	GEDrawMap m_drawMap;
};