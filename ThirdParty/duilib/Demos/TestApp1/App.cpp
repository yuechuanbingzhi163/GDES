// App.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

class CFrameWindowWnd : public CWindowWnd, public INotifyUI
{
public:
    CFrameWindowWnd() { };
    LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
    UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
    void OnFinalMessage(HWND /*hWnd*/) { delete this; };

    void Init() { }

    bool OnHChanged(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam) {
        TNotifyUI* pMsg = pTNotifyUI;
        if( pMsg->sType == _T("valuechanged") ) {
            short H, S, L;
            CPaintManagerUI::GetHSL(&H, &S, &L);
            CPaintManagerUI::SetHSL(true, (static_cast<CSliderUI*>(pMsg->pSender))->GetValue(), S, L);
        }
        return true;
    }

    bool OnSChanged(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam) {
        TNotifyUI* pMsg = pTNotifyUI;
        if( pMsg->sType == _T("valuechanged") ) {
            short H, S, L;
            CPaintManagerUI::GetHSL(&H, &S, &L);
            CPaintManagerUI::SetHSL(true, H, (static_cast<CSliderUI*>(pMsg->pSender))->GetValue(), L);
        }
        return true;
    }

    bool OnLChanged(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam) {
        TNotifyUI* pMsg = pTNotifyUI;
        if( pMsg->sType == _T("valuechanged") ) {
            short H, S, L;
            CPaintManagerUI::GetHSL(&H, &S, &L);
            CPaintManagerUI::SetHSL(true, H, S, (static_cast<CSliderUI*>(pMsg->pSender))->GetValue());
        }
        return true;
    }

    bool OnAlphaChanged(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam) {
        TNotifyUI* pMsg = pTNotifyUI;
        if( pMsg->sType == _T("valuechanged") ) {
            m_pm.SetTransparent((static_cast<CSliderUI*>(pMsg->pSender))->GetValue());
        }
        return true;
    }

    void OnPrepare() 
    {
        CSliderUI* pSilder = static_cast<CSliderUI*>(m_pm.FindControl(_T("alpha_controlor")));
        if( pSilder ) pSilder->OnNotify += MakeDelegate(this, &CFrameWindowWnd::OnAlphaChanged,_T("click"));
        pSilder = static_cast<CSliderUI*>(m_pm.FindControl(_T("h_controlor")));
        if( pSilder ) pSilder->OnNotify += MakeDelegate(this, &CFrameWindowWnd::OnHChanged,_T("click"));
        pSilder = static_cast<CSliderUI*>(m_pm.FindControl(_T("s_controlor")));
        if( pSilder ) pSilder->OnNotify += MakeDelegate(this, &CFrameWindowWnd::OnSChanged,_T("click"));
        pSilder = static_cast<CSliderUI*>(m_pm.FindControl(_T("l_controlor")));
        if( pSilder ) pSilder->OnNotify += MakeDelegate(this, &CFrameWindowWnd::OnLChanged,_T("click"));
    }

    void Notify(TNotifyUI& msg)
    {
        if( msg.sType == _T("windowinit") ) OnPrepare();
        else if( msg.sType == _T("click") ) {
            if( msg.pSender->GetName() == _T("insertimagebtn") ) {
                CRichEditUI* pRich = static_cast<CRichEditUI*>(m_pm.FindControl(_T("testrichedit")));
                if( pRich ) {
                    pRich->RemoveAll();
                }
            }
            else if( msg.pSender->GetName() == _T("changeskinbtn") ) {
                if( CPaintManagerUI::GetResourcePath() == CPaintManagerUI::GetInstancePath() )
                    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("..\\skin\\FlashRes"));
                else
                    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
                CPaintManagerUI::ReloadSkin();
            }
        }
    }

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
		if( wParam == SC_CLOSE ) {
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if( ::IsZoomed(*this) != bZoomed ) {
			if( !bZoomed ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(false);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(true);
			}
			else {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(true);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(false);
			}
		}
		return lRes;
	}

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
		LRESULT lRes = 0;
		BOOL bHandled = FALSE;

        if( uMsg == WM_CREATE ) {
            m_pm.Init(m_hWnd);
            CDialogBuilder builder;
            CControlUI* pRoot = builder.Create(_T("test1.xml"), (UINT)0, NULL, &m_pm);
            ASSERT(pRoot && "Failed to parse XML");
            m_pm.AttachDialog(pRoot);
            m_pm.AddNotifier(this);
            Init();
            return 0;
        }
        else if( uMsg == WM_DESTROY ) {
            ::PostQuitMessage(0L);
        }
        else if( uMsg == WM_ERASEBKGND ) {
            return 1;
        }
		else if( uMsg == WM_CLOSE) {
			lRes = OnClose(uMsg, wParam, lParam, bHandled);
		}
		else if(uMsg == WM_SYSCOMMAND) {
			lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); 
		}

		if( bHandled ) return lRes;
        if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }

public:
    CPaintManagerUI m_pm;
};


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("../skin/TestRes"));

    HRESULT Hr = ::CoInitialize(NULL);
    if( FAILED(Hr) ) return 0;

    CFrameWindowWnd* pFrame = new CFrameWindowWnd();
    if( pFrame == NULL ) return 0;
    pFrame->Create(NULL, _T("这是一个最简单的测试用exe，修改test1.xml就可以看到效果"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pFrame->CenterWindow();
    pFrame->ShowWindow(true);
    CPaintManagerUI::MessageLoop();

    ::CoUninitialize();
    return 0;
}
