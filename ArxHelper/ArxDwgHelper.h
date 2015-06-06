#pragma once

#include "dlimexp.h"

/**
* dwg数据库辅助操作类.
*/
class ARXHELPER_DLLIMPEXP ArxDwgHelper
{
public:
	//插入dwg文件
	static bool InsertDwg( const CString& dwgFilePath);

	//更新图形
	static void UpdateDwg();

	//获取模块运行路径
	static CString GetAppPathDir(HINSTANCE hInstance);

	//构造路径
	static CString BuildPath( const CString& dir, const CString& fileName );
};
