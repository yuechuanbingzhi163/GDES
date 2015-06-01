#pragma once

#include "dlimexp.h"

class MINEGE_DLLIMPEXP StringListHelper
{
public:
	static void GetAllNames(AcStringArray& names);
	static bool AddString(const CString& name, const CString& strValue);	// 实现效率较低
	static bool AddStringList(const CString& name, const AcStringArray& strList);
	static bool GetStringList(const CString& name, AcStringArray& strList);
	static void RemoveStringList(const CString& name);
};

class MINEGE_DLLIMPEXP IntStrListHelper
{
public:
	static void GetAllNames(AcStringArray& names);
	static bool AddIntStrPair(const CString& name, int intValue, const CString& strValue);	// 实现效率较低
	static bool AddIntStrList(const CString& name, const AcDbIntArray& intList, const AcStringArray& strList);
	static bool GetIntStrList(const CString& name, AcDbIntArray& intList, AcStringArray& strList);
	static void RemoveIntStrList(const CString& name);
};

// 数据对象列表辅助类
// 像"生产水平"、"煤层"之类的信息，每一类信息都包含多个子数据
class MINEGE_DLLIMPEXP DataObjectListHelper
{
public:
	static void GetAllNames(AcStringArray& names);
	static bool AddObject(const CString& name, const CString& value);
	static bool RemoveObject(const CString& name, const CString& value);
	static bool RemoveAllObject(const CString& name);
	static bool GetObjectNameList(const CString& name, AcStringArray& values);
	static bool IsObjectListType(const CString& name);
	static bool GetObjectId(const CString& name, const CString& value, AcDbObjectId& objId);
};

// 与DataObjectListHelper的相同，每个name中只包含1个数据对象
// 默认的value <==> SINGLE_DATA_OBJECT_DEF_NAME，参见config.h
// 主要用于描述全局唯一的信息，例如"矿井信息", "评价项目信息"等
// 这些信息对于当前的图形来说是唯一的，不需要有多个
class MINEGE_DLLIMPEXP SingleDataObjectHelper
{
public:
	static void GetAllNames(AcStringArray& names);
	static bool AddObject(const CString& name);
	static bool RemoveObject(const CString& name);
	static bool GetObjectId(const CString& name, AcDbObjectId& objId);
};

class MINEGE_DLLIMPEXP GDESDataObjectHelper
{
public:
	static void GetAllNames(AcStringArray& names);
	static bool AddObject(const CString& name);
	static bool RemoveObject(const CString& name);
	static bool GetObjectId(const CString& name, AcDbObjectId& objId);
};