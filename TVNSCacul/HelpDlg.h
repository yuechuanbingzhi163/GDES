#pragma once


#include "PropertyData_DockBarChildDlg.h"

class HelpDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(HelpDlg)

public:
	HelpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~HelpDlg();

// 对话框数据
	enum { IDD = IDD_HELP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
