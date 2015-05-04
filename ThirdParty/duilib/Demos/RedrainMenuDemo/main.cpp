#include "duilib.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    ::CoInitialize(NULL);

    CPaintManagerUI::SetInstance(hInstance);
	//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+_T("../skin/RedrainMenuRes"));

	CFrameWnd *pFrame = new CFrameWnd();
    pFrame->Create(NULL, _T("Redrain²Ëµ¥Demo"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->ShowModal();
	//pFrame->ShowWindow(true);
	//CPaintManagerUI::MessageLoop();

    //delete pFrame;
    ::CoUninitialize();
    return 0;
}
