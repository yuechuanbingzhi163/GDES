#include "StdAfx.h"
#include "RcuHelper.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

#include "../DefGE/RcuGE.h"
#include "../DefGE/Pore.h"
#include "../DefGE/DrillSite.h"
#include "../DefGE/RockGate.h"
#include "../DefGE/CoalSurface.h"

void RcuHelper::FindAllRockGates(AcDbObjectIdArray& rock_gates)
{
	ArxDataTool::GetEntsByType(_T("RockGate"), rock_gates, true);
}

void RcuHelper::GetCoalSurface(const AcDbObjectId& rock_gate, AcDbObjectId& coal_surf)
{
	//查找所有的煤层图元
	AcDbObjectIdArray tags;
	ArxDataTool::GetEntsByType(_T("CoalSurface"), tags, true);

	//查找煤层图元关联的宿主
	AcDbObjectIdArray hosts;
	DrawHelper::GetHosts(tags, hosts);

	//匹配
	int n = hosts.length();
	for(int i=0;i<n;i++)
	{
		if(hosts[i] == rock_gate)
		{
			coal_surf = tags[i];
			break;
		}
	}
}

void RcuHelper::GetDrillSites(const AcDbObjectId& rock_gate, AcDbObjectIdArray& drill_sites)
{
	//查找所有的钻场图元
	AcDbObjectIdArray tags;
	ArxDataTool::GetEntsByType(_T("DrillSite"), tags, true);

	//查找钻场图元关联的宿主
	AcDbObjectIdArray hosts;
	DrawHelper::GetHosts(tags, hosts);

	//匹配
	int n = hosts.length();
	for(int i=0;i<n;i++)
	{
		if(hosts[i] == rock_gate)
		{
			drill_sites.append(tags[i]);
		}
	}
}

void RcuHelper::GetAllClosePores(const AcDbObjectId& coal_surf, AcDbObjectIdArray& pores)
{
	//查找所有的钻孔图元
	AcDbObjectIdArray tags;
	ArxDataTool::GetEntsByType(_T("Pore"), tags, true);

	//查找钻孔图元关联的宿主
	AcDbObjectIdArray hosts;
	DrawHelper::GetHosts(tags, hosts);

	//匹配
	int n = hosts.length();
	for(int i=0;i<n;i++)
	{
		if(hosts[i] == coal_surf)
		{
			pores.append(tags[i]);
		}
	}
}

void RcuHelper::GetAllOpenPores(const AcDbObjectId& drill_site, AcDbObjectIdArray& pores)
{
	//查找所有的钻孔图元
	AcDbObjectIdArray tags;
	ArxDataTool::GetEntsByType(_T("Pore"), tags, true);

	//查找钻孔图元关联的宿主
	AcDbObjectIdArray hosts;
	DrawHelper::GetHosts(tags, hosts);

	//匹配
	int n = hosts.length();
	for(int i=0;i<n;i++)
	{
		if(hosts[i] == drill_site)
		{
			pores.append(tags[i]);
		}
	}
}

bool RcuHelper::GetRockGateData(const AcDbObjectId& rock_gate, RockGateLink& rg_link)
{
	if(rock_gate.isNull()) return false;

	rg_link.setDataSource(rock_gate);
	rg_link.updateData(false);
	return true;
}

bool RcuHelper::GetCoalSurfaceData(const AcDbObjectId& coal_surf, CoalSurfaceLink& cs_link)
{
	if(coal_surf.isNull()) return false;

	cs_link.setDataSource(coal_surf);
	cs_link.updateData(false);
	return true;
}

bool RcuHelper::GetDrillSiteData(const AcDbObjectId& drill_site, DrillSiteLink& ds_link)
{
	if(drill_site.isNull()) return false;

	ds_link.setDataSource(drill_site);
	ds_link.updateData(false);
	return true;
}

bool RcuHelper::GetPoreData(const AcDbObjectId& pore, PoreLink& po_link)
{
	if(pore.isNull()) return false;

	po_link.setDataSource(pore);
	po_link.updateData(false);
	return true;
}
