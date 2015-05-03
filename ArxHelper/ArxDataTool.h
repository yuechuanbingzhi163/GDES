#pragma once

#include "dlimexp.h"

// 数据辅助工具类
class ARXHELPER_DLLIMPEXP ArxDataTool
{
public:
	/* 扩展数据操作 */
	static void RegAppName(AcDbDatabase* db, const CString& appName);

	static bool GetDataFromXData(AcDbObject* pObj, const CString& appName, int index, CString& value);

	static bool SetDataToXData(AcDbObject* pObj, const CString& appName, int index, const CString& value);

	static void AddDataToXData(AcDbObject* pObj, const CString& appName);

	static void RemoveDataFromXData(AcDbObject* pObj, const CString& appName, int index);

	static void AddDataToObjects(const AcDbObjectIdArray& objIds, const CString& appName);

	static void RemoveDataFromObjects(const AcDbObjectIdArray& objIds, const CString& appName, int index);

	/* 判断词典是否存在 */
	static bool IsDictExist(const CString& dictName);
	
	/* 注册添加词典 */
	static void RegDict(const CString& dictName);

	/* 获取类型名称 */
	//static CString GetTypeName(const AcDbObjectId& objId);

	/* 判断图元是否与类型type匹配 */
	static bool IsEqualType(const CString& type, const AcDbObjectId& objId, bool isDerivedFromParent=true);

	/* 在模型空间的块表记录中查找所有指定类型的图元(屏幕上可见的) */
	static void GetEntsByType(const CString& type, AcDbObjectIdArray& objIds, bool isDerivedFromParent=true);

	/* 在模型空间的块表记录中查找所有指定类型的图元(屏幕上不可见的，已被删除的) */
	static void GetErasedEntsByType(const CString& type, AcDbObjectIdArray& objIds, bool isDerivedFromParent=true);

	/* 过滤图元 */
	static void FilterEntsByType(const CString& type, const AcDbObjectIdArray& allObjIds, AcDbObjectIdArray& objIds);
};
