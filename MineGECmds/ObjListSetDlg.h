#pragma once

#include "resource.h"
#include "afxwin.h"

class ObjListSetDlg : public CDialog
{
	DECLARE_DYNAMIC(ObjListSetDlg)

public:
	ObjListSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ObjListSetDlg();

// 对话框数据
	enum { IDD = IDD_OBJ_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStrAdd();
	afx_msg void OnBnClickedStrDel();
	afx_msg void OnBnClickedOk();
	
	bool isStrExistInListBox(const CString& str);
	CListBox m_strListBox;
	AcStringArray m_strList;  // 输入/输出
	virtual BOOL OnInitDialog();
};
