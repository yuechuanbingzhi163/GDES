// UiLibDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "MainWnd.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);

	CMainWnd* pFrame = new CMainWnd();
	if(pFrame == NULL)	return 0;
	pFrame->Create(NULL,_T("UiLib Demos"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 900, 600);
	pFrame->CenterWindow();
	pFrame->ShowWindow(true);
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();

	return 0;
}