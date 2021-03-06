#include "StdAfx.h"
#include "UIHelper.h"
#include "RcuDesiginDockBarDlg.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

RcuDesiginDockBarDlg* pRcuDesiginDockBarDlg = 0;

void UIHelper::ShowRcuDesignDockBar()
{
	CAcModuleResourceOverride myResources;

	CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();

	if( pRcuDesiginDockBarDlg == 0 )
	{
		pRcuDesiginDockBarDlg = new RcuDesiginDockBarDlg();
		pRcuDesiginDockBarDlg->Create ( pAcadFrame, _T("石门揭煤设计") ) ;
		pRcuDesiginDockBarDlg->EnableDocking ( CBRS_ALIGN_ANY ) ;
		pRcuDesiginDockBarDlg->RestoreControlBar () ;
	}
	pAcadFrame->ShowControlBar( pRcuDesiginDockBarDlg, TRUE, FALSE );

	//刷新界面
	UIHelper::SendMessage(WM_RCU_UPDATE, 0);
}

void UIHelper::DestroyRcuDesignDockBar()
{
	if( pRcuDesiginDockBarDlg != 0 )
	{
		pRcuDesiginDockBarDlg->DestroyWindow();
		delete pRcuDesiginDockBarDlg;
		pRcuDesiginDockBarDlg = 0;
	}
}

void UIHelper::SendMessage(unsigned int msgCode, ArxMsg* msgParam)
{
	if(pRcuDesiginDockBarDlg != 0)
	{
		pRcuDesiginDockBarDlg->mChildDlg.SendMessage(msgCode, (WPARAM)msgParam, 0);
	}
}
