#pragma once

#include "FieldInfoDlg.h"

// 图元字段管理对话框
class FieldManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(FieldManagerDlg)

public:
	FieldManagerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FieldManagerDlg();

// 对话框数据
	enum { IDD = IDD_FIELD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_typeCombo;
	CListBox m_fieldListBox;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAddBtn();
	afx_msg void OnBnClickedDelBtn();
	afx_msg void OnCbnSelchangeTypeCombo2();
	afx_msg void OnBnClickedApplyBtn();
	afx_msg void OnLbnSelchangeFieldList();
	afx_msg void OnClose(); // 善后工作(销毁指针)
	afx_msg void OnPaint();

private:
	void clearFieldInfoArray();
	void showFieldInfo();
	void clearFieldListBox();
	void fillContent();
	void fillFieldListBox(const CString& type);
	CString getSelType();
	CString getSelField();
	bool isFieldExistInListBox(const CString& field);      // 检查字段是否已存在于fieldlistbox中
	bool isValidField(const CString& field);               // 检查字段是否有效(有待修复)
	void setLastSelIndex(int sel);
	int getLastSelIndex() const;
	bool updateFieldInfo();
	void classNameTrans();

	FieldInfoDlg fidlg;
	AcArray<FieldInfo*> m_infoes;
	int m_lastSel;            // 记录listbox切换之前的索引位置
	CStringArray m_namesEn,m_namesZh;//图元类名及其对应的中文名

};
