#pragma once

#include "Resource.h"
#include "afxwin.h"

class DataObjectStateDlg : public CDialog
{
	DECLARE_DYNAMIC(DataObjectStateDlg)

public:
	DataObjectStateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DataObjectStateDlg();

// 对话框数据
	enum { IDD = IDD_OBJECT_STATE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	void updateTypeList();
	void updateTypeList2();
	void updateValueList();

private:
	CComboBox m_typeList;
	CComboBox m_typeList2;
	CComboBox m_valueList;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeTypeList();
	afx_msg void OnBnClickedShowBtn();
	afx_msg void OnBnClickedShowBtn2();
};
