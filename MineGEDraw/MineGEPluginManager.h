#pragma once

#include <deque>

class MineGEPlugin;
class MineGEDrawManager;
class ArxLoader;

class MineGEPluginManager
{
public:
	MineGEPluginManager(MineGEDrawManager* pMineGEDrawManager);
	~MineGEPluginManager(void);
	
	void load(const CString &arxFilePath, const CString& serviceName);
	void unload(const CString &arxFilePath);

private:
	struct PluginInfo
	{
		CString pluginFilePath; // 插件所在路径
		ArxLoader* pARXLoader;  // arx模块加载器
		MineGEPlugin* pMineGEPlugin; // 插件对象
	};

	void unload( PluginInfo &plugIn );

	void unloadAllPlugins(); // 卸载所有插件

	// 加载arx模块，并返回一个ARXLoader对象
	ArxLoader* loadARXModule(const CString &arxFilePath, const CString& serviceName);

	// 创建图元插件
	MineGEPlugin* createMineGEPlugin(ArxLoader* pARXLoader);

private:
	MineGEDrawManager* m_pMineGEDrawManager;
	typedef std::deque<PluginInfo> Plugins;
	Plugins m_plugins;
};
