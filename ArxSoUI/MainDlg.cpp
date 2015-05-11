#include "stdafx.h"
using namespace SOUI;

#include "MainDlg.h"
#include "FormatMsgDlg.h"

#include <controls.extend/FileHelper.h>
#include <controls.extend/SChatEdit.h>
#include <controls.extend/gif/SGifPlayer.h>
#include <controls.extend/reole/richeditole.h>

using namespace SOUI;

#pragma warning(disable:4192)

#ifdef _DEBUG
#import "SoSmileyd.dll" named_guids
#else
#import "SoSmiley.dll" named_guids
#endif

#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")

#include <shellapi.h>

int CMainDlg::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	//MARGINS mar = {5,5,30,5};
	//DwmExtendFrameIntoClientArea ( m_hWnd, &mar );//打开这里可以启用Aero效果
	SetMsgHandled(FALSE);
	return 0;
}

void CMainDlg::OnShowWindow( BOOL bShow, UINT nStatus )
{
	if(bShow)
	{
 		AnimateHostWindow(200,AW_CENTER);
	}
}

struct student{
    TCHAR szName[100];
    TCHAR szSex[10];
    int age;
    int score;
};

//init listctrl
void CMainDlg::InitListCtrl()
{
    //找到列表控件
    SListCtrl *pList=FindChildByName2<SListCtrl>(L"lc_test");
    if(pList)
    {
        //列表控件的唯一子控件即为表头控件
        SWindow *pHeader=pList->GetWindow(GSW_FIRSTCHILD);
        //向表头控件订阅表明点击事件，并把它和OnListHeaderClick函数相连。
        pHeader->GetEventSet()->subscribeEvent(EVT_HEADER_CLICK,Subscriber(&CMainDlg::OnListHeaderClick,this));

        TCHAR szSex[][5]={_T("男"),_T("女"),_T("人妖")};
        for(int i=0;i<100;i++)
        {
            student *pst=new student;
            _stprintf(pst->szName,_T("学生_%d"),i+1);
            _tcscpy(pst->szSex,szSex[rand()%3]);
            pst->age=rand()%30;
            pst->score=rand()%60+40;

            int iItem=pList->InsertItem(i,pst->szName);
            pList->SetItemData(iItem,(DWORD)pst);
            pList->SetSubItemText(iItem,1,pst->szSex);
            TCHAR szBuf[10];
            _stprintf(szBuf,_T("%d"),pst->age);
            pList->SetSubItemText(iItem,2,szBuf);
            _stprintf(szBuf,_T("%d"),pst->score);
            pList->SetSubItemText(iItem,3,szBuf);
        }
    }
}

int funCmpare(void* pCtx,const void *p1,const void *p2)
{
    int iCol=*(int*)pCtx;

    const DXLVITEM *plv1=(const DXLVITEM*)p1;
    const DXLVITEM *plv2=(const DXLVITEM*)p2;

    const student *pst1=(const student *)plv1->dwData;
    const student *pst2=(const student *)plv2->dwData;

    switch(iCol)
    {
    case 0://name
        return _tcscmp(pst1->szName,pst2->szName);
    case 1://sex
        return _tcscmp(pst1->szSex,pst2->szSex);
    case 2://age
        return pst1->age-pst2->age;
    case 3://score
        return pst1->score-pst2->score;
    default:
        return 0;
    }
}

//表头点击事件处理函数
bool CMainDlg::OnListHeaderClick(EventArgs *pEvtBase)
{
    //事件对象强制转换
    EventHeaderClick *pEvt =(EventHeaderClick*)pEvtBase;
    SHeaderCtrl *pHeader=(SHeaderCtrl*)pEvt->sender;
    //从表头控件获得列表控件对象
    SListCtrl *pList= (SListCtrl*)pHeader->GetParent();
    //列表数据排序
    SHDITEM hditem;
    hditem.mask=SHDI_ORDER;
    pHeader->GetItem(pEvt->iItem,&hditem);
    pList->SortItems(funCmpare,&hditem.iOrder);
    return true;
}

void CMainDlg::OnDestory()
{
    SListCtrl *pList=FindChildByName2<SListCtrl>(L"lc_test");
    if(pList)
    {
        for(int i=0;i<pList->GetItemCount();i++)
        {
            student *pst=(student*) pList->GetItemData(i);
            delete pst;
        }
    }
    SetMsgHandled(FALSE); 
}


class CSmileySource2 : public CSmileySource
{
public:
    CSmileySource2(){}

protected:
    //获对ID对应的图片路径
    virtual SStringW ImageID2Path(UINT nID)
    {
        return SStringW().Format(L"./gif/%d.gif",nID);
    }
};

//Richedit中插入表情使用的回调函数。
ISmileySource * CreateSource2()
{
    return  new CSmileySource2;
}

LRESULT CMainDlg::OnInitDialog( HWND hWnd, LPARAM lParam )
{
    m_bLayoutInited=TRUE;
    InitListCtrl();
        
    SRichEdit *pEdit = FindChildByName2<SRichEdit>(L"re_gifhost");
    if(pEdit)
    {
        SetSRicheditOleCallback(pEdit,CreateSource2);
        pEdit->SetAttribute(L"rtf",L"rtf:rtf_test");
    }

    return 0;
}

void CMainDlg::OnBtnSelectGIF()
{
    SGifPlayer *pGifPlayer = FindChildByName2<SGifPlayer>(L"giftest");
    if(pGifPlayer)
    {
        CFileDialogEx openDlg(TRUE,_T("gif"),0,6,_T("gif files(*.gif)\0*.gif\0All files (*.*)\0*.*\0\0"));
        if(openDlg.DoModal()==IDOK)
            pGifPlayer->PlayGifFile(openDlg.m_szFileName);
    }
}

void CMainDlg::OnBtnMenu()
{
    SMenu menu;
    menu.LoadMenu(_T("menu_test"),_T("LAYOUT"));
    POINT pt;
    GetCursorPos(&pt);
    menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);
}

//演示如何响应菜单事件
void CMainDlg::OnCommand( UINT uNotifyCode, int nID, HWND wndCtl )
{
    if(uNotifyCode==0)
    {
        if(nID==6)
        {//nID==6对应menu_test定义的菜单的exit项。
            PostMessage(WM_CLOSE);
        }else if(nID==54)
        {//about SOUI
            STabCtrl *pTabCtrl = FindChildByName2<STabCtrl>(L"tab_main");
            if(pTabCtrl) pTabCtrl->SetCurSel(_T("about"));
        }
    }
}

void CMainDlg::OnBtnInsertGif2RE()
{
    SRichEdit *pEdit = FindChildByName2<SRichEdit>(L"re_gifhost");
    if(pEdit)
    {
        CFileDialogEx openDlg(TRUE,_T("gif"),0,6,_T("gif files(*.gif)\0*.gif\0All files (*.*)\0*.*\0\0"));
        if(openDlg.DoModal()==IDOK)
        {
            ISmileySource* pSource = new CSmileySource2;
            HRESULT hr=pSource->LoadFromFile(S_CT2W(openDlg.m_szFileName));
            if(SUCCEEDED(hr))
            {
                SComPtr<ISoSmileyCtrl> pSmiley;
                hr=::CoCreateInstance(SoSmiley::CLSID_CSoSmileyCtrl,NULL,CLSCTX_INPROC,__uuidof(SoSmiley::ISoSmileyCtrl),(LPVOID*)&pSmiley); 
                if(SUCCEEDED(hr))
                {
                    pSmiley->SetSource(pSource);
                    SComPtr<IRichEditOle> ole;
                    pEdit->SSendMessage(EM_GETOLEINTERFACE,0,(LPARAM)&ole);
                    pSmiley->Insert2Richedit((DWORD_PTR)(void*)ole);
                }else
                {
                    UINT uRet = SMessageBox(m_hWnd,_T("可能是因为没有向系统注册表情COM模块。\\n现在注册吗?"),_T("创建表情OLE对象失败"),MB_YESNO|MB_ICONSTOP);
                    if(uRet == IDYES)
                    {
                        HMODULE hMod = LoadLibrary(_T("sosmiley.dll"));
                        if(hMod)
                        {
                            typedef HRESULT (STDAPICALLTYPE *DllRegisterServerPtr)();
                            DllRegisterServerPtr funRegDll = (DllRegisterServerPtr)GetProcAddress(hMod,"DllRegisterServer");
                            if(funRegDll)
                            {
                                HRESULT hr=funRegDll();
                                if(FAILED(hr))
                                {
                                    SMessageBox(m_hWnd,_T("请使用管理员权限运行模块注册程序"),_T("注册表情COM失败"),MB_OK|MB_ICONSTOP);
                                }else
                                {
                                    SMessageBox(m_hWnd,_T("请重试"),_T("注册成功"),MB_OK|MB_ICONINFORMATION);
                                }
                            }
                            FreeLibrary(hMod);
                        }else
                        {
                            SMessageBox(m_hWnd,_T("没有找到表情COM模块[sosmiley.dll]。\\n现在注册吗"),_T("错误"),MB_OK|MB_ICONSTOP);
                        }
                    }
                }
            }else
            {
                SMessageBox(m_hWnd,_T("加载表情失败"),_T("错误"),MB_OK|MB_ICONSTOP);
            }
            pSource->Release();
        }
    }
}

void CMainDlg::OnBtnMsgBox()
{
    SMessageBox(NULL,_T("this is a message box"),_T("haha"),MB_OK|MB_ICONEXCLAMATION);
    SMessageBox(NULL,_T("this message box includes two buttons"),_T("haha"),MB_YESNO|MB_ICONQUESTION);
    SMessageBox(NULL,_T("this message box includes three buttons"),NULL,MB_ABORTRETRYIGNORE);
}

void CMainDlg::OnListBoxExEvent( EventArgs *pEvt )
{
    EventOfPanel *pEvtOfPanel = (EventOfPanel*)pEvt;
    if(pEvtOfPanel->pOrgEvt->GetID() == EventCmd::EventID
        && pEvtOfPanel->pOrgEvt->sender->IsClass(SButton::GetClassName()))
    {
        int iItem = pEvtOfPanel->pPanel->GetItemIndex();
        SStringT strMsg;
        strMsg.Format(_T("收到列表项:%d中的name为%s的窗口点击事件"),iItem,S_CW2T(pEvtOfPanel->pOrgEvt->nameFrom));
        SMessageBox(m_hWnd,strMsg,_T("EVENTOFPANEL"),MB_OK|MB_ICONEXCLAMATION);        
    }
}

void CMainDlg::OnTreeBoxEvent( EventArgs *pEvt )
{
    EventOfPanel *pEvtOfPanel = (EventOfPanel*)pEvt;
    if(pEvtOfPanel->pOrgEvt->GetID() == EventCmd::EventID
        && pEvtOfPanel->pOrgEvt->sender->IsClass(SButton::GetClassName()))
    {
        HSTREEITEM hItem = (HSTREEITEM)pEvtOfPanel->pPanel->GetItemIndex();
        SStringT strMsg;
        strMsg.Format(_T("收到treebox item:0x%08x中的name为%s的窗口点击事件"),hItem,S_CW2T(pEvtOfPanel->pOrgEvt->nameFrom));
        SMessageBox(m_hWnd,strMsg,_T("EVENTOFPANEL"),MB_OK|MB_ICONEXCLAMATION);        
    }

    if(pEvtOfPanel->pOrgEvt->GetID() >= EVT_ITEMPANEL_CLICK && pEvtOfPanel->pOrgEvt->GetID() <= EVT_ITEMPANEL_RCLICK)
    {
        HSTREEITEM hItem = (HSTREEITEM)pEvtOfPanel->pPanel->GetItemIndex();
        STRACE(_T("OnTreeBoxEvent: EVT_ITEMPANEL_X, itemid=0x%08x,evtid=%d"),hItem,pEvtOfPanel->pOrgEvt->GetID());
    }
}

void CMainDlg::OnTreeBoxQueryItemHeight( EventArgs * pEvt )
{
    EventTBQueryItemHeight *pEvtTbQueryItemHeight = (EventTBQueryItemHeight*)pEvt;
    STreeBox *pTreeBox = (STreeBox*)pEvt->sender;
    STreeItem *pItem = pTreeBox->GetItemPanel(pEvtTbQueryItemHeight->hItem);

    if(pItem->m_nLevel>0)
    {
        if(pEvtTbQueryItemHeight->dwState & WndState_Check)
            pEvtTbQueryItemHeight->nItemHeight = 40;
        else
            pEvtTbQueryItemHeight->nItemHeight = 30;
    }else
    {
        pEvtTbQueryItemHeight->nItemHeight = 50;
    }
}