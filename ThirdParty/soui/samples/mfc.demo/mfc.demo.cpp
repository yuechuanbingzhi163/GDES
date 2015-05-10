
// mfc.demo.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "mfc.demo.h"
#include "mfc.demoDlg.h"
#include "SouiRealWndHandler.h"
#include <res.mgr/sobjdefattr.h>
#include <com-cfg.h>
#include <com-loader.hpp>

#include <helper/MenuWndHook.h>
#include <helper/mybuffer.h>


using namespace SOUI;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#define COM_IMGDECODER  _T("imgdecoder-wicd.dll")
#define COM_RENDER_GDI  _T("render-gdid.dll")
#else
#define COM_IMGDECODER  _T("imgdecoder-wic.dll")
#define COM_RENDER_GDI  _T("render-gdi.dll")
#endif


#ifdef _DEBUG
#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
#else
#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#endif

#define RES_TYPE 0   //从文件中加载资源
// #define RES_TYPE 1   //从PE资源中加载UI资源
// #define RES_TYPE 2   //从zip包中加载资源

class SoUILoader
{
public:
	SoUILoader(HINSTANCE _hInstance) : hInstance(_hInstance), pSouiApp(0)
	{
		HRESULT hRes = CoInitialize(NULL);
		SASSERT(SUCCEEDED(hRes));
		//和官方的例子有些不同
		//在SetCurrentDirectory前加载dll
		hSysResource=LoadLibrary(SYS_NAMED_RESOURCE);
	}

	~SoUILoader()
	{
		hInstance = 0; hSysResource = 0;
		delete pSouiApp; pSouiApp = 0;
		//卸载菜单边框绘制hook
		CMenuWndHook::UnInstallHook();
		CoUninitialize();
	}

	SComMgr* getComMrg()
	{
		return &comMgrObj;
	}

	LPCTSTR getSkinDir() const
	{
		return _T("\\..\\..\\Skin\\ArxSoUI");
	}

	bool initRender()
	{
		//加载render组件(render_gdi或render_skia)
		//bLoaded = pComMgr->CreateRender_Skia((IObjRef**)&pRenderFactory)
		BOOL bLoaded = getComMrg()->CreateRender_GDI((IObjRef**)&pRenderFactory);
		SASSERT_FMT(bLoaded,_T("load interface [%s] failed!"),_T("render-gdi"));

		//加载图片解码模块(imggid或imgwic)
		bLoaded=getComMrg()->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
		SASSERT_FMT(bLoaded,_T("load interface [%s] failed!"), _T("imgdecoder"));

		//为渲染模块设置它需要引用的图片解码模块
		if(bLoaded == TRUE)
		{
			pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
		}
		return bLoaded == TRUE;
	}

	bool initApp()
	{
		if(pSouiApp == 0)
		{
			pSouiApp=new SApplication(pRenderFactory, hInstance);
			SStringT strResDir = pSouiApp->GetAppDir();
			strResDir += getSkinDir();
			//将程序的运行路径修改到demo所在的目录
			//注意:最好是在加载soui-sys-resource.dll之后设置,否则soui系统资源加载会失败!!!
			SetCurrentDirectory(strResDir);
			//SOUI系统总是从appdir去查找资源
			//注:这个说法是不准确的,soui的源代码中并没有使用GetAppDir(),主要还是依赖SetCurrentDirectory()设置当前路径
			pSouiApp->SetAppDir(strResDir);
		}
		return true;
	}

	bool initResource()
	{
		//定义一人个资源提供对象,SOUI系统中实现了3种资源加载方式，分别是从文件加载，从EXE的资源加载及从ZIP压缩包加载
		CAutoRefPtr<IResProvider>   pResProvider;
		BOOL bLoaded = FALSE;

#if (RES_TYPE == 0)//从文件加载
		CreateResProvider(RES_FILE,(IObjRef**)&pResProvider);
		bLoaded = pResProvider->Init((LPARAM)_T("uires"),0);
#elif (RES_TYPE==1)//从EXE资源加载
		CreateResProvider(RES_PE,(IObjRef**)&pResProvider);
		bLoaded = pResProvider->Init((WPARAM)hInstance,0);
#elif (RES_TYPE==2)//从ZIP包加载
		bLoaded=getComMrg()->CreateResProvider_ZIP((IObjRef**)&pResProvider);
		SASSERT_FMT(bLoaded,_T("load interface [%s] failed!"),_T("resprovider_zip"));

		ZIPRES_PARAM param;
		param.ZipFile(pRenderFactory, _T("uires.zip"),"souizip");
		bLoaded = pResProvider->Init((WPARAM)&param,0);
		SASSERT(bLoaded);
#endif

		if(bLoaded == TRUE)
		{
			//将创建的IResProvider交给SApplication对象
			//从资源加载皮肤
			pSouiApp->AddResProvider(pResProvider);
		}
		return (bLoaded == TRUE);
	}

	bool initScripts()
	{
		BOOL bLoaded = FALSE;
#ifdef DLL_CORE
		//加载LUA脚本模块，注意，脚本模块只有在SOUI内核是以DLL方式编译时才能使用。
		bLoaded = getComMrg()->CreateScrpit_Lua((IObjRef**)&pScriptLua);
		SASSERT_FMT(bLoaded,_T("load interface [%s] failed!"),_T("scirpt_lua"));
		pSouiApp->SetScriptFactory(pScriptLua);
#endif//DLL_CORE

		return true;
	}

	bool initExtendControls()
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

	bool initSysResource()
	{
		//加载系统资源
		if(hSysResource == 0) return false;

		BOOL bLoaded = FALSE;
		CAutoRefPtr<IResProvider> sysSesProvider;
		CreateResProvider(RES_PE,(IObjRef**)&sysSesProvider);
		bLoaded = sysSesProvider->Init((WPARAM)hSysResource,0);

		if(bLoaded == TRUE)
		{
			pSouiApp->LoadSystemNamedResource(sysSesProvider);

			//采用hook绘制菜单的边框
			CMenuWndHook::InstallHook(hInstance,_T("_skin.sys.menu.border"));
		}

		return (bLoaded == TRUE);
	}

	bool initRealWnd()
	{
		//设置真窗口处理接口
		CSouiRealWndHandler * pRealWndHandler = new CSouiRealWndHandler();
		pSouiApp->SetRealWndHandler(pRealWndHandler);
		pRealWndHandler->Release();
		return true;
	}

	bool initGlobalStyle()
	{
		//加载全局资源描述XML
		//初始化SOUI全局资源
		return (TRUE == pSouiApp->Init(_T("xml_init"), _T("xml"))); 
	}

private:
	HINSTANCE hInstance;                                //当前模块(exe、dll的句柄)
	HMODULE hSysResource;                               //soui系统资源dll的句柄
	SComMgr comMgrObj;
	CAutoRefPtr<IImgDecoderFactory> pImgDecoderFactory; //图片解码器
	CAutoRefPtr<IRenderFactory> pRenderFactory;         //UI渲染模块,由render-gdi.dll或render-skia.dll提供
	//CAutoRefPtr<ITranslatorMgr> trans;                //多语言翻译模块,由translator.dll提供
	CAutoRefPtr<IScriptFactory> pScriptLua;             //lua脚本模块,由scriptmodule-lua.dll提供
	SApplication* pSouiApp;
};


// CmfcdemoApp 构造

CmfcdemoApp::CmfcdemoApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CmfcdemoApp 对象

CmfcdemoApp theApp;


// CmfcdemoApp 初始化

BOOL CmfcdemoApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	//HRESULT hRes = CoInitialize(NULL);
	//SASSERT(SUCCEEDED(hRes));

 //   {//这一个括号很重要，保证里面声明的局部对象在CoUninitialize()之前释放

 //       SComLoader imgDecLoader;
 //       SComLoader renderLoader;
 //       CAutoRefPtr<IImgDecoderFactory> pImgDecoderFactory;
 //       CAutoRefPtr<IRenderFactory> pRenderFactory;
 //       imgDecLoader.CreateInstance(COM_IMGDECODER,(IObjRef**)&pImgDecoderFactory);
 //       renderLoader.CreateInstance(COM_RENDER_GDI,(IObjRef**)&pRenderFactory);

 //       pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);

 //       SApplication *pSouiApp=new SApplication(pRenderFactory,theApp.m_hInstance);

 //       //加载系统资源
 //       HMODULE hSysResource=LoadLibrary(SYS_NAMED_RESOURCE);
 //       if(hSysResource)
 //       {
 //           CAutoRefPtr<IResProvider> sysSesProvider;
 //           CreateResProvider(RES_PE,(IObjRef**)&sysSesProvider);
 //           sysSesProvider->Init((WPARAM)hSysResource,0);
 //           pSouiApp->LoadSystemNamedResource(sysSesProvider);
 //       }

 //       CAutoRefPtr<IResProvider>   pResProvider;
 //       CreateResProvider(RES_PE,(IObjRef**)&pResProvider);
 //       pResProvider->Init((WPARAM)theApp.m_hInstance,0);

 //       pSouiApp->AddResProvider(pResProvider);    //从资源加载皮肤

 //       //设置真窗口处理接口
 //       CSouiRealWndHandler * pRealWndHandler = new CSouiRealWndHandler();
 //       pSouiApp->SetRealWndHandler(pRealWndHandler);
 //       pRealWndHandler->Release();

 //       pSouiApp->Init(_T("uidef"),_T("xml")); //初始化SOUI全局资源

	//    CmfcdemoDlg dlg;
	//    m_pMainWnd = &dlg;
	//    dlg.DoModal();
 //   	
 //       delete pSouiApp;

 //   }

	//CoUninitialize();

	//将arx程序的实例传递过来
	HINSTANCE hInstance = theApp.m_hInstance;

	SoUILoader suLoader(hInstance);
	if(!suLoader.initRender()) return FALSE;
	if(!suLoader.initApp()) return FALSE;
	if(!suLoader.initSysResource()) return FALSE;
	if(!suLoader.initResource()) return FALSE;
	if(!suLoader.initScripts()) return FALSE;
	if(!suLoader.initExtendControls()) return FALSE;
	if(!suLoader.initRealWnd()) return FALSE;
	if(!suLoader.initGlobalStyle()) return FALSE;

    CmfcdemoDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();
   	
	return FALSE;
}
