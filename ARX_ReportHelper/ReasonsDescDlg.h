#pragma once

#include "Resource.h"

class ReasonsDescDlg : public CDialog
{
	DECLARE_DYNAMIC(ReasonsDescDlg)

public:
	ReasonsDescDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ReasonsDescDlg();

// 对话框数据
	enum { IDD = IDD_REASONS_DESC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	// 结论描述
	CString m_reasonsDesc;
	BOOL m_yesOrNo;
	afx_msg void OnBnClickedOk();
private:
	HICON m_hIcon;
};
