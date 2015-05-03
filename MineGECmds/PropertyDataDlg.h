#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "../MineGE/FieldInfo.h"
#include "../MineGE/FieldInfoHelper.h"

//为了避免和PropertyArray类型冲突，使用AcArray定义一个新的数组类型
//用于属性的同步计算
typedef AcArray<CMFCPropertyGridProperty*> AcPropertyArray;

// 属性数据对话框
class PropertyDataDlg : public CDialog
{
	DECLARE_DYNAMIC(PropertyDataDlg)

public:
	PropertyDataDlg(CWnd* pParent = NULL,CString func = _T(""));   // 标准构造函数
	virtual ~PropertyDataDlg();

	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam );
// 对话框数据
	enum { IDD = IDD_PROPERTY_DLG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	CMFCPropertyGridCtrl m_propertyDataList;
	AcDbObjectId m_objId;          // 图元id
	AcStringArray m_fields;        // 要显示的字段
	bool m_showAll;                // 是否显示全部数据(默认true)
	CString m_func;
	AcPropertyArray m_gasProps;           //用于实现瓦斯管相关的同步计算
	AcPropertyArray m_pressureProps;      //用于实现压力相关的同步计算
	AcPropertyArray m_flowProps;          //用于实现瓦斯涌出量相关的同步计算
	bool caculate();
	void CtrlProperty(COleVariant& changedValue);
	void SyncPropertyDatas(const CString& filedName, COleVariant& changedValue,CMFCPropertyGridProperty*pProp );
};
