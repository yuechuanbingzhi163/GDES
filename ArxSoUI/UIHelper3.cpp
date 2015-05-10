#include "StdAfx.h"
#include "UIHelper.h"

#include "SoUILoader.h"
#include "MainDlg.h"
//#include "SouiRealWndHandler.h"
//#include "MemFlash.h"
//#include "httpsvr/HTTPServer.h"
//#include "uianimation/UiAnimationWnd.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

//#include <controls.extend/propgrid/SPropertyGrid.h>
//#include <controls.extend/SFlyWnd.h>
//#include <controls.extend/SFadeFrame.h>
//#include <controls.extend/sradiobox2.h>
//#include <controls.extend/SVscrollbar.h>
//#include <controls.extend/SChromeTabCtrl.h>
//#include <controls.extend/siectrl.h>
//#include <controls.extend/SCalendar2.h>
#include <controls.extend/SIPAddressCtrl.h>
#include <controls.extend/SChatEdit.h>
#include <controls.extend/SScrollText.h>
#include <controls.extend/SListCtrlEx.h>
#include <controls.extend/gif/SGifPlayer.h>
#include <controls.extend/gif/SSkinGif.h>

using namespace SOUI;

class MySoUiLoader3 : public SoUILoader
{
public:
	MySoUiLoader3(HINSTANCE hInstance) : SoUILoader(hInstance)
	{

	}

	~MySoUiLoader3() 
	{
		//退出GDI+环境
		SSkinGif::Gdiplus_Shutdown();
	}

	virtual LPCTSTR getSkinDir() const
	{
		return _T("\\..\\..\\Skin\\demo3");
	}

	virtual bool initExtendControls()
	{
		//向SApplication系统中注册由外部扩展的控件及SkinObj类
		//SWkeLoader wkeLoader;
		//if(wkeLoader.Init(_T("wke.dll")))        
		//{
		//	pSouiApp->RegisterWndFactory(TplSWindowFactory<SWkeWebkit>());//注册WKE浏览器
		//}
		pSouiApp->RegisterWndFactory(TplSWindowFactory<SGifPlayer>());//注册GIFPlayer
		pSouiApp->RegisterSkinFactory(TplSkinFactory<SSkinGif>());//注册SkinGif
		//pSouiApp->RegisterSkinFactory(TplSkinFactory<SSkinAPNG>());//注册SSkinAPNG
		//pSouiApp->RegisterSkinFactory(TplSkinFactory<SSkinVScrollbar>());//注册纵向滚动条皮肤

		pSouiApp->RegisterWndFactory(TplSWindowFactory<SIPAddressCtrl>());//注册IP控件
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SPropertyGrid>());//注册属性表控件
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SChromeTabCtrl>());//注册ChromeTabCtrl
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SIECtrl>());//注册IECtrl
		pSouiApp->RegisterWndFactory(TplSWindowFactory<SChatEdit>());//注册ChatEdit
		pSouiApp->RegisterWndFactory(TplSWindowFactory<SScrollText>());//注册SScrollText
		pSouiApp->RegisterWndFactory(TplSWindowFactory<SListCtrlEx>());//注册SListCtrlEx

		//if(SUCCEEDED(CUiAnimation::Init()))
		//{
		//	pSouiApp->RegisterWndFactory(TplSWindowFactory<SUiAnimationWnd>());//注册动画控件
		//}
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SFlyWnd>());//注册飞行动画控件
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SFadeFrame>());//注册渐显隐动画控件
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SRadioBox2>());//注册RadioBox2
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SCalendar2>());//注册SCalendar2

		//初始化GDI+环境
		SSkinGif::Gdiplus_Startup();
		return true;
	}

	virtual bool initRealWnd()
	{
		//设置真窗口处理接口
		//CSouiRealWndHandler * pRealWndHandler = new CSouiRealWndHandler();
		//pSouiApp->SetRealWndHandler(pRealWndHandler);
		//pRealWndHandler->Release();

		return SoUILoader::initRealWnd();
	}

	virtual bool initGlobalStyle()
	{
		//加载全局资源描述XML
		//初始化SOUI全局资源
		return (TRUE == pSouiApp->Init(_T("xml_init"), _T("uidef"))); 
	}
};

void UIHelper::ShowSoUIDlg3()
{
	// 切换资源
	CAcModuleResourceOverride myResources;

	//加载soui相关的组建和资源
	MySoUiLoader3 suLoader(_hdllInstance);
	if(!suLoader.init()) return;

	//创建并显示使用SOUI布局应用程序窗口
	CMainDlg dlgMain;
	//dlgMain.Create(acedGetAcadFrame()->GetSafeHwnd(),0,0,800,600);
	dlgMain.Create(GetActiveWindow(),0,0,800,600);
	dlgMain.GetNative()->SendMessage(WM_INITDIALOG);
	dlgMain.CenterWindow();
	dlgMain.ShowWindow(SW_SHOWNORMAL);
	suLoader.getApp()->Run(dlgMain.m_hWnd);	
}