#include <UiLib\UIlib.h>
using namespace UiLib;

#include "main_frame.hpp"

#if defined(WIN32) && !defined(UNDER_CE)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
#else
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int nCmdShow)
#endif
{
	CPaintManagerUI::SetInstance(hInstance);

#if defined(WIN32) && !defined(UNDER_CE)
	HRESULT Hr = ::CoInitialize(NULL);
#else
	HRESULT Hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
#endif
	if( FAILED(Hr) ) return 0;

	MainFrame* pFrame = new MainFrame();
	if( pFrame == NULL ) return 0;
#if defined(WIN32) && !defined(UNDER_CE)
	pFrame->Create(NULL, _T("MenuTest"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 800);
#else
	pFrame->Create(NULL, _T("MenuTest"), UI_WNDSTYLE_FRAME, WS_EX_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
#endif
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);

	CPaintManagerUI::MessageLoop();

	return 0;
}