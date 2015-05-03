#pragma once

#include "dlimexp.h"

class MineGEDraw;
class MineGEDraw_Jig;
class MineGEPluginManager;
class MineGEDrawManager;

class MINEGE_DRAW_DLLIMPEXP MineGEDrawSystem
{
public:
	static MineGEDrawSystem* CreateInstance();
	static MineGEDrawSystem* GetInstance();
	static void DestroyInstance();

	// 插件功能
public:
	// 加载绘制效果插件
	void loadPlugin(const CString &arxFilePath);

	// 卸载绘制效果插件
	void unloadPlugin(const CString &arxFilePath);

	// 绘制效果功能
public:
	// 获取图元类型的默认绘制效果
	MineGEDraw* getDefaultGEDraw(const CString& geType);

	// 获取图元类型的默认绘制效果
	CString getDefaultGEDrawName(const CString& geType);

	MineGEDraw* getGEDraw(const CString& geType, const CString& realGEDrawName);

	MineGEDraw_Jig* getGEDraw_Jig(const CString& geType, const CString& realGEDrawName);

	// 指定图元类型的当前绘制效果
	bool setCurrentGEDraw(const CString& geType, const CString& realGEDrawName);

	// 获取指定图元类型的所有绘制效果（用于遍历）
	void getAllGEDrawsByGEType(const CString& geType, AcStringArray& drawList);

private:
	static MineGEDrawSystem* m_pMineGEService;
	MineGEDrawSystem(void);
	~MineGEDrawSystem(void);

private:
	MineGEPluginManager* m_pMineGEPluginManager;
	MineGEDrawManager* m_pMineGEDrawManager;
};
