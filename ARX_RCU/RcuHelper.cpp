#include "StdAfx.h"
#include "RcuHelper.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

void RcuHelper::FindAllRockGates(AcDbObjectIdArray& objIds)
{
	ArxDataTool::GetEntsByType(_T("RockGate"), objIds, true);
}
