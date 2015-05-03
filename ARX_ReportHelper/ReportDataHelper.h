#pragma once
#include "../MineGE/DataObject.h"
#include "../MineGE/DataListHelper.h"
#include "config.h"

#define ISNULL _T("**")

class ReportDataHelper
{
public:
	static void	ReadDatas(const CString& name, ArrayVector& datasVector,int unitCount);
	static void WriteDatas(const CString& name,const ArrayVector& datas);

private:
	static bool SetObjectDatas(const AcDbObjectId& objId,const AcStringArray& values);
	static bool GetObjectDatas(const AcDbObjectId& objId,AcStringArray& values);
};
