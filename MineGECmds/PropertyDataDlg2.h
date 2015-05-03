#pragma once

#include "afxcmn.h"
#include "resource.h"

#include "ListCtrlEx.h"

class PropertyDataDlg2 : public CDialog
{
	DECLARE_DYNAMIC(PropertyDataDlg2)

public:
	PropertyDataDlg2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PropertyDataDlg2();

// 对话框数据
	enum { IDD = IDD_PROPERTY_DLG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	// 关联图元
	void setMineGE(const AcDbObjectId& objId);

	// 添加字段
	void addField(const CString& field);

	// 是否显示全部数据
	void showAllData(bool bFlag);

public:
	AcDbObjectId m_objId;          // 图元id
	AcStringArray m_fields;        // 要显示的字段
	bool m_showAll;                // 是否显示全部数据(默认true)

	CListCtrlEx m_propertyDataList;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
