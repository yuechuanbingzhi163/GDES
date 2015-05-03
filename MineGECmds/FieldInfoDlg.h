#pragma once

#include "resource.h"
#include "afxwin.h"

#include "../MineGE/FieldInfo.h"

// FieldInfoDlg 对话框
class FieldInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(FieldInfoDlg)

public:
	FieldInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FieldInfoDlg();

// 对话框数据
	enum { IDD = IDD_FIELD_INFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedUseList();
	afx_msg void OnBnClickedSetDef();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedDisplay();
	afx_msg void OnCbnSelchangeVarNameList();

public:
	// 在对话框显示之前调用
	// dlg.readFromFieldInfo(info);
	// dlg.showwindow() 或者dlg.domodal()
	void readFromFieldInfo(const FieldInfo& info);

	// 从对话框中提取FieldInfo信息
	void writeToFieldInfo(FieldInfo& info);

	// 验证数据格式及其有效性
	bool validateFieldInfo();

private:
	bool validDataformat(const CString& str, bool isIntType=true);
	bool validMinMaxValue(long id, bool isIntType = true);
	bool compareMinMaxValue(long id1, long id2, bool isIntType = true);
	bool checkEmptyValue(long id1);
	void displayControls();
	void fillVarList();
	CString getSelVarName();	
	
	int m_dt;
	
	double m_dMinValue;   // 浮点数区间
	double m_dMaxValue;
	
	int m_nMinValue;      // 整数区间
	int m_nMaxValue;
	
	BOOL m_unEnable;
	CString m_descr;

	int m_lt;
	CString m_varName;

	CComboBox m_varList;

	BOOL m_showInTooltip;
public:
	afx_msg void OnBnClickedRadio6();
};
