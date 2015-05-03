#pragma once

class CFrameWnd: public WindowImplBase
{
public:
	CFrameWnd();

	LPCTSTR GetWindowClassName() const;
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	virtual void OnFinalMessage( HWND hWnd );
	//UILIB_RESOURCETYPE GetResourceType() const;
	//LPCTSTR GetResourceID() const;

     void InitWindow();
     void Notify(TNotifyUI& msg);
	 LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	 // CControlUI* CreateControl(LPCTSTR pstrClassName);
private:
	map<CDuiString,bool> m_MenuCheckInfo; //保存菜单的单选复选信息
};