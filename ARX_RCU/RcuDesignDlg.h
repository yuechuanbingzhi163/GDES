#pragma once

#include "DockBarChildDlg.h"

//石门揭煤设计主对话框
class RcuDesignDlg : public DockBarChildDlg
{
	DECLARE_DYNAMIC(RcuDesignDlg)

public:
	RcuDesignDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RcuDesignDlg();

// 对话框数据
	enum { IDD = IDD_RCU_DESIGN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//石门列表
	CListCtrl m_list;
	//钻场列表
	CListCtrl m_list2;

	//导出按钮单击消息
	afx_msg void OnBnClickedExport();
	//石门列表行切换时触发的消息
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	//石门列表双击消息
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	//石门列表右键消息
	afx_msg void OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	//钻场列表双击消息
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	//钻场列表右键消息
	afx_msg void OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult);

	//石门列表右键菜单项消息响应
	afx_msg void OnHilightRockGateCommand();
	afx_msg void OnModifyRockGateCommand();
	afx_msg void OnDeleteRockGateCommand();
	afx_msg void OnAddRockGateCommand();

	//钻场列表右键菜单项消息响应
	afx_msg void OnAddDrillSiteCommand();
	afx_msg void OnDeleteDrillSiteCommand();
	afx_msg void OnModifyDrillSiteCommand();
	afx_msg void OnHilightDrillSiteCommand();

	//对话框虚函数重载
	virtual BOOL OnInitDialog();
	virtual void OnClosing();
	//刷新界面
	virtual void update();

private:
	//更新石门列表
	void updateRockGateListCtrl();
};
