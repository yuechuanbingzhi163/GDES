#pragma once

#include "acui.h"

#include "HelpDlg.h"
#include "PropertyDockBarChildDlg.h"
//#include "TTunnelDockBarChildDlg.h"
#include "ChimneyDockBarChildDlg.h"
#include "FanMethoChoosedDlg.h"
//#include "LibParamInputDlg.h"

enum DI_TYPE
{
	DI_HELP   = 0,
	DI_TUNNEL = 1,
	DI_TTUNNEL = 2,
	DI_CHIMNEY = 3,
	DI_FAN	   = 4,
	DI_LIB	   = 5
};

class DataInput_DockBar : public CAcUiDockControlBar 
{
	DECLARE_DYNAMIC (DataInput_DockBar)

public:
	DataInput_DockBar ();
	virtual ~DataInput_DockBar ();

	void update(DI_TYPE di, const AcDbObjectId& objId);

private:
	HelpDlg m_helpDlg;
	PropertyDockBarChildDlg m_PropertyDlg;
	//TTunnelDockBarChildDlg m_ttunnelDlg;
	ChimneyDockBarChildDlg m_chimneyDlg;
	FanMethoChoosedDlg m_fanDlg;
	//LibParamInputDlg m_libDlg;

	PropertyData_DockBarChildDlg* dlgArray[2]; // 辅助用
	void showDlg(DI_TYPE di);

	AcDbObjectId m_objId;
	DI_TYPE m_di;  // 默认DI_HELP

public:
	virtual BOOL Create (CWnd *pParent, LPCTSTR lpszTitle) ;
	virtual void SizeChanged (CRect *lpRect, BOOL bFloating, int flags) ;
	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct) ;
	afx_msg void OnSysCommand (UINT nID, LPARAM lParam) ;
	afx_msg void OnSize (UINT nType, int cx, int cy) ;

	// 处理dock bar关闭或隐藏时的情况
	virtual bool OnClosing ();

	DECLARE_MESSAGE_MAP()
} ;
