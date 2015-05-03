//--------------------------------------------------------------------------------//
//                                                                                //
//                         头文件名: RegTbOpt.h                                   //
//                     源程序文件名: RegTbOpt.cpp                                 //
//                             类名: CRegTbOpt                                    //
//                             功能: 封装对注册表的操作                           //
//                             作者: 叶俊斌                                       //
//                             日期: 2006.9.6                                     //
//                           版本号: v1.00                                        //
//                                                                                //
//   提示: 默认不使用Debug模式, 如果要使用Debug模式, 把Debug参数设置TRUE就OK了.   //
//                                                                                //
//--------------------------------------------------------------------------------// 

#if !defined(AFX_REGTBOPT_H__7C393DE1_7C0A_4736_BF2D_C93111445B37__INCLUDED_)
#define AFX_REGTBOPT_H__7C393DE1_7C0A_4736_BF2D_C93111445B37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRegTbOpt  
{
public:
	CRegTbOpt();
	virtual ~CRegTbOpt();

public:
	void SetDebugMode( BOOL bDebug );                         // 设置Debug模式

	BOOL CreateRegKey( HKEY hKey, LPCTSTR lpKeyName );        // 创建子键
	
	BOOL OpenRegTable( HKEY hKey, LPCSTR lpSubKey );          // 打开指定的子键

	BOOL ReadRegTable( LPTSTR lpValueName, LPBYTE lpData );   // 读注册表
	
	BOOL WriteRegTable( LPCTSTR lpValueName, DWORD dwType, 
		CONST BYTE *lpData );                                 // 写注册表

	void CloseRegTable();                                     // 关闭注册表

	BOOL RegTbIsOpen();                                       // 判断注册表是否打开

// 私有成员变量
private:
	HKEY   m_hKey;         // 键的句柄
	BOOL   m_bDebug;       // 设置为Debug模式

// 私有成员函数
private:
	void ErrorMessageBox( DWORD dwErrCode );    // 错误信息函数
	

};

#endif // !defined(AFX_REGTBOPT_H__7C393DE1_7C0A_4736_BF2D_C93111445B37__INCLUDED_)
