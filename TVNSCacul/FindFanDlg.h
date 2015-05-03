//#pragma once
//#include "afxwin.h"
//#include "resource.h"
//#include "enumDefine.h"
//#include "afxcmn.h"
//// FindFanDlg 对话框
////typedef AcArray<int> intArray;
//
//class FindFanDlg : public CDialog
//{
//	DECLARE_DYNAMIC(FindFanDlg)
//
//public:
//	FindFanDlg(CWnd* pParent = NULL);   // 标准构造函数
//	virtual ~FindFanDlg();
//
//// 对话框数据
//	enum { IDD = IDD_FIND_FAN_DIALOG };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//
//	virtual BOOL OnInitDialog();
//	DECLARE_MESSAGE_MAP()
//private:
//	CString m_name;
//	CString m_minQ;
//	CString m_maxQ;
//	CString m_minH;
//	CString m_maxH;
//	CString m_power;
//	CComboBox m_methodComBox;
//
//	CButton *m_minQBtn;
//	CButton *m_nameBtn;
//	CButton *m_maxQBtn;
//	CButton *m_minHBtn;
//	CButton *m_maxHBtn;
//	CButton *m_powerBtn;
//	CButton *m_allBtn;
//
//	CEdit *m_nameEdit;
//	CEdit *m_minQEdit;
//	CEdit *m_maxQEdit;
//	CEdit *m_minHEdit;
//	CEdit *m_maxHEdit;
//	CEdit *m_powerEdit;
//
//	int m_selItem;
//	AcStringArray m_oldValues;
//
//	afx_msg void OnBnClickedBynameCheck();
//	afx_msg void OnBnClickedByminqCheck();
//	afx_msg void OnBnClickedBymaxqCheck();
//	afx_msg void OnBnClickedByminhCheck();
//	afx_msg void OnBnClickedBymaxhCheck();
//	afx_msg void OnBnClickedByallCheck();
//	afx_msg void OnBnClickedBypowerCheck();
//	afx_msg void OnBnClickedFind();
//
//	void setCountFans();
//	CString getFindFansSQL(intArray cheakedIDs);
//	intArray getCheckedIDs();
//	void listOnInit();
//	void deleteFan();
//
//	void getSelectedValue();
//public:
//	CListCtrl m_List;
//	afx_msg void OnNMClickFindRetList(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnCbnSelchangeFindMethodCombo();
//	afx_msg void OnNMDblclkFindRetList(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMKillfocusFindRetList(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnBnClickedUpdateDbButton();
//	afx_msg void OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult);
//};
