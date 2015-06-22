#pragma once

#include "RcuAcUiBaseDlg.h"
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

private:
	void initRockGateListCtrl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	CListCtrl m_list2;

	//输出按钮消息
	afx_msg void OnBnClickedExport();
	//列表选中的行切换时触发消息
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	//listctrl双击消息
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	//listctrl右键消息(可以弹出菜单，但无法响应菜单消息)
	//因为cad的主窗口总是试图抢夺非模态对话框的焦点(WM_ACAD_KEEPFOCUS)
	//在消息映射暂时屏蔽右键消息
	afx_msg void OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	//listrctrl双击消息
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	//listrctrl右键消息
	afx_msg void OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult);

	//菜单项消息响应
	afx_msg void OnAddDrillSiteCommand();
	afx_msg void OnDeleteDrillSiteCommand();
	afx_msg void OnModifyDrillSiteCommand();
	afx_msg void OnHilightDrillSiteCommand();

	//菜单项消息响应
	afx_msg void OnHilightRockGateCommand();
	afx_msg void OnModifyRockGateCommand();
	afx_msg void OnDeleteRockGateCommand();
	afx_msg void OnAddRockGateCommand();

	//虚函数重载
	virtual BOOL OnInitDialog();
	virtual void OnClosing();
};
