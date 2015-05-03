#pragma once

#include "dlimexp.h"

class ARXHELPER_DLLIMPEXP ArxClassHelper
{
public:
	static bool IsValidClass(const CString& type);
	static bool IsAbstractClass(const CString& type);
	static bool IsDerivedFrom(const CString& type, const CString& parentType);
	static bool GetDxfName(const CString& type, CString& dxfName);
	static void GetClassHierarchy(const CString& type, AcStringArray& types, bool findAbstractType);
	static bool GetChildClass(const CString& type, AcStringArray& types);
	static bool GetTopParentClass(const CString& type, CString& parentType);
	static bool GetAllTopParentClass(const CString& root, AcStringArray& types);
	static void GetArxClassTypes(const CString& root, AcStringArray& types, bool findAbstractType);
	
	static AcDbObject* CreateObjectByType(const CString& type);
};
