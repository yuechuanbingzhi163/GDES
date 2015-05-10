#include "StdAfx.h"
#include "UIHelper.h"

#include "SoUILoader.h"
#include "realwnddlg.h"
#include "SouiRealWndHandler.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

using namespace SOUI;

class MySoUiLoader1 : public SoUILoader
{
public:
	MySoUiLoader1(HINSTANCE hInstance) : SoUILoader(hInstance)
	{

	}

	~MySoUiLoader1() 
	{
	}

	virtual LPCTSTR getSkinDir() const
	{
		return _T("\\..\\..\\Skin\\demo1");
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
		return (TRUE == pSouiApp->Init(_T("uidef"), _T("xml"))); 
	}
};

void UIHelper::ShowSoUIDlg1()
{
	// 切换资源
	CAcModuleResourceOverride myResources;

	//加载soui相关的组建和资源
	MySoUiLoader1 suLoader(_hdllInstance);
	if(!suLoader.init()) return;

	CRealWndDlg dlg;
	dlg.DoModal();
}