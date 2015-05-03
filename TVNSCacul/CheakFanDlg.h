#pragma once
#include "Resource.h"

// CheakFanDlg 对话框

class CheakFanDlg : public CDialog
{
	DECLARE_DYNAMIC(CheakFanDlg)

public:
	CheakFanDlg(CWnd* pParent = NULL,AcDbObjectId fanId = NULL);   // 标准构造函数
	virtual ~CheakFanDlg();

// 对话框数据
	enum { IDD = IDD_FAN_ENSUR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_fantype;
	CString m_minQ;
	CString m_maxQ;
	CString m_minH;
	CString m_maxH;

	CString m_thearyQ;
	CString m_thearyH;
	
	AcDbObjectId m_fanId;
public:
	void readPropertyData();
	void writePropertyData();

	BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
