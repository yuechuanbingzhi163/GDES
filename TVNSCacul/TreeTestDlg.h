#pragma once
#include "afxcmn.h"
#include "Resource.h"

// TreeTestDlg 对话框

class TreeTestDlg : public CDialog
{
	DECLARE_DYNAMIC(TreeTestDlg)

public:
	TreeTestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TreeTestDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_treeCtrl;
	BOOL m_result;
};
