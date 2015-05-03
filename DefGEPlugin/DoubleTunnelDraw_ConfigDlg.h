#pragma once

#include "resource.h"
#include "afxwin.h"

// 双线巷道可视化效果配置对话框
class DoubleTunnelDraw_ConfigDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(DoubleTunnelDraw_ConfigDlg)

public:
	DoubleTunnelDraw_ConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DoubleTunnelDraw_ConfigDlg();

// 对话框数据
	enum { IDD = IDD_DOUBLE_TUNNEL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CAcUiNumericEdit m_widthEdit;
	CString m_strWidth;
	
	CComboBox m_jointDrawList;
	int m_jdt;
	afx_msg void OnEnKillfocusWidthEdit();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
