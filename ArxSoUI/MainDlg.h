/**
* Copyright (C) 2014-2050 
* All rights reserved.
* 
* @file       MainDlg.h
* @brief      
* @version    v1.0      
* @author     SOUI group   
* @date       2014/08/15
* 
* Describe    主窗口实现
*/

#pragma once

#include <SqliteHelper/SqliteHelper.h>
struct DBDatas 
{
	CString type;
	CString weight;
	CString speed;
	CString maxp;
	CString factory;
	CString length;
	CString height;
	CString weidth;
	CString absp;
	CString power;
	CString maxq;
	CString minabsp;

	void CopyFromType(const TypeTable& tt)
	{
		type = tt.type;
		minabsp.Format(_T("%d"), tt.absP);
		weight.Format(_T("%d"), tt.weight);
		length.Format(_T("%d"), tt.length);
		weidth.Format(_T("%d"), tt.weidth);
		height.Format(_T("%d"), tt.heigth);
		factory = tt.factName;
	}

	void CopyFromProperty(const PropertyTable& pt)
	{
		speed.Format(_T("%d"), pt.speed);
		power.Format(_T("%.1f"), pt.power);
		maxq.Format(_T("%.2f"), pt.maxQ);
		maxp.Format(_T("%d"), pt.maxP);
		absp.Format(_T("%d"), pt.absP);
	}

	void Print()
	{
		acutPrintf(_T("\n类型:%s,最低吸入绝压:%s,泵重:%s,长:%s,宽:%s,高:%s,厂家:%s,转速:%s,电机功率:%s,最大气量:%s,极限压力:%s,吸入绝压:%s"),
			type,minabsp,weight,length,weidth,height,factory,speed,power,maxq,maxp,absp);
	}
};

typedef std::vector<DBDatas> DBDatasVector;

/**
* @class      CMainDlg
* @brief      主窗口实现
* 
* Describe    非模式窗口从SHostWnd派生，模式窗口从SHostDialog派生
*/
class CMainDlg : public SOUI::SHostWnd
{
public:

    /**
     * CMainDlg
     * @brief    构造函数
     * Describe  使用uires.idx中定义的maindlg对应的xml布局创建UI
     */    
	CMainDlg() : SOUI::SHostWnd(_T("layout:maindlg")),m_bLayoutInited(FALSE)
    {
    } 

protected:
    //////////////////////////////////////////////////////////////////////////
    //  Window消息响应函数
    LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);
    void OnDestory();

	void OnClose()
	{
		AnimateHostWindow(200,AW_CENTER|AW_HIDE);
        PostMessage(WM_QUIT);
	}
	void OnMaximize()
	{
		GetNative()->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE);
	}
	void OnRestore()
	{
		GetNative()->SendMessage(WM_SYSCOMMAND,SC_RESTORE);
	}
	void OnMinimize()
	{
		GetNative()->SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
	}

	void OnSize(UINT nType, SOUI::CSize size)
	{
		SetMsgHandled(FALSE);   //这一行很重要，保证消息继续传递给SHostWnd处理，当然也可以用SHostWnd::OnSize(nType,size);代替，但是这里使用的方法更简单，通用
		if(!m_bLayoutInited) return;
		if(nType==SIZE_MAXIMIZED)
		{
			FindChildByID(3)->SetVisible(TRUE);
			FindChildByID(2)->SetVisible(FALSE);
		}else if(nType==SIZE_RESTORED)
		{
			FindChildByID(3)->SetVisible(FALSE);
			FindChildByID(2)->SetVisible(TRUE);
		}
	}
    
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnShowWindow(BOOL bShow, UINT nStatus);
    
    //演示如何在应用层使用定时器
    void OnTimer(UINT_PTR idEvent);

    //DUI菜单响应函数
    void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl);
        

protected:
    //////////////////////////////////////////////////////////////////////////
    // SOUI事件处理函数
	//演示屏蔽指定edit控件的右键菜单
	BOOL OnEditMenu(SOUI::CPoint pt)
	{
		return TRUE;
	}

    //按钮控件的响应
    void OnBtnSelectGIF();
    void OnBtnMenu();

    void OnBtnInsertGif2RE();
    
    void OnBtnMsgBox();
    
    //演示如何使用subscribeEvent来不使用事件映射表实现事件响应
    bool OnListHeaderClick(SOUI::EventArgs *pEvt);

    //演示如何响应如listboxex,treeboxex中的item中包含控件的事件。
    void OnListBoxExEvent(SOUI::EventArgs *pEvt);
    void OnTreeBoxEvent(SOUI::EventArgs *pEvt);
    
    void OnTreeBoxQueryItemHeight(SOUI::EventArgs * pEvt);
    
	//点击泵查询对话框中的"查询"按钮
	void OnBtnFindPump();
	void OnBtnUpdatePump();

    //UI控件的事件及响应函数映射表
	EVENT_MAP_BEGIN()
		EVENT_ID_COMMAND(1, OnClose)
		EVENT_ID_COMMAND(2, OnMaximize)
		EVENT_ID_COMMAND(3, OnRestore)
		EVENT_ID_COMMAND(5, OnMinimize)
		EVENT_NAME_CONTEXTMENU(L"edit_1140",OnEditMenu)
		EVENT_NAME_COMMAND(L"btn_msgbox",OnBtnMsgBox)
		EVENT_NAME_COMMAND(L"btnSelectGif",OnBtnSelectGIF)
        EVENT_NAME_COMMAND(L"btn_menu",OnBtnMenu)
//      EVENT_NAME_COMMAND(L"btn_insert_gif",OnBtnInsertGif2RE)
        EVENT_ID_COMMAND(STR2ID(L"id_test1"),OnBtnInsertGif2RE) //在init.xml中增加支持id表，这一行和上面一行效果一样。
        EVENT_NAME_HANDLER(L"lbe_test",EVT_OFPANEL,OnListBoxExEvent)//响应EVT_OFPANEL事件
        EVENT_NAME_HANDLER(L"tb_test",EVT_OFPANEL,OnTreeBoxEvent)//响应EVT_OFPANEL事件
        EVENT_NAME_HANDLER(L"tb_test",EVT_TB_QUERYITEMHEIGHT,OnTreeBoxQueryItemHeight)//响应动态查询高度事件
	
		EVENT_NAME_COMMAND(L"findPumpBtn",OnBtnFindPump)
		EVENT_NAME_COMMAND(L"updatePumpBtn",OnBtnUpdatePump)
	EVENT_MAP_END()	

    //HOST消息及响应函数映射表
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_CREATE(OnCreate)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_DESTROY(OnDestory)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		MSG_WM_COMMAND(OnCommand)
		MSG_WM_SHOWWINDOW(OnShowWindow)
		CHAIN_MSG_MAP(SOUI::SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

protected:
    //////////////////////////////////////////////////////////////////////////
    //  辅助函数
    void InitListCtrl();
	bool GetEditContents(TypeTable& tt,PropertyTable& pt);
	//判断泵查询对话框的编辑框是否有为空的
	bool EditsHasEmpty();
	bool FindPumpsByCondition( DBDatasVector& datasV );
	int CheckBoxTable();
	bool DeletePump( const DBDatas& datas );
	void OnlyTypesql(CString& ttsql,CString& msg);
	void OnlyPropertysql(CString& ptsql,CString& msg);
	int GetCheckBoxNum();
	void InitPumpCheckBox();
	void UpdateList( const DBDatasVector& datasV );

private:
	BOOL			m_bLayoutInited;/**<UI完成布局标志 */

	SCheckBox *pTypeCheck;
	SCheckBox *pFactoryCheck;
	SCheckBox *pSpeedCheck;
	SCheckBox *pPowerCheck;
	SCheckBox *pLengthCheck; 
	SCheckBox *pWidthCheck;
	SCheckBox *pHeightCheck; 
	SCheckBox *pWeightCheck;
	SCheckBox *pAbsPCheck;
	SCheckBox *pMaxPCheck; 
	SCheckBox *pMinPCheck; 
	SCheckBox *pMaxQCheck;
};