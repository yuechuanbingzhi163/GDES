#pragma once

#include "dlimexp.h"

// 属性字段管理辅助类
class MINEGE_DLLIMPEXP FieldHelper
{
public:
	static bool AddField(const CString& type, const CString& field);        // 添加字段
	static bool RemoveField(const CString& type, const CString& field);     // 删除字段
	static void RemoveAllFields(const CString& type);                       // 删除所有字段
	static int FindField(const CString& type, const CString& field);        // 查找字段的索引位置
	static void GetAllFields(const CString& type, AcStringArray& fields);   // 获取图元的所有字段
	static int CountFields(const CString& type);                            // 统计图元的字段个数
	static void GetAllRegTypes(AcStringArray& types);		                // 获取当前已注册的图元类型
};
