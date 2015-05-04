#pragma once

#include <SqliteHelper/SqliteHelper.h>

class UIHelper
{
public:
	static void ShowDataListDockBar(const AcDbObjectId& sObjId, const AcDbObjectId& tObjId);
	static void DestroyDataListDockBar();
	static void CreatReport();
	static void SetGESize();
	static void GetPumpDB();
};

