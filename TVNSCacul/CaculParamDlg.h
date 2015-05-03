#pragma once
#include "afxwin.h"
#include "Resource.h"

// CaculParamDlg 对话框

class CaculParamDlg : public CDialog
{
	DECLARE_DYNAMIC(CaculParamDlg)

public:
	CaculParamDlg(CWnd* pParent = NULL,AcDbObjectId objId = NULL);   // 标准构造函数
	virtual ~CaculParamDlg();

// 对话框数据
	enum { IDD = IDD_CACUL_PARAM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void readPropertyData();
	void writePropertyData();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

public:
	CString m_time;
	CString m_speed;
	CString m_A;
	CString m_N;
	CComboBox m_dynamiteWay;
	CString m_g;
	CString m_q0;
	CString m_PN;
	CString m_C1;
	CString m_inT;
	CString m_outT;
	CString m_allPower;
	CString m_Cp;
	CString m_ro;

private:
	AcDbObjectId m_objId;
public:
	afx_msg void OnEnKillfocusWindSpeedEdit();
	afx_msg void OnEnSetfocusWindSpeedEdit();
};
