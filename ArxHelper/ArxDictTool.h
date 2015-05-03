#pragma once

#include "dlimexp.h"

// 无效的entry索引
#define INVALID_ENTRY 0

// CAD词典管理辅助工具类
// AcDbDictionay的key不是大小写敏感的
// 例如NAME, name, Name, naMe等都是一样的

// 一个key->XRecord(记录集)
// XRecord实质是一个resbuf list, 可以保存一串数据，
// 这里使用XRecord保存一些简单的文本串数据(可以是1个，也可以是多个)
class ARXHELPER_DLLIMPEXP ArxDictTool
{
public:
	static ArxDictTool* GetDictTool(const CString& dictName);

	bool findKey(const CString& key);
	void removeKey(const CString& key);
	void getAllKeys(AcStringArray& keys);

	int addEntry(const CString& key, const CString& entry);         // 添加entry,如果key不存在，则自动添加
	int removeEntry(const CString& key, const CString& entry);      // 删除entry
	int findEntry(const CString& key, const CString& entry);        // 查找entry的索引位置
	void getAllEntries(const CString& key, AcStringArray& entries); // 获取key包含的的所有entry
	int countEntries(const CString& key);                           // 统计key包含的所有entry个数

	// index必须要大于0
	bool modifyEntry(const CString& key, int index, const CString& newEntry); // 修改key中第index项的值
	bool getEntry(const CString& key, int index, CString& entry);             // 获取key中第index项的值

private:
	ArxDictTool(const CString& dictName);
	CString m_dictName;
};

// 一个key->AcDbObject
class ARXHELPER_DLLIMPEXP ArxDictTool2
{
public:
	static ArxDictTool2* GetDictTool(const CString& dictName);

	// 添加到词典中，同时自动关闭对象pObj->close();
	bool addEntry(const CString& key, AcDbObject* pObj);         // 添加
	bool removeEntry(const CString& key);                        // 删除
	bool findEntry(const CString& key, AcDbObjectId& objId);     // 查找
	void getAllKeys(AcStringArray& keys);
	void getAllEntries(AcDbObjectIdArray& objIds);               // 获取所有的对象

private:
	ArxDictTool2(const CString& dictName);
	CString m_dictName;
};

class ARXHELPER_DLLIMPEXP ArxDictHelper
{
public:
	static void GetAllKeys( const CString& dictName, AcStringArray& keys);
	static bool GetAllEntries(const CString& dictName, const CString& key, AcStringArray& entries);
	static bool AddEntry(const CString& dictName, const CString& key, const CString& entry);
	static void RemoveAllEntries(const CString& dictName, const CString& key);
	static bool RemoveEntry(const CString& dictName, const CString& key, const CString& entry);
	static void RemoveKey(const CString& dictName, const CString& key);
};