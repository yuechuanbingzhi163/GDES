#pragma once

#include "dlimexp.h"

//图算法帮助类
class GRAPH_EXPORT_API GraphAlgo
{
public:
	//查找两条管路之间的所有瓦斯管路路径
	static void FindGasTubePaths(const AcDbObjectId& sObjId, const AcDbObjectId& tObjId, AcDbIntArray& pathNums, AcDbObjectIdArray& allPaths);
};