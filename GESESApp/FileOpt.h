// FileOpt.h: interface for the CFileOpt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEOPT_H__B3894791_C188_4455_A0DB_A911F67D3F32__INCLUDED_)
#define AFX_FILEOPT_H__B3894791_C188_4455_A0DB_A911F67D3F32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_BUFF 255

#include <afx.h>
class CFileOpt : public CStdioFile 
{
public:
	CFileOpt();
	virtual ~CFileOpt();

public:
	void SetDebugMode( BOOL bDebug );        // 设置是否为调试模式
	void CloseFile();                        // 关闭文件
	BOOL isExistFile( LPCTSTR lpFile );      // 文件存在返回真,不存在返回假
	BOOL CreateFile( LPCTSTR lpFile );       // 创建文件
	BOOL OpenFile( LPCTSTR lpFile );         // 以读/写方式打开文件
	BOOL WriteFile( LPTSTR lpData );         // 写文件
	BOOL ReadFile( LPTSTR lpData );          // 读文件
	BOOL FilePointerSeek( UINT nMethod, 
		LONG nCurrent = 0 );                 // 在打开文件后设置文件指针位置

private:
	BOOL     m_bDebug;    // 调试模试

private:
	void ErrorMessageBox( DWORD dwErrCode );

};;

#endif // !defined(AFX_FILEOPT_H__B3894791_C188_4455_A0DB_A911F67D3F32__INCLUDED_)
