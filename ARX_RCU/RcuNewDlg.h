#pragma once

#include "acui.h"
#include "resource.h"

// RcuDlg 对话框

class RcuNewDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(RcuNewDlg)

public:
	RcuNewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RcuNewDlg();

// 对话框数据
	enum { IDD = IDD_RCU_NEW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
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
	double m_thick;
	double m_angle;
	double m_dist;
	double m_radius;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
};
