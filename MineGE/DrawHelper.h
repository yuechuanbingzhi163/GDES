#pragma once

#include "dlimexp.h"

class MineGE;

/*
	颜色表
-----------------
1	|	Red		|
-----------------
2	|	Yellow	|
-----------------
3	|	Green	|
-----------------
4	|	Cyan	|
-----------------
5	|	Blue	|
-----------------
6	|	Magenta	|
-----------------
7	|White/Black|
-----------------
*/

// 可视化效果辅助类
class MINEGE_DLLIMPEXP DrawHelper
{
public:	
	// LinkedGE类型图元闭合处理
	static void LinkedGEJunctionClosure(const AcGePoint3d& junctionPt);

	// LinkedGE类型图元闭合处理
	static void LinkedGEJunctionClosure2(const AcDbObjectId& objId);

	// 分支类图元反向
	// 交换巷道、工作面等图元的始末点位置
	// 同时反向关联的方向类图元,例如风流方向，风门等
	static void ReverseDirection( const AcDbObjectId& objId );

	// 获取当前的可视化效果
	static bool GetCurrentDraw(const CString& type, CString& draw);

	// 切换所有geType图元的显示效果
	static void SwitchDraw(const CString& geType, const CString& drawName);

	// 配置图元的附加参数
	// 但不更新图形效果
	static void ConfigDraw(const CString& geType, const CString& drawName);

	// 判断图元类型geType当前绘制效果是否实现了jig过程
	static bool IsGETypeHasDrawJig(const CString& geType);

	// 获取当前已注册的MineGE图元类型(用于draw的参数设置)
	static void GetAllRegGETypesForDraw(AcStringArray& allGETypes);

	// 查找指定类型geType的所有图元
	static void FindMineGEs(const CString& geType, AcDbObjectIdArray& objIds);

	// 查找图元的所有关联的标签图元
	static void GetAllTagGEById(const AcDbObjectId& objId, AcDbObjectIdArray& objIds);

	// 查找与图元objId关联的指定类型的标签图元(包括从geType派生的图元类型)
	static void GetTagGEById2(const AcDbObjectId& objId, const CString& geType, AcDbObjectIdArray& objIds);

	// 查找所有以objId为宿主的模型图元ModelGE
	static void GetModelGEById(const AcDbObjectId& objId, AcDbObjectIdArray& objIds);

	// 用一种颜色显示图元的所有标签图元
	static void ShowAllTagGE(const AcDbObjectId& objId, unsigned short colorIndex);

	// 用一种颜色显示标签图元所关联的宿主图元HostGE
	static void ShowHostGE(const AcDbObjectId& objId, unsigned short colorIndex);

	static bool GetHostGE( const AcDbObjectId& objId, AcDbObjectId& host );

	static AcDbObjectId GetRelatedTW(AcDbObjectId objId);

	static AcDbObjectIdArray GetRelatedTunnel(AcDbObjectId tunnelId);

};