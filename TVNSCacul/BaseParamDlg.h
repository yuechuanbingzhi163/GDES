#pragma once

#include "Resource.h"
class BaseParamDlg : public CDialog
{
	DECLARE_DYNAMIC(BaseParamDlg)

public:
	BaseParamDlg(CWnd* pParent = NULL,AcDbObjectId objId = NULL);   // 标准构造函数
	virtual ~BaseParamDlg();

	// 对话框数据
	enum { IDD = IDD_PARAM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
	CString m_district;
	CComboBox m_method;
	CComboBox m_way;

private:
	AcDbObjectId m_objId;

public:
	void readPropertyData();
	void writePropertyData();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
