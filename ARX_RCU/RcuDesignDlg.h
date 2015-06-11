#pragma once

#include "acui.h"
#include "resource.h"

// RcuDesignDlg 对话框

class RcuDesignDlg : public CAcUiDialog
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
	CListCtrl m_rcuList;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};
