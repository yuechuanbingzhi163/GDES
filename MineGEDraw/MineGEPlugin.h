#pragma once

#include "MineGEDrawManager.h"
#include "HelperMacro.h"

class MineGEPlugin
{
public:
	virtual ~MineGEPlugin() {}

	// 加载插件后的初始化工作
	virtual void initialize(MineGEDrawManager* pGEDrawManager) = 0;

	// 卸载插件之前的清理工作
	virtual void unInitialize(MineGEDrawManager* pGEDrawManager) = 0;	
};

typedef MineGEPlugin* (*MineGEPluginCreateFunc)();