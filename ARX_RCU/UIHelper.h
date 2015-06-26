#pragma once

#include "MyMsg.h"

class UIHelper
{
public:
	//显示钻场设计非模态对话框
	static void ShowRcuDesignDockBar();
	//销毁钻场设计对话框
	static void DestroyRcuDesignDockBar();
	//向钻场设计对话框发送消息
	static void SendMessage(unsigned int msgCode, ArxMsg* msgParam);
};
