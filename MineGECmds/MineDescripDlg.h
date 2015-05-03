#pragma once
#include "afxcmn.h"
#include "Resource.h"

// MineMainDescipDlg 对话框

class  MineMainDescipDlg : public CDialog
{
	DECLARE_DYNAMIC(MineMainDescipDlg)

public:
	MineMainDescipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MineMainDescipDlg();

	// 对话框数据
	enum { IDD = IDD_MINEDESC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

public:
	CRichEditCtrl m_EditCtrl;
	CRichEditCtrl m_numEditCtrl;
};
