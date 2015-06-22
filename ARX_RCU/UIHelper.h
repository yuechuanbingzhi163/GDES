#pragma once

class UIHelper
{
public:
	//显示石门设计非模态对话框
	static void ShowRcuDesignDockBar();
	//销毁石门设计对话框
	static void DestroyRcuDesignDockBar();
	//刷新石门设计对话框
	//用于反应器强制刷新停靠对话框
	static void ForceUpdateDockBar();
};
