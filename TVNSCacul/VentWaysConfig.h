#pragma once

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/ArxUtilHelper.h"


#define SINGLE_FAN_CHIM		_T("单机单风筒")

#define AXILIARY_RAM		_T("辅助风筒")

#define DISCONTI_RAM		_T("间断风筒")

#define SINGLE_DOUBLE_ROW	_T("单机双列")

#define FAN_IN_SERRIER		_T("串联局扇")

#define HYBIRD_VENT			_T("混合通风")

#define CABINET_VENT		_T("风柜通风")

#define LIBRARY_VENT		_T("风库通风")

#define DRILLING_VENT		_T("钻孔通风")

static AcDbObjectId GetLinkedTTunelId(AcDbObjectId fanObjId)
{
	AcDbObjectId TTunelId;
	//获取风机所在的巷道
	AcDbObjectId tunelId;
	DrawHelper::GetHostGE(fanObjId,tunelId);

	if (ArxUtilHelper::IsEqualType(_T("TTunnel"),tunelId))
	{
		TTunelId = tunelId;
	}

	else
	{
		//获取风机所在巷道相连接的工作面
		TTunelId = DrawHelper::GetRelatedTW(tunelId);
	}

return TTunelId;
}

static AcDbObjectIdArray GetLinkedFans(AcDbObjectId ttunnelId)
{
	AcDbObjectIdArray tunnelIds = DrawHelper::GetRelatedTunnel(ttunnelId);
	
	//tunnelIds.append(ttunnelId);
	//acutPrintf(_T("\n连接的巷道总数:%d"),tunnelIds.length());

	AcDbObjectIdArray fanIds;
	int len = tunnelIds.length();
	for (int i = 0; i < len; i++)
	{
		AcDbObjectIdArray releatFanIds;
		DrawHelper::GetTagGEById2(tunnelIds[i],_T("LocalFan"),releatFanIds);
		int fanNum = releatFanIds.length();
		if(0 == fanNum) continue;
		for(int j = 0; j < fanNum; j++)
		{
			fanIds.append(releatFanIds[j]);
		}
	}

	//acutPrintf(_T("\n找到相关联的风机总数是:%d"),fanIds.length());
	return fanIds;
}