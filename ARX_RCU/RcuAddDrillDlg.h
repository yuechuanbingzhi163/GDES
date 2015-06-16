#pragma once
#include "RcuAcUiBaseDlg.h"

//新增钻场对话框
class RcuAddDrillDlg : public RcuAcUiBaseDlg
{
	DECLARE_DYNAMIC(RcuAddDrillDlg)

public:
	RcuAddDrillDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RcuAddDrillDlg();

// 对话框数据
	enum { IDD = IDD_DRILL_NEW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
