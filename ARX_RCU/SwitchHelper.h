#pragma once

//非模态下文档锁定切换
class DocumentLockSwitch
{
public:
	DocumentLockSwitch()
	{
		acDocManager->lockDocument( curDoc() );
	}
	~DocumentLockSwitch()
	{
		acDocManager->unlockDocument( curDoc() );
	}
};

//切换controlbar显示
class ControlBarShowSwitch
{
public:
	//构造函数隐藏controlbar
	ControlBarShowSwitch(CWnd* _dlg) : dlg(_dlg)
	{
		ShowParentControlBar(dlg, FALSE);
	}
	//析构函数显示controlbar
	~ControlBarShowSwitch()
	{
		ShowParentControlBar(dlg, TRUE);
	}

private:
	static void ShowParentControlBar(CWnd* pWnd, BOOL bShow)
	{
		CAcModuleResourceOverride myResources;
		CControlBar* pBar = reinterpret_cast<CControlBar*>(pWnd->GetParent());
		if(pBar == 0) return;

		CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();
		if(pAcadFrame == 0) return;

		pAcadFrame->ShowControlBar( pBar, bShow, FALSE );
	}
	CWnd* dlg;
};

//切换模态窗口显示/隐藏
class ShowOrHideWindow
{
public:
	//构造函数隐藏
	ShowOrHideWindow(CDialog* _dlg) : dlg(_dlg)
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
	CDialog* dlg;
};