#include "stdafx.h"

#include "DataInitHelper.h"

// 配置文件
#include "../LoadConfig.h"

CString GetAppPathDir()
{
    TCHAR szModulePath[_MAX_PATH];
    GetModuleFileName( _hdllInstance, szModulePath, _MAX_PATH );

    TCHAR drive[_MAX_DRIVE];
    TCHAR dir[_MAX_DIR];
    _tsplitpath( szModulePath, drive, dir, NULL, NULL );

    TCHAR szPath[_MAX_PATH] = {0};
    _tmakepath( szPath, drive, dir, NULL, NULL );

    return CString( szPath );
}

CString BuildPath( const CString& dir, const CString& fileName )
{
    CString path;
    path.Format( _T( "%s%s" ), dir, fileName );
    return path;
}

void InitAllData( const CString& dirPath )
{
#if INCLUDE_TVNS_MODULE
	DataInitHelper::InitDataField( BuildPath( dirPath, _T( "石门揭煤-字段-图元属性.txt" ) ) );
	DataInitHelper::InitDataFieldWithMoreInfo( BuildPath( dirPath, _T( "瓦斯抽采-字段-图元属性.txt" ) ) );
	DataInitHelper::InitIntStrList( BuildPath( dirPath, _T( "瓦斯抽采-字符串-整数列表.txt" ) ) );
	DataInitHelper::InitDataFieldWithMoreInfo( BuildPath( dirPath, _T( "瓦斯抽采-字段-数据对象.txt" ) ) );
	DataInitHelper::InitSingleDataObject( BuildPath( dirPath, _T( "瓦斯抽采-全局信息对象列表.txt" ) ) );
#endif

#if INCLUDE_EVAL_MODULE
    DataInitHelper::InitDataFieldWithMoreInfo( BuildPath( dirPath, _T( "通风评价-字段-数据对象.txt" ) ) );
    DataInitHelper::InitStrList( BuildPath( dirPath, _T( "通风评价-字符串列表.txt" ) ) );
    DataInitHelper::InitIntStrList( BuildPath( dirPath, _T( "通风评价-字符串-整数列表.txt" ) ) );
    DataInitHelper::InitDataObjectList( BuildPath( dirPath, _T( "通风评价-字符串-数据对象列表.txt" ) ) );
    DataInitHelper::InitSingleDataObject( BuildPath( dirPath, _T( "通风评价-全局信息对象列表.txt" ) ) );
#endif

#if INCLUDE_VNC_MODULE
    DataInitHelper::InitDataFieldWithMoreInfo( BuildPath( dirPath, _T( "VNC-字段-通风图元.txt" ) ) );
#endif

#if INCLUDE_MVSS_MODULE
    // 添加属性数据字段
    DataInitHelper::InitDataFieldWithMoreInfo( BuildPath( dirPath, _T( "MVSS-字段-通风图元.txt" ) ) );
#endif

#if INCLUDE_GAS_MODULE
    // 添加属性数据字段
    DataInitHelper::InitDataFieldWithMoreInfo( BuildPath( dirPath, _T( "瓦斯地质图-字段-瓦斯图元.txt" ) ) );
#endif

#if INCLUDE_VNG_GEN_MODULE
    // 添加属性数据字段
    DataInitHelper::InitDataFieldWithMoreInfo( BuildPath( dirPath, _T( "通风网络图-字段-网络图元.txt" ) ) );
#endif

}