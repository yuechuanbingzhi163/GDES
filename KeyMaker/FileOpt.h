//--------------------------------------------------------------------------------//
//                                                                                //
//                         头文件名: FileOpt.h                                    //
//                     源程序文件名: FileOpt.cpp                                  //
//                             类名: CFileOpt                                     //
//                             功能: 封装对文件的操作                             //
//                             作者: 叶俊斌                                       //
//                             日期: 2006.9.6                                     //
//                           版本号: v1.00                                        //
//                                                                                //
//   提示: 默认不使用Debug模式, 如果要使用Debug模式, 把Debug参数设置TRUE就OK了.   //
//                                                                                //
//--------------------------------------------------------------------------------// 

#if !defined(AFX_FILEOPT_H__3CA90917_1AAB_4221_A2D1_FD9452F26904__INCLUDED_)
#define AFX_FILEOPT_H__3CA90917_1AAB_4221_A2D1_FD9452F26904__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_BUFF 255

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

};

#endif // !defined(AFX_FILEOPT_H__3CA90917_1AAB_4221_A2D1_FD9452F26904__INCLUDED_)
