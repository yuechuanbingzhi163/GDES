#include "StdAfx.h"
#include "ArxLoader.h"

ArxLoader::ArxLoader( const CString& arxFilePath, const CString& serviceName )
    : m_arxFilePath( arxFilePath ), m_serviceName( serviceName )
{

}

bool ArxLoader::loadLibrary()
{
    TCHAR szPath[_MAX_PATH] = {0}, szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFile[_MAX_FNAME], szExt[_MAX_EXT];
    _tsplitpath( ( LPCTSTR )m_arxFilePath, szDrive, szDir, szFile, szExt ); // 拆分路径

    // 判断扩展名是否为arx
    if( CString( szExt ) != _T( ".arx" ) ) return false;

    bool flag = false; // 默认为加载失败（服务已注册或者无法加载）
    AcRxObject* pSvc;
    if ( !( pSvc = acrxServiceDictionary->at ( ( LPCTSTR )m_serviceName ) ) ) // 服务尚未注册
    {
        flag = acrxDynamicLinker->loadModule ( ( LPCTSTR )m_arxFilePath, 0 );
    }
    return flag;
}

void ArxLoader::releaseLibrary()
{
    acrxDynamicLinker->unloadModule ( ( LPCTSTR )m_arxFilePath, 0 );
}

void* ArxLoader::getFunction( const CString& functionName )
{
    return acrxDynamicLinker->getSymbolAddress( ( LPCTSTR )m_serviceName, ( LPCTSTR )functionName );
}