#pragma once

#define RES_TYPE 0   //从文件中加载资源
// #define RES_TYPE 1   //从PE资源中加载UI资源
// #define RES_TYPE 2   //从zip包中加载资源

#ifdef _DEBUG
#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
#else
#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#endif

using namespace SOUI;

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

	virtual ~SoUILoader()
	{
		hInstance = 0; hSysResource = 0;
		delete pSouiApp; pSouiApp = 0;
		CoUninitialize();
	}

	SComMgr* getComMrg()
	{
		return &comMgrObj;
	}

	bool init()
	{
		if(!initRender()) return false;
		if(!initApp()) return false;
		if(!initSysResource()) return false;
		if(!initResource()) return false;
		if(!initScripts()) return false;
		if(!initExtendControls()) return false;
		if(!initRealWnd()) return false;
		if(!initGlobalStyle()) return false;
		return true;
	}

	virtual LPCTSTR getSkinDir() const
	{
		return _T(".");
	}

	virtual bool initRender()
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

	virtual bool initApp()
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

	virtual bool initResource()
	{
		//定义一个资源提供对象,SOUI系统中实现了3种资源加载方式
		//分别是从文件加载，从EXE的资源加载及从ZIP压缩包加载
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

	virtual bool initScripts()
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

	virtual bool initExtendControls()
	{
		return true;
	}

	virtual bool initSysResource()
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

	virtual bool initRealWnd()
	{
		return true;
	}

	virtual bool initGlobalStyle()
	{
		//加载全局资源描述XML
		//初始化SOUI全局资源
		return (TRUE == pSouiApp->Init(_T("xml_init"), _T("xml"))); 
	}

protected:
	HINSTANCE hInstance;                                //当前模块(exe、dll的句柄)
	HMODULE hSysResource;                               //soui系统资源dll的句柄
	SComMgr comMgrObj;                                  // 组件加载管理器
	CAutoRefPtr<IImgDecoderFactory> pImgDecoderFactory; //图片解码器
	CAutoRefPtr<IRenderFactory> pRenderFactory;         //UI渲染模块,由render-gdi.dll或render-skia.dll提供
	//CAutoRefPtr<ITranslatorMgr> trans;                //多语言翻译模块,由translator.dll提供
	CAutoRefPtr<IScriptFactory> pScriptLua;             //lua脚本模块,由scriptmodule-lua.dll提供
	SApplication* pSouiApp;
};
