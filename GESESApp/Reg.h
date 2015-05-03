// Reg.h: interface for the CReg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REG_H__9F2525E4_6C3A_437A_8C92_91677ABF3A2C__INCLUDED_)
#define AFX_REG_H__9F2525E4_6C3A_437A_8C92_91677ABF3A2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 这个类用来实现ID号的数字化、注册码的产生、比较注册码是否正确.
#include <windows.h>
#include <atlstr.h>

class CReg  
{
public:
	CReg();
	virtual ~CReg();

public:
	int CmpReg( CString userID, CString userReg );   // 比效注册码是否正确
	int JmValue(int nValue ,int nSelect);            // 加密
	int UnJmValue(int nJmValue, int nValue);         // 解密
	CString DisPoseID( CString machineNum );         // 把字符ID转换成数字ID
	CString GetRegNum( CString userID );             // 通过硬盘ID或Bios获取注册码.

private:
	char *m_pMachineNum;
	char *m_pRegNum;
};


#endif // !defined(AFX_REG_H__9F2525E4_6C3A_437A_8C92_91677ABF3A2C__INCLUDED_)
