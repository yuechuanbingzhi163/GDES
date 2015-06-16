#pragma once

#include "acui.h"
#include "resource.h"

class RcuAcUiBaseDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(RcuAcUiBaseDlg)

public:
	RcuAcUiBaseDlg(UINT idd,CWnd* pParent = NULL,HINSTANCE hDialogResource=NULL);   // 标准构造函数
	virtual ~RcuAcUiBaseDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	HICON m_hIcon;
};

class ShowOrHideWindow
{
public:
	//构造函数隐藏
	ShowOrHideWindow(RcuAcUiBaseDlg* _dlg) : dlg(_dlg)
	{
		dlg->ShowWindow(SW_HIDE);
		////当隐藏之后整个CAD窗口失去焦点
		//dlg->SetFocus();
	}
	//析构函数显示
	~ShowOrHideWindow()
	{
		dlg->ShowWindow(SW_SHOW);
	}
	RcuAcUiBaseDlg* dlg;
};
