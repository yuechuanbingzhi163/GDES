#include "StdAfx.h"
#include "MineGEPluginManager.h"
#include "MineGEPlugin.h"
#include "MineGEDrawManager.h"
#include "ARXLoader.h"
#include "MineGEPlugin.h"

MineGEPluginManager::MineGEPluginManager( MineGEDrawManager* pMineGEDrawManager ) : m_pMineGEDrawManager( pMineGEDrawManager )
{
}

MineGEPluginManager::~MineGEPluginManager( void )
{
    unloadAllPlugins();
}

ArxLoader* MineGEPluginManager::loadARXModule( const CString& arxFilePath, const CString& serviceName )
{
    ArxLoader* pARXLoader = new ArxLoader( arxFilePath, serviceName );

    if( !pARXLoader->loadLibrary() ) 	          // 加载插件失败
    {
        delete pARXLoader;
        return 0;
    }

    return pARXLoader;
}

MineGEPlugin* MineGEPluginManager::createMineGEPlugin( ArxLoader* pARXLoader )
{
    MineGEPluginCreateFunc mgdpc = 		        // 查找创建函数
        ( MineGEPluginCreateFunc )pARXLoader->getFunction( SYMBOL_STRINGIZE( MINEGE_PLUGIN_CREATE_FUNCTION_NAME ) );

    if( mgdpc == 0 ) return 0;

    // 创建插件对象
    return ( *mgdpc )();
}

void MineGEPluginManager::load( const CString& arxFilePath, const CString& serviceName )
{
    ArxLoader* pARXLoader = loadARXModule( arxFilePath, serviceName );
    if( pARXLoader == 0 )
    {
        acutPrintf( _T( "\n加载插件:\n\t%s失败\n\t注册服务名称:%s" ), arxFilePath, serviceName );
        return;
    }

    MineGEPlugin* pMineGEPlugin = createMineGEPlugin( pARXLoader );
    if( pMineGEPlugin == 0 )
    {
        acutPrintf( _T( "\n初始化插件:\n\t%s失败\n\t注册服务名称:%s" ), arxFilePath, serviceName );
        pARXLoader->releaseLibrary();
        delete pARXLoader;
        return;
    }

    acutPrintf( _T( "\n成功加载插件\n\t%s\n\t注册服务名称:%s" ), arxFilePath, serviceName );

    // 插件初始化
    pMineGEPlugin->initialize( m_pMineGEDrawManager );

    // 创建插件信息，并添加到插件集合
    PluginInfo info;
    info.pluginFilePath = arxFilePath;
    info.pARXLoader = pARXLoader;
    info.pMineGEPlugin = pMineGEPlugin;

    m_plugins.push_back( info );
}

void MineGEPluginManager::unload( const CString& arxFilePath )
{
    for ( Plugins::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it )
    {
        if ( arxFilePath.CompareNoCase( ( *it ).pluginFilePath ) == 0 )
        {
            unload( *it );
            m_plugins.erase( it );
            break;
        }
    }
}

void MineGEPluginManager::unload( PluginInfo& info )
{
    info.pMineGEPlugin->unInitialize( m_pMineGEDrawManager ); // write

    info.pARXLoader->releaseLibrary();
    delete info.pARXLoader;
    // 不可以delete pMineGEPlugin，因为它的内存是在dll通过静态变量的方式分配的
    //delete info.pMineGEPlugin;
}

void MineGEPluginManager::unloadAllPlugins()
{
    for ( Plugins::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it )
    {
        unload( *it );
    }
}