#pragma once

/**
非模态对话框与cad操作同步刷新的实现方法
在ARX_RCU项目中要实现如下的功能：
(1)在非模态对话框中增加、删除、修改图元，cad窗口中能同步更新（这个实现比较简单）
(2)在cad窗口中增加、删除、修改、undo/redo，非模态对话框能同步更新

第2个功能的实现涉及到如下的一些技术要点：
1、定义数据库反应器，对图元删除、修改、undo/redo的函数进行重载
   这些函数都调用非模态对话框的父窗口（dockbar）的强制刷新函数
   参见Rcu_DbReactor::updateRcuDesignDlg()函数
2、Rcu_DbReator::updateRcuDesignDlg()函数的实现比较简单，就是调用非模态对话框的ForceUpdate()函数强制刷新
3、RcuDesiginDockBarDlg::ForceUpdate()函数内部就是简单调用了Invalidate()函数重绘界面
   为了避免无限制的刷新，函数实现使用一个简单的“布尔变量锁”，通过Global_GotoPaintFlag控制OnPaint()只被调用一次
4、RcuDesiginDockBarDlg::OnPaint()内部则调用子对话框的更新函数，实时更新界面
*/

#include "RcuDesignDlg.h"

//非模态停靠对话框
class RcuDesiginDockBarDlg : public CAcUiDockControlBar {
	DECLARE_DYNAMIC (RcuDesiginDockBarDlg)

public:
	//----- Child dialog which will use the resource id supplied
	RcuDesignDlg mChildDlg;
	//强制刷新界面
	void ForceUpdate();

public:
	RcuDesiginDockBarDlg ();
	virtual ~RcuDesiginDockBarDlg ();

public:
	virtual BOOL Create (CWnd *pParent, LPCTSTR lpszTitle) ;
	virtual void SizeChanged (CRect *lpRect, BOOL bFloating, int flags) ;

	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct) ;
	afx_msg void OnSysCommand (UINT nID, LPARAM lParam) ;
	afx_msg void OnSize (UINT nType, int cx, int cy) ;
	afx_msg void OnPaint();
	virtual bool OnClosing ();

	DECLARE_MESSAGE_MAP()
} ;
