#pragma once

#include "MyMsg.h"

class UIHelper
{
public:
	//显示石门设计非模态对话框
	static void ShowRcuDesignDockBar();
	//销毁石门设计对话框
	static void DestroyRcuDesignDockBar();
	//向石门设计对话框发送消息
	static void SendMessage(unsigned int msgCode, ArxMsg* msgParam);
};
