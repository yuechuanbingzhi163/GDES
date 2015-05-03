#pragma once

#include "acui.h"
#include "Resource.h"

// 将非模态对话框的3个消息放在了基类DockBarChildDlg
// 这样派生类就可以只专注对话框本身的消息处理
// 同时减少重复代码
class DockBarChildDlg : public CAcUiDialog 
{
	DECLARE_DYNAMIC (DockBarChildDlg)

protected:
	DockBarChildDlg(UINT idd, CWnd *pParent =NULL, HINSTANCE hInstance = NULL) ;
	~DockBarChildDlg();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnClosing(); // 默认实现为空(如果需要处理关闭，重载该函数)

	virtual BOOL OnCommand (WPARAM wParam, LPARAM lParam) ;
	afx_msg void OnSize (UINT nType, int cx, int cy) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM wParam, LPARAM lParam) ;
} ;
