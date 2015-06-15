#pragma once

#include "RcuDataLink.h"

class RcuHelper
{
public:
	//查找所有的石门
	static void FindAllRockGates(AcDbObjectIdArray& rock_gates);
	//查找与石门关联的煤层
	static void GetCoalSurface(const AcDbObjectId& rock_gate, AcDbObjectId& coal_surf);
	//查找与石门关联的钻场
	static void GetDrillSites(const AcDbObjectId& rock_gate, AcDbObjectIdArray& drill_sites);
	//查找与煤层关联的终孔
	static void GetAllClosePores(const AcDbObjectId& coal_surf, AcDbObjectIdArray& pores);
	//查找与钻场关联的开孔
	static void GetAllOpenPores(const AcDbObjectId& drill_site, AcDbObjectIdArray& pores);

	//从石门中提取数据
	static bool GetRockGateData(const AcDbObjectId& rock_gate, RockGateLink& rg_link);
	//从煤层中提取数据
	static bool GetCoalSurfaceData(const AcDbObjectId& coal_surf, CoalSurfaceLink& cs_link);
	//从钻场提取数据
	static bool GetDrillSiteData(const AcDbObjectId& drill_site, DrillSiteLink& ds_link);
	//从钻孔提取数据
	static bool GetPoreData(const AcDbObjectId& pore, PoreLink& po_link);
};
