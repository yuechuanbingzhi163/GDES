#pragma once
#include "afxwin.h"
#include "resource.h"
#include "afxcmn.h"
#include "enumDefine.h"


// FanChooseDlg 对话框
class FanChooseDlg : public CDialog
{
	DECLARE_DYNAMIC(FanChooseDlg)

public:
	FanChooseDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FanChooseDlg();

// 对话框数据
	enum { IDD = IDD_CHOOSE_FAN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();

	chooseType m_chFanType;
};
