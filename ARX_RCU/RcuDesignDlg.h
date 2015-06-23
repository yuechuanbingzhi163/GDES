#pragma once

#include "DockBarChildDlg.h"
#include "BtnST.h"
/**
	CButtonST使用简介
	http://www.cnblogs.com/lidabo/archive/2012/12/17/2821122.html
*/

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
	CButtonST m_updBtn;

	//刷新按钮单击消息
	afx_msg void OnBnClickedUpdate();
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

	//响应MyMsg.h中自定义消息
	afx_msg LRESULT OnRcuAddMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRcuDelMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRcuModifyMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRcuUpdateMessage(WPARAM wParam, LPARAM lParam);

	//对话框虚函数重载
	virtual BOOL OnInitDialog();
	virtual void OnClosing();

	//下面的这些函数仅提供给自定义消息映射函数使用
private:
	//根据用户指定的操作更新石门列表
	void updateRockGateListCtrl(unsigned int op, const AcDbObjectId& rock_gate);
	//根据用户指定的操作更新钻场列表
	void updateDrillSiteListCtrl(unsigned int op, const AcDbObjectId& drill_site);
	
	//增加石门
	void addRockGate(const AcDbObjectId& rock_gate);
	//删除石门
	void delRockGate(int row1);
	//修改石门
	void modifyRockGate(int row1);

	//增加钻场
	void addDrillSite(const AcDbObjectId& drill_site);
	//删除钻场
	void delDrillSite(int row2);
	//修改钻场
	void modifyDrillSite(int row2);

	//更新对话框
	void updateUI();
};
