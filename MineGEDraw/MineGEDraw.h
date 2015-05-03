//-----------------------------------------------------------------------------
//----- MineGEDraw.h : Declaration of the MineGEDraw
//-----------------------------------------------------------------------------
#pragma once

#include "dbmain.h"
#include "dlimexp.h"
#include "../ArxDbgXdata/XDataStream.h"

// 圆周率
#define PI 3.1415926535897932384626433832795

// 矿井图元绘制效果抽象基类
class MINEGE_DRAW_DLLIMPEXP MineGEDraw : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(MineGEDraw) ;

public:
	virtual ~MineGEDraw () {}

	// 设置所有附加参数为默认值
	virtual void setAllExtraParamsToDefault() = 0;

	// 弹出一个对话框进行附加参数的修改
	// 例如，1）修改双线巷道的宽度
	//		  2）修改圆形节点的半径等
	virtual void configExtraParams() = 0;

	// 当附加参数发生变化的时候，更新其它的附加参数
	// 例如双线巷道，当巷道宽度变化时，需要重新计算轮廓线坐标
	virtual void updateExtraParams() = 0;

	// 读写关键参数
	virtual void readKeyParam(DrawParamReader& reader) = 0;
	virtual void writeKeyParam(DrawParamWriter& writer) = 0;

	// 读写附加参数
	virtual void readExtraParam(DrawParamReader& reader) = 0;
	virtual void writeExtraParam(DrawParamWriter& writer) = 0;

	// 由MineGEDraw派生类负责填充names数组
	virtual void regPropertyDataNames(AcStringArray& names) const {}

	// 从图元中读取相应的属性数据
	// 图元类(MineGE及派生类)负责填充数据到reader中
	virtual void readPropertyDataFromGE(const AcStringArray& values) {}

	// 获取绘制效果的最小多边形包围盒
	// 用于背景绘制，实现"消隐"效果
	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts); // 默认实现，什么也不做

protected:
	MineGEDraw();;  // 将来可能会在构造函数中添加一些信息参数，目前尚不确定(暂时保留构造函数)
} ;

#ifdef MINEGEDRAW_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MineGEDraw)
#endif
