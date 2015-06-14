#pragma once

#include "acui.h"
#include "RcuDesignDlg.h"
//#include "DockBarChildDlg.h"

class RcuDesiginDockBarDlg : public CAcUiDockControlBar {
	DECLARE_DYNAMIC (RcuDesiginDockBarDlg)

public:
	//----- Child dialog which will use the resource id supplied
	RcuDesignDlg mChildDlg ;

public:
	RcuDesiginDockBarDlg ();
	virtual ~RcuDesiginDockBarDlg ();

public:
	virtual BOOL Create (CWnd *pParent, LPCTSTR lpszTitle) ;
	virtual void SizeChanged (CRect *lpRect, BOOL bFloating, int flags) ;

	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct) ;
	afx_msg void OnSysCommand (UINT nID, LPARAM lParam) ;
	afx_msg void OnSize (UINT nType, int cx, int cy) ;

	virtual bool OnClosing ();

	DECLARE_MESSAGE_MAP()
} ;
