#pragma once
#include "afxwin.h"
#include "Resource.h"

// WcyPropertyDlg 对话框

class WcyPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(WcyPropertyDlg)

public:
	WcyPropertyDlg(CWnd* pParent = NULL,CString func = _T(""));   // 标准构造函数
	virtual ~WcyPropertyDlg();

// 对话框数据
	enum { IDD = IDD_PROPERTY_DLG2 };

	// 关联图元
	void setMineGE(const AcDbObjectId& objId);

	// 添加字段
	void addField(const CString& field);

	// 是否显示全部数据
	void showAllData(bool bFlag);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	BOOL InitPropGridCtrl();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeCaculMathodCombo();

private:
	CComboBox m_method;
	CMFCPropertyGridCtrl m_propertyDataList;
	AcDbObjectId m_objId;          // 图元id
	AcStringArray m_fields;        // 要显示的字段
	bool m_showAll;                // 是否显示全部数据(默认true)
	CString m_func;

	bool caculate();
};
