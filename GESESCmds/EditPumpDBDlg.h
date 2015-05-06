#pragma once

#include "Resource.h"
#include "afxcmn.h"

class EditPumpDBDlg : public CDialog
{
	DECLARE_DYNAMIC(EditPumpDBDlg)

public:
	EditPumpDBDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~EditPumpDBDlg();

// 对话框数据
	enum { IDD = IDD_FIND_PUMP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_btype;
	BOOL m_bweight;
	BOOL m_bspeed;
	BOOL m_bmaxp;
	BOOL m_bfactory;
	BOOL m_blength;
	BOOL m_bheight;
	BOOL m_bweidth;
	BOOL m_bminabsp;
	BOOL m_babsp;
	BOOL m_bpower;
	BOOL m_bmaxq;
	BOOL m_ball;
	CString m_type;
	CString m_weight;
	CString m_speed;
	CString m_maxp;
	CString m_factory;
	CString m_length;
	CString m_height;
	CString m_weidth;
	CString m_absp;
	CString m_power;
	CString m_maxq;
	CString m_minabsp;
	CString m_itemsNum;
	CListCtrl m_listCtrl;

protected:
	HICON m_hIcon;

private:
	void SetAllCheckBox();
	void OnInitListCtrl();

public:
	afx_msg void OnBnClickedAllPumpCheck();
	afx_msg void OnBnClickedPumpNameCheck();
	afx_msg void OnBnClickedPumpLengthCheck();
	afx_msg void OnBnClickedPumpAbspCheck();
	afx_msg void OnBnClickedPumpWeightCheck();
	afx_msg void OnBnClickedPumpHeightCheck();
	afx_msg void OnBnClickedPumpPowerCheck();
	afx_msg void OnBnClickedPumpSpeedCheck();
	afx_msg void OnBnClickedPumpWeidthCheck();
	afx_msg void OnBnClickedPumpMaxqCheck();
	afx_msg void OnBnClickedPumpMaxpCheck();
	afx_msg void OnBnClickedPumpMinabspCheck();
	afx_msg void OnBnClickedPumpFactoryCheck();
	afx_msg void OnBnClickedExitButton();
	afx_msg void OnBnClickedUpdatePumpdbButton();
	afx_msg void OnBnClickedFindPump();
};
