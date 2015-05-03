#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>

#include "ControlEx.h"

class CLoginFrameWnd : public WindowImplBase
{
public:
	CLoginFrameWnd() { };
    LPCTSTR GetWindowClassName() const { return _T("UILoginFrame"); };
    UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	virtual CDuiString GetSkinFile() { return _T("login.xml"); }
	virtual CDuiString GetSkinFolder() { return _T("../skin/GameRes"); }
	void OnFinalMessage( HWND hWnd ) { WindowImplBase::OnFinalMessage(hWnd); delete this; }

    void InitWindow() 
	{
        CComboUI* pAccountCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("accountcombo")));
        CEditUI* pAccountEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("accountedit")));
        if( pAccountCombo && pAccountEdit ) pAccountEdit->SetText(pAccountCombo->GetText());
        pAccountEdit->SetFocus();
    }

    void Notify(TNotifyUI& msg)
    {
        if( msg.sType == _T("click") ) {
            if( msg.pSender->GetName() == _T("closebtn") ) { PostQuitMessage(0); return; }
            else if( msg.pSender->GetName() == _T("loginBtn") ) { Close(); return; }
        }
        else if( msg.sType == _T("itemselect") ) {
            if( msg.pSender->GetName() == _T("accountcombo") ) {
                CEditUI* pAccountEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("accountedit")));
                if( pAccountEdit ) pAccountEdit->SetText(msg.pSender->GetText());
            }
        }
    }

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
    {
        if( uMsg == WM_KEYDOWN ) {
            if( wParam == VK_RETURN ) {
                CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("accountedit")));
                if( pEdit->GetText().IsEmpty() ) pEdit->SetFocus();
                else {
                    pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("pwdedit")));
                    if( pEdit->GetText().IsEmpty() ) pEdit->SetFocus();
                    else Close();
                }
                return true;
            }
            else if( wParam == VK_ESCAPE ) {
                PostQuitMessage(0);
                return true;
            }

        }
        return false;
    }
};

class CGameFrameWnd :  public IListCallbackUI, public WindowImplBase
{
public:
	CGameFrameWnd() {};
    LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
    UINT GetClassStyle() const { return CS_DBLCLKS; };
	virtual CDuiString GetSkinFile() { return _T("hall.xml"); }
	virtual CDuiString GetSkinFolder() { return _T("../skin/GameRes"); }
	void OnFinalMessage( HWND hWnd ) { WindowImplBase::OnFinalMessage(hWnd); delete this; }

    void InitWindow() {
        m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closebtn")));
        m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("maxbtn")));
        m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("restorebtn")));
        m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("minbtn")));

        CActiveXUI* pActiveXUI = static_cast<CActiveXUI*>(m_PaintManager.FindControl(_T("ie")));
        if( pActiveXUI ) {
            IWebBrowser2* pWebBrowser = NULL;
            pActiveXUI->GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);
            if( pWebBrowser != NULL ) {
                pWebBrowser->Navigate(_T("https://github.com/duilib/duilib"),NULL,NULL,NULL,NULL);  
                //pWebBrowser->Navigate(_T("about:blank"),NULL,NULL,NULL,NULL); 
                pWebBrowser->Release();
            }
        }
    }

	CControlUI* CreateControl(LPCTSTR pstrClass) 
	{
		if( _tcscmp(pstrClass, _T("GameList")) == 0 ) return new GameListUI(&m_PaintManager);
		else if( _tcscmp(pstrClass, _T("DeskList")) == 0 ) return new DeskListUI(&m_PaintManager);
		return NULL;
	}

    void OnPrepare() 
	{ 	
        GameListUI* pGameList = static_cast<GameListUI*>(m_PaintManager.FindControl(_T("gamelist")));
        GameListUI::Node* pCategoryNode = NULL;
        GameListUI::Node* pGameNode = NULL;
        GameListUI::Node* pServerNode = NULL;
        GameListUI::Node* pRoomNode = NULL;
        pCategoryNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 3}{x 4}推荐游戏"));
        for( int i = 0; i < 4; ++i )
        {
            pGameNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 10}{x 4}四人斗地主"), pCategoryNode);
            for( int i = 0; i < 3; ++i )
            {
                pServerNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 10}{x 4}测试服务器"), pGameNode);
                for( int i = 0; i < 3; ++i )
                {
                    pRoomNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 10}{x 4}测试房间"), pServerNode);
                }
            }
        }
        pCategoryNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 3}{x 4}最近玩过的游戏"));
        for( int i = 0; i < 2; ++i )
        {
            pGameList->AddNode(_T("三缺一"), pCategoryNode);
        }
        pCategoryNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 3}{x 4}棋牌游戏"));
        for( int i = 0; i < 8; ++i )
        {
            pGameList->AddNode(_T("双扣"), pCategoryNode);
        }
        pCategoryNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 3}{x 4}休闲游戏"));
        for( int i = 0; i < 8; ++i )
        {
            pGameList->AddNode(_T("飞行棋"), pCategoryNode);
        }

        CListUI* pUserList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("userlist")));
        pUserList->SetTextCallback(this);      
        for( int i = 0; i < 400; i++ ) {
            CListTextElementUI* pListElement = new CListTextElementUI;
            pUserList->Add(pListElement);
        }

		 CLoginFrameWnd* pLoginFrame = new CLoginFrameWnd();
		 if( pLoginFrame == NULL ) { Close(); return; }
		 pLoginFrame->Create(m_hWnd, _T(""), UI_WNDSTYLE_DIALOG, 0L, 0, 0, 0, 0);
		 pLoginFrame->SetIcon(IDI_ICON_DUILIB);
		 pLoginFrame->CenterWindow();
		 pLoginFrame->ShowModal();
    }

    void SendChatMessage() {
        CEditUI* pChatEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("chatEdit")));
        if( pChatEdit == NULL ) return;
        pChatEdit->SetFocus();
        if( pChatEdit->GetText().IsEmpty() ) return;

        CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("chatmsglist")));
        if( pRichEdit == NULL ) return;
        long lSelBegin = 0, lSelEnd = 0;
        CHARFORMAT2 cf;
        ZeroMemory(&cf, sizeof(CHARFORMAT2));
        cf.cbSize = sizeof(cf);
        cf.dwReserved = 0;
        cf.dwMask = CFM_COLOR;
        cf.crTextColor = RGB(220, 0, 0);

        lSelEnd = lSelBegin = pRichEdit->GetTextLength();
        pRichEdit->SetSel(lSelEnd, lSelEnd);
        pRichEdit->ReplaceSel(_T("某人"), false);
        lSelEnd = pRichEdit->GetTextLength();
        pRichEdit->SetSel(lSelBegin, lSelEnd);
        pRichEdit->SetSelectionCharFormat(cf);

        lSelBegin = lSelEnd;
        pRichEdit->SetSel(-1, -1);
        pRichEdit->ReplaceSel(_T("说:"), false);

        pRichEdit->SetSel(-1, -1);
        pRichEdit->ReplaceSel(pChatEdit->GetText(), false);
        pChatEdit->SetText(_T(""));

        pRichEdit->SetSel(-1, -1);
        pRichEdit->ReplaceSel(_T("\n"), false);

        cf.crTextColor = RGB(0, 0, 0);
        lSelEnd = pRichEdit->GetTextLength();
        pRichEdit->SetSel(lSelBegin, lSelEnd);
        pRichEdit->SetSelectionCharFormat(cf);

        pRichEdit->EndDown();
    }

    void Notify(TNotifyUI& msg)
    {
		if( msg.sType == _T("windowinit") ) {
			OnPrepare();
		}
        else if( msg.sType == _T("click") ) {
            if( msg.pSender == m_pCloseBtn ) { 
                COptionUI* pControl = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("hallswitch")));
                if( pControl && pControl->IsSelected() == false ) {
                    CControlUI* pFadeControl = m_PaintManager.FindControl(_T("fadeEffect"));
                    if( pFadeControl ) pFadeControl->SetVisible(true);
                }
                else {
                    /*Close()*/PostQuitMessage(0); // 因为activex的原因，使用close可能会出现错误
                }
                return; 
            }
            else if( msg.pSender == m_pMinBtn ) { SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
            else if( msg.pSender == m_pMaxBtn ) { SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
            else if( msg.pSender == m_pRestoreBtn ) { SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
            CDuiString name = msg.pSender->GetName();
            if( name == _T("quitbtn") ) {
                /*Close()*/PostQuitMessage(0); // 因为activex的原因，使用close可能会出现错误
            }
            else if( name == _T("returnhallbtn") ) {
                CControlUI* pFadeControl = m_PaintManager.FindControl(_T("fadeEffect"));
                if( pFadeControl ) pFadeControl->SetVisible(false); 

                COptionUI* pControl = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("hallswitch")));
                pControl->Activate();
                CHorizontalLayoutUI* pH = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("roomswitchpanel")));
                if( pH ) pH->SetVisible(false);
            }
            else if( name == _T("fontswitch") ) {
                TFontInfo* pFontInfo = m_PaintManager.GetDefaultFontInfo();
                if( pFontInfo->iSize < 18 ) {
                    TFontInfo* pFontInfo = m_PaintManager.GetFontInfo(0);
                    if( pFontInfo )m_PaintManager.SetDefaultFont(pFontInfo->sFontName, pFontInfo->iSize, pFontInfo->bBold, 
                        pFontInfo->bUnderline, pFontInfo->bItalic);
                }
                else {
                    TFontInfo* pFontInfo = m_PaintManager.GetFontInfo(1);
                    if( pFontInfo )m_PaintManager.SetDefaultFont(pFontInfo->sFontName, pFontInfo->iSize, pFontInfo->bBold, 
                        pFontInfo->bUnderline, pFontInfo->bItalic);
                }
                m_PaintManager.GetRoot()->NeedUpdate();
            }
            else if( name == _T("leaveBtn")  || name == _T("roomclosebtn") ) {
                COptionUI* pControl = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("hallswitch")));
                if( pControl ) {
                    pControl->Activate();
                    CHorizontalLayoutUI* pH = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("roomswitchpanel")));
                    if( pH ) pH->SetVisible(false);
                }
            }
            else if( name == _T("sendbtn") ) {
                SendChatMessage();
            }
        }
        else if( msg.sType == _T("selectchanged") ) {
            CDuiString name = msg.pSender->GetName();
            if( name == _T("hallswitch") ) {
                CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("switch")));
                if( pControl && pControl->GetCurSel() != 0 ) pControl->SelectItem(0);
            }
            else if( name == _T("roomswitch") ) {
                CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("switch")));
                if( pControl && pControl->GetCurSel() != 1 ) {
                    pControl->SelectItem(1);
                    DeskListUI* pDeskList = static_cast<DeskListUI*>(m_PaintManager.FindControl(_T("destlist")));
                    pDeskList->SetFocus();
                    CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("chatmsglist")));
                    if( pRichEdit ) {
                        pRichEdit->SetText(_T("欢迎进入XXX游戏，祝游戏愉快！\n\n"));
                        long lSelBegin = 0, lSelEnd = 0;
                        CHARFORMAT2 cf;
                        ZeroMemory(&cf, sizeof(CHARFORMAT2));
                        cf.cbSize = sizeof(cf);
                        cf.dwReserved = 0;
                        cf.dwMask = CFM_COLOR;
                        cf.crTextColor = RGB(255, 0, 0);
                        lSelEnd = pRichEdit->GetTextLength();
                        pRichEdit->SetSel(lSelBegin, lSelEnd);
                        pRichEdit->SetSelectionCharFormat(cf);
                    }
                }
            }
        }
        else if( msg.sType == _T("itemclick") ) {
            GameListUI* pGameList = static_cast<GameListUI*>(m_PaintManager.FindControl(_T("gamelist")));
            if( pGameList->GetItemIndex(msg.pSender) != -1 )
            {
                if( _tcscmp(msg.pSender->GetClass(), _T("ListLabelElementUI")) == 0 ) {
                    GameListUI::Node* node = (GameListUI::Node*)msg.pSender->GetTag();

                    POINT pt = { 0 };
                    ::GetCursorPos(&pt);
                    ::ScreenToClient(m_PaintManager.GetPaintWindow(), &pt);
                    pt.x -= msg.pSender->GetX();
                    SIZE sz = pGameList->GetExpanderSizeX(node);
                    if( pt.x >= sz.cx && pt.x < sz.cy )                     
                        pGameList->ExpandNode(node, !node->data()._expand);
                }
            }
        }
        else if( msg.sType == _T("itemactivate") ) {
            GameListUI* pGameList = static_cast<GameListUI*>(m_PaintManager.FindControl(_T("gamelist")));
            if( pGameList->GetItemIndex(msg.pSender) != -1 )
            {
                if( _tcscmp(msg.pSender->GetClass(), _T("ListLabelElementUI")) == 0 ) {
                    GameListUI::Node* node = (GameListUI::Node*)msg.pSender->GetTag();
                    pGameList->ExpandNode(node, !node->data()._expand);
                    if( node->data()._level == 3 ) {
                        COptionUI* pControl = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("roomswitch")));
                        if( pControl ) {
                            CHorizontalLayoutUI* pH = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("roomswitchpanel")));
                            if( pH ) pH->SetVisible(true);
                            pControl->SetText(node->parent()->parent()->data()._text);
                            pControl->Activate();
                        }
                    }
                }
            }
        }
        else if( msg.sType == _T("itemselect") ) {
            if( msg.pSender->GetName() == _T("chatCombo") ) {
                CEditUI* pChatEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("chatEdit")));
                if( pChatEdit ) pChatEdit->SetText(msg.pSender->GetText());
                static_cast<CComboUI*>(msg.pSender)->SelectItem(-1);
            }
        }
        else if( msg.sType == _T("return") ) {
            if( msg.pSender->GetName() == _T("chatEdit") ) {
                SendChatMessage();
            }
        }
    }

    LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
    {
        if( pControl->GetParent()->GetParent()->GetName() == _T("userlist") ) {
            if( iSubItem == 0 ) return _T("<i vip.png>");
            if( iSubItem == 1 ) return _T("<i vip.png>");
            if( iSubItem == 2 ) return _T("此人昵称");
            if( iSubItem == 3 ) return _T("5");
            if( iSubItem == 4 ) return _T("50%");
            if( iSubItem == 5 ) return _T("0%");
            if( iSubItem == 6 ) return _T("100");
        }

        return _T("");
    }

private:
    CButtonUI* m_pCloseBtn;
    CButtonUI* m_pMaxBtn;
    CButtonUI* m_pRestoreBtn;
    CButtonUI* m_pMinBtn;
    //...
};


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
	//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("../skin/GameRes"));

    HRESULT Hr = ::CoInitialize(NULL);
    if( FAILED(Hr) ) return 0;

    CGameFrameWnd* pFrame = new CGameFrameWnd();
    if( pFrame == NULL ) return 0;
	pFrame->SetIcon(IDI_ICON_DUILIB);
    pFrame->Create(NULL, _T("游戏中心"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 1024, 738);
    pFrame->CenterWindow();
    ::ShowWindow(*pFrame, SW_SHOWMAXIMIZED);

    CPaintManagerUI::MessageLoop();

    ::CoUninitialize();
    return 0;
}
