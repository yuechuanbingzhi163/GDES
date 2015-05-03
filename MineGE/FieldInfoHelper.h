#pragma once

/* FieldInfo已经导出(dllexport) */
#include "FieldInfo.h"

class MINEGE_DLLIMPEXP FieldInfoHelper
{
public:
	static void AddFieldInfo(const CString& type, const CString& field, const FieldInfo& info);
	static void RemoveFieldInfo(const CString& type, const CString& field);
	static bool FindFieldInfo(const CString& type, const CString& field);

	static void ReadFieldInfo(const CString& type, const CString& field, FieldInfo& info);
	static void WriteFieldInfo(const CString& type, const CString& field, const FieldInfo& info);
};