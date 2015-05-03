// RegTbOpt.cpp: implementation of the CRegTbOpt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Easson2d.h"
#include "RegTbOpt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegTbOpt::CRegTbOpt()
{
	m_bDebug = FALSE;
}

CRegTbOpt::~CRegTbOpt()
{
	if ( m_hKey )                    // 如果句柄不为空
	{
		RegCloseKey( m_hKey );       // 关闭句柄
		m_hKey = NULL;               // 清空句柄变量
	}
}

// 创建注册表键值
BOOL CRegTbOpt::CreateRegKey( HKEY hKey, LPCTSTR lpKeyName )
{
	LONG   nErrCode = 0;             // 错误代码

	nErrCode = RegCreateKey( hKey, lpKeyName, &m_hKey );

	if ( ERROR_SUCCESS == nErrCode )
	{
		return TRUE;
	}

	if ( m_bDebug )                    // 如果为调试模式
	{
		ErrorMessageBox( nErrCode );
	}
	
	return FALSE;                    // 打开键失败
}

// 打开注册表
BOOL CRegTbOpt::OpenRegTable( HKEY hKey, LPCSTR lpSubKey )
{
	LONG   nErrCode = 0;             // 错误代码
	
	nErrCode = RegOpenKey( hKey, lpSubKey, &m_hKey );

	if ( ERROR_SUCCESS == nErrCode )
	{
		return TRUE;                 // 打开键成功
	}

	if ( m_bDebug )                    // 如果为调试模式
	{
		ErrorMessageBox( nErrCode );
	}
	
	return FALSE;                    // 打开键失败
}

// 读注册表
BOOL CRegTbOpt::ReadRegTable( LPTSTR lpValueName, LPBYTE lpData )
{
	DWORD  dwType;                   // 用来接收读出数据的类型, 如: REG_SZ
	DWORD  dwSize;                   // 用来接收读出的数据大小
	LONG   nErrCode = 0;             // 错误代码

	nErrCode = RegQueryValueEx( m_hKey, lpValueName, NULL, &dwType, lpData, 
		&dwSize );                   // 读注册表的值

	if ( ERROR_SUCCESS == nErrCode )
	{
		return TRUE;
	}

	if ( m_bDebug )                    // 如果为调试模式
	{
		ErrorMessageBox( nErrCode );
	}

	return FALSE;
}

// 写注册表
BOOL CRegTbOpt::WriteRegTable( LPCTSTR lpValueName, DWORD dwType, CONST BYTE *lpData )
{
	LONG   nErrCode = 0;             // 错误代码

	nErrCode = RegSetValueEx( m_hKey, lpValueName, NULL, dwType, lpData, 
		sizeof(lpData) );            // 设置注册表的值勤

	if ( ERROR_SUCCESS == nErrCode )
	{
		return TRUE;
	}

	if ( m_bDebug )
	{
		ErrorMessageBox( nErrCode );
	}

	return FALSE;
}

// 关闭注册表
void CRegTbOpt::CloseRegTable()
{
	if ( m_hKey )
	{
		RegCloseKey( m_hKey );       // 关闭注册表
		m_hKey = NULL;               // 注空句柄值
	}
}

// 弹出格式化后捕获的错误代码
void CRegTbOpt::ErrorMessageBox( DWORD dwErrCode )
{
	HLOCAL hLocal = NULL;     // 内存块句柄

	// 格式化捕获的错误代码
	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER , 
		NULL, dwErrCode, MAKELANGID( LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED ),
		(LPTSTR)&hLocal, 0, NULL );

	if ( hLocal != NULL )
	{
		MessageBox( NULL, (LPCTSTR)LocalLock(hLocal), "注册表错误", MB_OK | MB_ICONERROR );
		LocalFree( hLocal );
	}
	else
	{
		MessageBox( NULL, "一个未知的错误", "注册表错误", MB_OK | MB_ICONERROR );
	}
}

void CRegTbOpt::SetDebugMode( BOOL bDebug )
{
	m_bDebug = bDebug;
}

BOOL CRegTbOpt::RegTbIsOpen()
{
	if ( m_hKey )
	{
		return TRUE;
	}
	return FALSE;
}