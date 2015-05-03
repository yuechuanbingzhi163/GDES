#pragma once

#include "dlimexp.h"

class GRAPH_EXPORT_API DFSHelper
{
public:
	static void FindGasTubePaths(const AcDbObjectId& sObjId, const AcDbObjectId& tObjId, 
		AcDbIntArray& pathNums, AcDbObjectIdArray& allPaths);
};