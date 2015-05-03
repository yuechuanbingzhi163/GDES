#pragma once

typedef AcArray<double> doubleArray;
typedef AcArray<doubleArray> ddoubleArray;
typedef AcArray<int> intArray;
typedef AcArray<AcDbObjectIdArray> libFanIdsArray;
typedef AcArray<CString> cstringArray;

enum chooseType
{
	POWER = 0,
	QUANTITY = 1
};
