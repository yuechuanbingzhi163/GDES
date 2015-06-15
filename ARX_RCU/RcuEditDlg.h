#pragma once

#include "acui.h"
#include "resource.h"

#include "RcuDataLink.h"

// RcuDesignDlg 对话框

class RcuEditDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(RcuEditDlg)

public:
	RcuEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RcuEditDlg();

// 对话框数据
	enum { IDD = IDD_RCU_EDIT_DLG };

public:
	AcDbObjectId rock_gate;
	bool readData();
	bool writeData();

protected:
	void exchangeRockGateData(RockGateLink& rg_link, bool save);
	void exchangeCoalSurfaceData(CoalSurfaceLink& cs_link, bool save);
	void exchangeDrillSiteData(DrillSiteLink& ds_link, bool save);
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
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddCommand();
	afx_msg void OnDeleteCommand();
	afx_msg void OnModifyCommand();
	afx_msg void OnHilightCommand();
};
