// regMachineDlg.h : header file
//

#if !defined(AFX_REGMACHINEDLG_H__A2805AFB_9D1D_43F2_B9ED_1ED8402EAFEA__INCLUDED_)
#define AFX_REGMACHINEDLG_H__A2805AFB_9D1D_43F2_B9ED_1ED8402EAFEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRegMachineDlg dialog


#include "Reg.h"
#include "HardWare.h"

class CRegMachineDlg : public CDialog
{
// Construction
public:
	CRegMachineDlg(CWnd* pParent = NULL);	// standard constructor
	~CRegMachineDlg();

public:
	CHardWare m_hWare;
	CReg      m_reg;
	CString   m_strUID;
	CString   m_strReg;
	DISKINFO  m_dInfo;




// Dialog Data
	//{{AFX_DATA(CRegMachineDlg)
	enum { IDD = IDD_REGMACHINE_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegMachineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRegMachineDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGetId();
	afx_msg void OnGetReg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGMACHINEDLG_H__A2805AFB_9D1D_43F2_B9ED_1ED8402EAFEA__INCLUDED_)
