#pragma once

// ARX模块加载器
class ArxLoader
{
public:
	ArxLoader(const CString& arxFilePath, const CString& serviceName);

	bool loadLibrary();                                    // 加载模块
	void releaseLibrary();                                 // 卸载模块
	void* getFunction(const CString& functionName);        // 查找ARX模块中的导出export函数地址

private:
	CString m_arxFilePath; // arx文件路径
	CString m_serviceName; // arx模块的注册服务名称
};