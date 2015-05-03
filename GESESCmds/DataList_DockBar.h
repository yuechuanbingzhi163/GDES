#pragma once

#include "acui.h"
#include "DataListDlg.h"

class DataList_DockBar : public CAcUiDockControlBar {
	DECLARE_DYNAMIC (DataList_DockBar)

public:
	//----- Child dialog which will use the resource id supplied
	DataListDlg mChildDlg ;
public:
	DataList_DockBar ();
	virtual ~DataList_DockBar ();
public:
	virtual BOOL Create (CWnd *pParent, LPCTSTR lpszTitle) ;
	virtual void SizeChanged (CRect *lpRect, BOOL bFloating, int flags) ;

	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct) ;
	afx_msg void OnSysCommand (UINT nID, LPARAM lParam) ;
	afx_msg void OnSize (UINT nType, int cx, int cy) ;

	virtual bool OnClosing ();

	DECLARE_MESSAGE_MAP()
} ;
