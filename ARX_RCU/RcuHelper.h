#pragma once

#include "RcuDataLink.h"

class RcuHelper
{
public:
	//查找所有的石门
	static void FindAllRockGates(AcDbObjectIdArray& rock_gates);
	//查找与石门关联的煤层
	static void GetRelatedCoalSurface(const AcDbObjectId& rock_gate, AcDbObjectId& coal_surf);
	//查找与石门关联的钻场
	static void GetRelatedDrillSites(const AcDbObjectId& rock_gate, AcDbObjectIdArray& drill_sites);
	//查找与煤层关联的终孔
	static void GetRelatedClosePores(const AcDbObjectId& coal_surf, AcDbObjectIdArray& pores);
	//查找与钻场关联的开孔
	static void GetRelatedOpenPores(const AcDbObjectId& drill_site, AcDbObjectIdArray& pores);

	//从石门中提取数据
	static bool ReadRockGateData(const AcDbObjectId& rock_gate, RockGateLink& rg_link);
	//从煤层中提取数据
	static bool ReadCoalSurfaceData(const AcDbObjectId& coal_surf, CoalSurfaceLink& cs_link);
	//从钻场提取数据
	static bool ReadDrillSiteData(const AcDbObjectId& drill_site, DrillSiteLink& ds_link);
	//从钻孔提取数据
	static bool ReadPoreData(const AcDbObjectId& pore, PoreLink& po_link);

	//得到石门以及关联煤层的数据
	static bool GetRockGateAndCoalSurfDatas(const AcDbObjectId& rock_gate, RockGateLink& rg_link, CoalSurfaceLink& cs_link);
	//得到钻场的数据
	static bool GetDrillSiteData(const AcDbObjectId& drill_site, DrillSiteLink& ds_link);

	//向量在水平面的方向角和仰角
	static void VectorToAngle(const AcGeVector3d& v, double& ang1, double& ang2);
	//测试VectorToAngle函数
	static void VectorToAngleTest();

	//计算煤层面参数
	static bool CaculCoalSurfParam(const RockGateLink& rg_link,CoalSurfaceLink& cs_link);
	//获取石门的几何插入点坐标
	static bool GetRockGateInsertPt(const AcDbObjectId& rock_gate, AcGePoint3d& insertPt);
	//计算钻场的2点坐标
	static bool CaculDrillSitePt(const DrillSiteLink& ds_link,const RockGateLink& rg_link,const AcGePoint3d& rgInsertPt,AcGePoint3d& insertPt,AcGePoint3d& linkPt);
	//设置钻场图元的2点坐标
	static bool SetDrillSitePt(const AcDbObjectId& drill_site, const AcGePoint3d& insertPt, const AcGePoint3d& linkPt);

	//创建石门和煤层图元(rg_link和cs_link的数据会与新增的图元关联，并被更新到图元)
	static bool CreateRockGate(const AcGePoint3d& pt, RockGateLink& rg_link, CoalSurfaceLink& cs_link);
	//创建钻场图元(与石门关联)
	static bool CreateDrillSite(const AcDbObjectId& rock_gate, DrillSiteLink& ds_link);
	//修改钻场参数
	static bool ModifyDrillSiteParam(const AcDbObjectId& drill_site, DrillSiteLink& ds_link);
};