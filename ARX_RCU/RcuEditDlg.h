#pragma once

#include "acui.h"
#include "resource.h"

#include "RcuDataLink.h"

//石门揭煤编辑对话框(从"石门设计"右键菜单中调用该对话框)
class RcuEditDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(RcuEditDlg)

public:
	RcuEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RcuEditDlg();

// 对话框数据
	enum { IDD = IDD_RCU_EDIT_DLG };

public:
	//石门图元id
	AcDbObjectId m_rock_gate;
	//从石门中读取数据给对话框
	bool readDataFromRockGate(const AcDbObjectId& rock_gate);
	//从对话框中提取数据并保存到石门图元中
	bool writeDataToRockGate(const AcDbObjectId& rock_gate);

protected:
	//对话框与石门图元之间交换数据
	void exchangeRockGateData(RockGateLink& rg_link, bool save);
	//对话框与煤层图元之间交换数据
	void exchangeCoalSurfaceData(CoalSurfaceLink& cs_link, bool save);
	//对话框与钻场图元之间交换数据
	void exchangeDrillSiteData(DrillSiteLink& ds_link, bool save);
	//在listctrl中查找id等于drill_site的行
	int findDrillSiteRow(const AcDbObjectId& drill_site);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
	double m_x;
	double m_y;
	double m_z;
	double m_length;
	double m_width;
	double m_height;
	double m_left;
	double m_right;
	double m_top;
	double m_bottom;
	double m_radius;
	double m_thick;
	double m_angle;
	double m_dist;
	CListCtrl m_list;

	//"确定"按钮消息
	afx_msg void OnBnClickedOk();
	//listrctrl双击消息
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	//listrctrl右键消息
	afx_msg void OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult);

	//菜单项消息响应
	afx_msg void OnAddCommand();
	afx_msg void OnDeleteCommand();
	afx_msg void OnModifyCommand();
	afx_msg void OnHilightCommand();

	//虚函数重载
	virtual BOOL OnInitDialog();
};
