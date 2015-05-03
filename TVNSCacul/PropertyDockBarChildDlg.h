#pragma once

#include "afxpropertygridctrl.h"
#include "PropertyData_DockBarChildDlg.h"
#include "afxwin.h"

class PropertyDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(PropertyDockBarChildDlg)

public:
	PropertyDockBarChildDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PropertyDockBarChildDlg();

	afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM);
// 对话框数据
	enum { IDD = IDD_PROPERTY_GRID_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	//// 关联图元
	//void setMineGE(const AcDbObjectId& objId);

	//// 添加字段
	//void addField(const CString& field);

	void readPropertyData();
	void writePropertyData();

private:
	CMFCPropertyGridCtrl m_propertyDataList;
	//AcDbObjectId m_objId;          // 图元id
	//AcStringArray m_fields;        // 要显示的字段
};
