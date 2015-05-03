// FileOpt.cpp: implementation of the CFileOpt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../Easson2d.h"
#include "FileOpt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileOpt::CFileOpt()
{
}

CFileOpt::~CFileOpt()
{
//	if ( m_hFile )
//	{
//		Close();           // 关闭文件
// 	}
}

BOOL CFileOpt::CreateFile( LPCTSTR lpFile )
{
	// 判断文件是否存在
	if ( isExistFile( lpFile ) )
	{
		if ( IDOK == MessageBox( NULL, "文件已存在, 是否覆盖?", "提示", MB_OKCANCEL 
			| MB_ICONINFORMATION ) )
		{
			// 创建并覆盖文件
			Open( lpFile, CFile::modeCreate );
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	// 创建文件
	Open( lpFile, CFile::modeCreate );
	return TRUE;
}

BOOL CFileOpt::OpenFile( LPCTSTR lpFile )
{
	DWORD    dwErrCode = 0;                // 错误代码

	Open( lpFile, CFile::modeReadWrite );
	dwErrCode = GetLastError();

	if ( 0 == dwErrCode )        // 如果文件存在且打开文件成功(都能捕获)
	{
		return TRUE;
	}

	if ( m_bDebug )                        // 是调试模式
	{
		ErrorMessageBox( dwErrCode );
	}

	return FALSE;                // 文件不存在
}

BOOL CFileOpt::FilePointerSeek( UINT nMethod, LONG nCurrent )
{
	DWORD dwErrCode = 0;

	if ( 0 == nCurrent )
	{
		switch ( nMethod )
		{
		case 1:                    // 文件首
			SeekToBegin();
			break;
		case 2:                    // 文件尾
			SeekToEnd();
			break;
		}
		dwErrCode = GetLastError();
	}
	else
	{
		switch ( nMethod )
		{
		case 1:
			Seek( nCurrent, CFile::begin );
			break;
		case 2:
			Seek( nCurrent, CFile::end );
			break;
		}
		dwErrCode = GetLastError();
	}

	if ( 0 == dwErrCode )   // 设置文件成功
	{
		return TRUE;
	}

	if ( m_bDebug )                     // 是调试模式
	{
		ErrorMessageBox( dwErrCode );
	}
	
	return FALSE;
}

BOOL CFileOpt::ReadFile( LPTSTR lpData )
{
	Read( lpData, MAX_BUFF );
	return TRUE;
}

void CFileOpt::CloseFile()
{
	Close();
}

void CFileOpt::SetDebugMode( BOOL bDebug )
{
	m_bDebug = bDebug;
}

BOOL CFileOpt::isExistFile( LPCTSTR lpFile )
{
	DWORD    dwErrCode = 0;                // 错误代码

	Open( lpFile, CFile::modeRead );
	dwErrCode = GetLastError();

	if ( 0 == dwErrCode )        // 如果文件存在且打开文件成功(都能捕获)
	{
		Close();
		return TRUE;
	}

	if ( m_bDebug )                        // 是调试模式
	{
		ErrorMessageBox( dwErrCode );
	}

	return FALSE;                // 文件不存在
}

void CFileOpt::ErrorMessageBox( DWORD dwErrCode )
{
	HLOCAL hLocal = NULL;     // 内存块句柄

	// 格式化捕获的错误代码
	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER , 
		NULL, dwErrCode, MAKELANGID( LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED ),
		(LPTSTR)&hLocal, 0, NULL );

	if ( hLocal != NULL )
	{
		MessageBox( NULL, (LPCTSTR)LocalLock(hLocal), "文件错误", MB_OK | MB_ICONERROR );
		LocalFree( hLocal );
	}
	else
	{
		MessageBox( NULL, "一个未知的错误", "文件错误", MB_OK | MB_ICONERROR );
	}
}

BOOL CFileOpt::WriteFile( LPTSTR lpData )
{
	DWORD dwErrCode = 0;


	Write( lpData, strlen(lpData) );
	dwErrCode = GetLastError();

	if ( 0 == dwErrCode )      // 写入成功
	{
		return TRUE;
	}

	if ( m_bDebug )                        // 是调试模式
	{
		ErrorMessageBox( dwErrCode );
	}

	return FALSE;              // 写入失败
}