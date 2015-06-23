#include "StdAfx.h"
#include "Rcu.h"
#include "RcuDataLink.h"
#include "SwitchHelper.h"
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

void RcuHelper::GetRelatedCoalSurface(const AcDbObjectId& rock_gate, AcDbObjectId& coal_surf)
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

void RcuHelper::GetRelatedDrillSites(const AcDbObjectId& rock_gate, AcDbObjectIdArray& drill_sites)
{
	//查找所有的钻场图元
	AcDbObjectIdArray tags;
	ArxDataTool::GetEntsByType(_T("DrillSite"), tags, true);
	if(tags.isEmpty()) return;

	//查找钻场图元关联的宿主
	AcDbObjectIdArray hosts;
	DrawHelper::GetHosts(tags, hosts);
	if(hosts.length() != tags.length()) return;

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

void RcuHelper::GetRelatedClosePores(const AcDbObjectId& coal_surf, AcDbObjectIdArray& pores)
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

void RcuHelper::GetRelatedOpenPores(const AcDbObjectId& drill_site, AcDbObjectIdArray& pores)
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

bool RcuHelper::ReadRockGateData(const AcDbObjectId& rock_gate, RockGateLink& rg_link)
{
	if(rock_gate.isNull()) return false;

	rg_link.setDataSource(rock_gate);
	return rg_link.updateData(false);
}

bool RcuHelper::GetRockGateAndCoalSurfDatas(const AcDbObjectId& rock_gate, RockGateLink& rg_link, CoalSurfaceLink& cs_link)
{
	//提取石门关联的所有数据并填充到对话框中
	if(!RcuHelper::ReadRockGateData(rock_gate, rg_link)) return false;

	//查找石门关联的煤层
	AcDbObjectId coal_surf;
	RcuHelper::GetRelatedCoalSurface(rock_gate, coal_surf);
	if(coal_surf.isNull()) return false;

	//提取煤层的数据
	if(!RcuHelper::ReadCoalSurfaceData(coal_surf, cs_link))	return false;
	
	return true;
}

bool RcuHelper::GetDrillSiteData(const AcDbObjectId& drill_site, DrillSiteLink& ds_link)
{
	//提取钻场数据
	return RcuHelper::ReadDrillSiteData(drill_site, ds_link);
}

bool RcuHelper::ReadCoalSurfaceData(const AcDbObjectId& coal_surf, CoalSurfaceLink& cs_link)
{
	if(coal_surf.isNull()) return false;

	cs_link.setDataSource(coal_surf);
	return cs_link.updateData(false);
}

bool RcuHelper::ReadDrillSiteData(const AcDbObjectId& drill_site, DrillSiteLink& ds_link)
{
	if(drill_site.isNull()) return false;

	ds_link.setDataSource(drill_site);
	return ds_link.updateData(false);
}

bool RcuHelper::ReadPoreData(const AcDbObjectId& pore, PoreLink& po_link)
{
	if(pore.isNull()) return false;

	po_link.setDataSource(pore);
	po_link.updateData(false);
	return true;
}

void RcuHelper::VectorToAngle(const AcGeVector3d& v, double& ang1, double& ang2)
{
	//向量在xoy平面上的投影
	AcGeVector3d u = v.orthoProject(AcGeVector3d::kZAxis);
	acutPrintf(_T("\n投影值:x=%.3f, y=%.3f z=%.3f"), u.x, u.y, u.z);
	ang1 = u.angleTo(AcGeVector3d::kXAxis/*, AcGeVector3d::kZAxis*/);
	ang2 = u.angleTo(v);
}

void RcuHelper::VectorToAngleTest()
{
	AcGeVector3d v(1,1,1);
	double ang1, ang2;
	RcuHelper::VectorToAngle(v, ang1, ang2);
	acutPrintf(_T("\n向量:(%.3f, %.3f, %.3f)"), v.x, v.y, v.z);
	acutPrintf(_T("\n弧度-->方向角1:%f  仰角：%f"), ang1, ang2);
	acutPrintf(_T("\n角度-->方向角1:%f  仰角：%f"), ang1*57.295, ang2*57.295);

	AcGeVector3d v1(1,0,0);
	RcuHelper::VectorToAngle(v1, ang1, ang2);
	acutPrintf(_T("\n向量:(%.3f, %.3f, %.3f)"), v1.x, v1.y, v1.z);
	acutPrintf(_T("\n弧度-->方向角1:%f  仰角：%f"), ang1, ang2);
	acutPrintf(_T("\n角度-->方向角1:%f  仰角：%f"), ang1*57.295, ang2*57.295);

	AcGeVector3d v2(-1,-1,1);
	RcuHelper::VectorToAngle(v2, ang1, ang2);
	acutPrintf(_T("\n向量:(%.3f, %.3f, %.3f)"), v2.x, v2.y, v2.z);
	acutPrintf(_T("\n弧度-->方向角1:%f  仰角：%f"), ang1, ang2);
	acutPrintf(_T("\n角度-->方向角1:%f  仰角：%f"), ang1*57.295, ang2*57.295);
}

bool RcuHelper::CaculCoalSurfParam(const RockGateLink& rg_link,CoalSurfaceLink& cs_link)
{
	AcGePoint3d orign;
	ArxUtilHelper::StringToPoint3d(rg_link.m_pt,orign);
	Rcu rcu;
	rcu.setOrigin(orign);
	rcu.setRockGateParams(rg_link.m_dist,rg_link.m_top,rg_link.m_bottom,rg_link.m_left,rg_link.m_right);
	rcu.setTunnelParams(rg_link.m_height,rg_link.m_width,rg_link.m_width);
	//第二个参数是石门轴线与煤层走向的夹角
	//但是界面上暂时没有，所以考虑90度的
	double angle = DegToRad(cs_link.m_angle);
	rcu.setCoalParams(angle,PI*0.5,cs_link.m_thick);
	double w,h;
	rcu.drillExtent(w,h);
	cs_link.m_width = w;
	cs_link.m_height = h;
	acutPrintf(_T("\n计算时->宽度:%.4lf\t高度:%.4lf\n"),cs_link.m_width,cs_link.m_height);
	return true;
}

bool RcuHelper::GetRockGateInsertPt( const AcDbObjectId& rock_gate, AcGePoint3d& insertPt )
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return false;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, rock_gate, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	RockGate* pRG = RockGate::cast( pObj );
	if(pRG == 0)
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	insertPt = pRG->getInsertPt();

	actrTransactionManager->endTransaction();
	return true;
}

bool RcuHelper::CaculDrillSitePt(const DrillSiteLink& ds_link,const RockGateLink& rg_link, const AcGePoint3d& rgInsertPt, AcGePoint3d& insertPt, AcGePoint3d& linkPt)
{
	//左帮
	if(0 == ds_link.m_leftOrRight)
	{
		insertPt.x = rgInsertPt.x - rg_link.m_width / 2;
		linkPt.x = rgInsertPt.x - rg_link.m_width / 2 - 2 * ds_link.m_dist;
	}

	//右帮
	else if(1 == ds_link.m_leftOrRight)
	{
		insertPt.x = rgInsertPt.x + rg_link.m_width / 2;
		linkPt.x = rgInsertPt.x + rg_link.m_width / 2 + 2 * ds_link.m_dist;
	}

	else return false;

	insertPt.y = rgInsertPt.y - ds_link.m_dist;
	insertPt.z = rgInsertPt.z;
	linkPt.y = rgInsertPt.y - ds_link.m_dist;
	linkPt.z = rgInsertPt.z;
	return true;
}

bool RcuHelper::SetDrillSitePt(const AcDbObjectId& drill_site, const AcGePoint3d& insertPt, const AcGePoint3d& linkPt)
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return false;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, drill_site, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	DrillSite* pDS = DrillSite::cast( pObj );
	if(pDS == 0)
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	pDS->setInsertPt(insertPt);
	pDS->setLinkPt(linkPt);

	actrTransactionManager->endTransaction();
	return true;
}

bool RcuHelper::CreateRockGate(const AcGePoint3d& pt, RockGateLink& rg_link, CoalSurfaceLink& cs_link)
{
	DocumentLockSwitch lock_switch;

	//新建石门并设置插入点坐标
	RockGate* pRG = new RockGate();
	pRG->setInsertPt(pt);

	CoalSurface* pCS = new CoalSurface();
	pCS->setInsertPt(AcGePoint3d(pt.x, pt.y+500, pt.z));

	//添加石门到cad图形数据库
	if(!ArxUtilHelper::PostToModelSpace(pRG))
	{
		delete pRG; pRG = 0;
		return  false;
	}
	
	//计算煤层参数
	RcuHelper::CaculCoalSurfParam(rg_link,cs_link);
	//acutPrintf(_T("\n计算之后->宽度:%.4lf\t高度:%.4lf\n"),cs_link.m_width,cs_link.m_height);

	//煤层关联到石门
	pCS->setRelatedGE(pRG->objectId());
	//添加煤层到cad图形数据库
	if(!ArxUtilHelper::PostToModelSpace(pCS))
	{
		delete pCS; pCS = 0;
		//煤层提交失败,则删除已添加的石门
		ArxEntityHelper::EraseObject(pRG->objectId(), true);
		return false;
	}

	//通过DataLink修改石门和煤层的数据
	rg_link.setDataSource(pRG->objectId());
	rg_link.updateData(true);
	cs_link.setDataSource(pCS->objectId());
	cs_link.updateData(true);

	return true;
}

bool RcuHelper::CreateDrillSite(const AcDbObjectId& rock_gate, DrillSiteLink& ds_link)
{
	RockGateLink rg_link;
	if(!RcuHelper::ReadRockGateData(rock_gate, rg_link))
	{
		AfxMessageBox(_T("提取石门数据发生错误!"));
		return false;
	}

	//计算钻场插入坐标和连接坐标
	AcGePoint3d insertPt,linkPt,rgInsertPt;
	RcuHelper::GetRockGateInsertPt(rock_gate,rgInsertPt);
	if(!RcuHelper::CaculDrillSitePt(ds_link,rg_link,rgInsertPt,insertPt,linkPt)) 
	{
		AfxMessageBox(_T("钻场位置数据错误!"));
		return false;
	}

	//新建钻场并设置插入点坐标
	DrillSite* pDS = new DrillSite();
	pDS->setInsertPt(insertPt);
	pDS->setLinkPt(linkPt);
	pDS->setRelatedGE(rock_gate);

	//添加钻场到cad图形数据库
	if(!ArxUtilHelper::PostToModelSpace(pDS))
	{
		delete pDS; pDS = 0;
		return false;
	}

	//关联图元并更新
	ds_link.setDataSource(pDS->objectId());
	ds_link.updateData(true);

	return true;
}

bool RcuHelper::ModifyDrillSiteParam(const AcDbObjectId& drill_site, DrillSiteLink& ds_link)
{
	if(drill_site.isNull()) return false;

	AcDbObjectId rock_gate;
	if(!DrawHelper::GetHostGE(drill_site, rock_gate)) return false;

	//读取石门的数据
	RockGateLink rg_link;
	if(!RcuHelper::ReadRockGateData(rock_gate, rg_link)) return false;

	//获取石门的插入点坐标
	AcGePoint3d pt;
	if(!RcuHelper::GetRockGateInsertPt(rock_gate, pt)) return false;

	AcGePoint3d insertPt, linkPt;
	if(!RcuHelper::CaculDrillSitePt(ds_link, rg_link, pt, insertPt, linkPt)) return false;

	//修改钻场图元的几何点坐标
	return RcuHelper::SetDrillSitePt(drill_site, insertPt, linkPt);
}
