#pragma once
#include "AcUiBaseDlg.h"

//新增钻场对话框
class AddDrillDlg : public AcUiBaseDlg
{
	DECLARE_DYNAMIC(AddDrillDlg)

public:
	AddDrillDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AddDrillDlg();

// 对话框数据
	enum { IDD = IDD_DRILL_NEW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
