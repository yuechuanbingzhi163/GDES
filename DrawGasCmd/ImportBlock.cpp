#include "stdafx.h"

#include <ctime>
#include "../ArxHelper/HelperClass.h"

// 更新瓦斯地质图块定义
// 从当前模块的子目录下加载
void UpdateDwgBlock()
{
    //CString dwgFilePath = _T("C:\\Users\\anheihb03dlj\\Desktop\\瓦斯地质图块定义.dwg");
	CString dwgFilePath = ArxUtilHelper::BuildPath(ArxUtilHelper::GetAppPathDir(_hdllInstance), _T( "Datas\\瓦斯地质图块定义.dwg" ));
	if( ArxDwgHelper::InsertDwg( dwgFilePath ) )
    {
        acutPrintf( _T( "\n更新块成功!\n" ) );
		ArxDwgHelper::UpdateDwg(); // 更新图形
    }
    else
    {
        acutPrintf( _T( "\n更新块失败!\n" ) );
    }
}