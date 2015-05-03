#pragma once

#include "afxwin.h"
#include "resource.h"

class StrListSetDlg : public CDialog
{
	DECLARE_DYNAMIC(StrListSetDlg)

public:
	StrListSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~StrListSetDlg();

// 对话框数据
	enum { IDD = IDD_STR_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStrAdd();
	afx_msg void OnBnClickedStrDel();
	afx_msg void OnBnClickedOk();

public:
	bool isStrExistInListBox(const CString& str);
	CListBox m_strListBox;

	AcStringArray m_strList;  // 输入/输出
};
