#pragma once
#include "PropertyData_DockBarChildDlg.h"
#include "afxwin.h"

// FanMethoChoosedDlg 对话框

class FanMethoChoosedDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(FanMethoChoosedDlg)

public:
	FanMethoChoosedDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FanMethoChoosedDlg();

// 对话框数据
	enum { IDD = IDD_FAN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_method;

protected:
	void readPropertyData();
	void writePropertyData();

public:
	virtual BOOL OnInitDialog();

	CString m_fanRank;
private:
	CString m_ventMethod;
public:
	CString m_ventWay;
};
