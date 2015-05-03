#ifndef FUNC_FIELD_HELPER_H
#define FUNC_FIELD_HELPER_H

#include "dlimexp.h"

class MINEGE_DLLIMPEXP FuncFieldHelper
{
public:
	static bool GetFunctions(AcStringArray& funcs);                                 // 获取所有功能
	static bool GetTypes(const CString& f, AcStringArray& types);                   // 获取功能下的所有类型
	static bool GetFields(const CString& f, const CString& type, AcStringArray& fields);      // 获取字段
	static bool AddField(const CString& f, const CString& type, const CString& field);        // 添加字段
	static bool RemoveFunction(const CString& f);                                             // 删除功能
	static bool RemoveType(const CString& f, const CString& type);                            // 删除类型
	static bool RemoveField(const CString& f, const CString& type, const CString& field);     // 删除字段
};

#endif // FUNC_FIELD_HELPER_H
