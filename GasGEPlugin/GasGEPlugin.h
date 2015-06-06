#pragma once

#include "../MineGEDraw/MineGEPlugin.h"

class GasGEPlugin : public MineGEPlugin
{
public:
	virtual void initialize(MineGEDrawManager* pGEDrawManager);

	// 卸载插件之前的清理工作
	virtual void unInitialize(MineGEDrawManager* pGEDrawManager);
};
