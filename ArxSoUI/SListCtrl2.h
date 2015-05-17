#pragma once

#include <event/Events.h>
#include <control/SListCtrl.h>

#define EVT_LC_DBL_CLICK (SOUI::EVT_EXTERNAL_BEGIN+500)
#define EVT_LC_RBUTTON_UP (SOUI::EVT_EXTERNAL_BEGIN+501)

//双击消息
class /*SOUI_EXP*/ EventLCDblClick : public SOUI::TplEventArgs<EventLCDblClick>
{
	SOUI_CLASS_NAME(EventLCDblClick,L"on_listctrl2_dbl_click")
public:
	EventLCDblClick(SOUI::SObject *pSender):SOUI::TplEventArgs<EventLCDblClick>(pSender){}
	enum{EventID=EVT_LC_DBL_CLICK};
	int nCurSel;
};

/**
* soui提供的SListCtrl无法处理鼠标双击消息
* 好在soui提供的派生机制也非常简单
* 1、从原控件派生(SOUI_CLASS_NAME这个宏不能少)
* 2、增加要处理的消息映射表及要处理的消息
* 3、为了便于扩展使用,将双击消息转发出去
*    自定义消息EventLCDblClick,定义好消息ID，以及需要的参数
* 4、在派生类的构造函数中增加我们自定义的消息
* 5、然后在双击的消息函数中将自定义消息转发出去(参见OnLButtonDblClick的实现)
* 6、派生的控件一定要注册到soui里(参见UiHelper3.cpp里的MySoUiLoader3::initExtendControls()函数
*/
class SListCtrl2 : public SOUI::SListCtrl
{
	SOUI_CLASS_NAME(SListCtrl2,L"listctrl2")
public:
	SListCtrl2() 
	{
		GetEventSet()->addEvent(EVENTID(EventLCDblClick));
	}

protected:
	void OnLButtonDblClick(UINT nFlags, SOUI::CPoint pt);
	void OnRButtonDown(UINT nFlags, SOUI::CPoint pt);

protected://消息处理，SOUI控件的消息处理和WTL，MFC很相似，采用相似的映射表，相同或者相似的消息映射宏
	//SOUI控件消息映射表
	SOUI_MSG_MAP_BEGIN()	
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClick)   
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
	SOUI_MSG_MAP_END()
};