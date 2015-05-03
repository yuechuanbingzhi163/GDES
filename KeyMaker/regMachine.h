// regMachine.h : main header file for the REGMACHINE application
//

#if !defined(AFX_REGMACHINE_H__B8A045FA_60FD_4C19_B7C5_81978D646F5D__INCLUDED_)
#define AFX_REGMACHINE_H__B8A045FA_60FD_4C19_B7C5_81978D646F5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRegMachineApp:
// See regMachine.cpp for the implementation of this class
//

#include "HardWare.h"
#include "Reg.h"

class CRegMachineApp : public CWinApp
{
public:
	CRegMachineApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegMachineApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRegMachineApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGMACHINE_H__B8A045FA_60FD_4C19_B7C5_81978D646F5D__INCLUDED_)
