#include "StdAfx.h"
#include "UIHelper.h"

#include "SoUILoader.h"
#include "realwnddlg.h"
//#include "SouiRealWndHandler.h"
//#include "MemFlash.h"
//#include "httpsvr/HTTPServer.h"
//#include "uianimation/UiAnimationWnd.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

#include <controls.extend/propgrid/SPropertyGrid.h>
#include <controls.extend/SFlyWnd.h>
#include <controls.extend/SFadeFrame.h>
#include <controls.extend/sradiobox2.h>
#include <controls.extend/SVscrollbar.h>
#include <controls.extend/SChromeTabCtrl.h>
#include <controls.extend/siectrl.h>
#include <controls.extend/schatedit.h>
#include <controls.extend/SScrollText.h>
#include <controls.extend/SCalendar2.h>
#include <controls.extend/SListCtrlEx.h>

class MySoUiLoader : public SoUILoader
{
public:
	MySoUiLoader(HINSTANCE hInstance) : SoUILoader(hInstance)
	{

	}

	~MySoUiLoader() 
	{
		//卸载菜单边框绘制hook
		CMenuWndHook::UnInstallHook();  
	}

	virtual LPCTSTR getSkinDir() const
	{
		return _T("\\..\\..\\Skin\\ArxSoUI");
	}

	virtual bool initExtendControls()
	{
		//向SApplication系统中注册由外部扩展的控件及SkinObj类
		//SWkeLoader wkeLoader;
		//if(wkeLoader.Init(_T("wke.dll")))        
		//{
		//	pSouiApp->RegisterWndFactory(TplSWindowFactory<SWkeWebkit>());//注册WKE浏览器
		//}
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SGifPlayer>());//注册GIFPlayer
		//pSouiApp->RegisterSkinFactory(TplSkinFactory<SSkinGif>());//注册SkinGif
		//pSouiApp->RegisterSkinFactory(TplSkinFactory<SSkinAPNG>());//注册SSkinAPNG
		//pSouiApp->RegisterSkinFactory(TplSkinFactory<SSkinVScrollbar>());//注册纵向滚动条皮肤

		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SIPAddressCtrl>());//注册IP控件
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SPropertyGrid>());//注册属性表控件
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SChromeTabCtrl>());//注册ChromeTabCtrl
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SIECtrl>());//注册IECtrl
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SChatEdit>());//注册ChatEdit
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SScrollText>());//注册SScrollText
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SListCtrlEx>());//注册SListCtrlEx

		//if(SUCCEEDED(CUiAnimation::Init()))
		//{
		//	pSouiApp->RegisterWndFactory(TplSWindowFactory<SUiAnimationWnd>());//注册动画控件
		//}
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SFlyWnd>());//注册飞行动画控件
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SFadeFrame>());//注册渐显隐动画控件
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SRadioBox2>());//注册RadioBox2
		//pSouiApp->RegisterWndFactory(TplSWindowFactory<SCalendar2>());//注册SCalendar2

		//SSkinGif::Gdiplus_Startup();
		return true;
	}

	virtual bool initSysResource()
	{
		if(SoUILoader::initSysResource())
		{
			//采用hook绘制菜单的边框
			CMenuWndHook::InstallHook(hInstance,_T("_skin.sys.menu.border"));
		}
		return SoUILoader::initSysResource();
	}

	virtual bool initRealWnd()
	{
		//设置真窗口处理接口
		//CSouiRealWndHandler * pRealWndHandler = new CSouiRealWndHandler();
		//pSouiApp->SetRealWndHandler(pRealWndHandler);
		//pRealWndHandler->Release();

		return SoUILoader::initRealWnd();
	}
};

void UIHelper::ShowSoUIDlg()
{
	// 切换资源
	CAcModuleResourceOverride myResources;

	//加载soui相关的组建和资源
	MySoUiLoader suLoader(_hdllInstance);
	if(!suLoader.init()) return;

	//创建并显示使用SOUI布局应用程序窗口
	CRealWndDlg dlg;
	dlg.DoModal();
}