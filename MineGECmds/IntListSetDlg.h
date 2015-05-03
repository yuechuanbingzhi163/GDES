#pragma once

#include "resource.h"
#include "afxcmn.h"

class IntListSetDlg : public CDialog
{
	DECLARE_DYNAMIC(IntListSetDlg)

public:
	IntListSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~IntListSetDlg();

// 对话框数据
	enum { IDD = IDD_INT_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	CString m_intValue;
	CString m_strValue;

	AcStringArray m_intStrList;
	AcDbIntArray m_intList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedIntStrAdd();
	afx_msg void OnBnClickedIntStrDel();
	afx_msg void OnBnClickedOk();

	CEdit* getEdit(long id);
	bool isValueExist(const CString& value, bool isIntType);	
};
