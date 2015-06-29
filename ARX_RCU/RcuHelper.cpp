#include "StdAfx.h"
#include "Rcu2.h"
#include "RcuDataLink.h"
#include "SwitchHelper.h"
#include "RcuHelper.h"
#include <algorithm>

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

#include "../DefGE/RcuGE.h"
#include "../DefGE/Pore.h"
#include "../DefGE/DrillSite.h"
#include "../DefGE/RockGate.h"
#include "../DefGE/CoalSurface.h"

void RcuHelper::FindAllDrillSite(AcDbObjectIdArray& rock_gates)
{
	ArxDataTool::GetEntsByType(_T("DrillSite"), rock_gates, true);
}

void RcuHelper::GetRelatedCoalSurface(const AcDbObjectId& drill_site, AcDbObjectId& coal_surf)
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
		if(hosts[i] == drill_site)
		{
			coal_surf = tags[i];
			break;
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

void RcuHelper::ClearRelatedOpenPores(const AcDbObjectId& drill_site)
{
	//查找所有与钻场关联的钻孔
	AcDbObjectIdArray pores;
	RcuHelper::GetRelatedOpenPores(drill_site, pores);
	if(pores.isEmpty()) return;

	//删除钻孔
	ArxEntityHelper::EraseObjects(pores, Adesk::kTrue);
}

void RcuHelper::ClearRelatedClosePores(const AcDbObjectId& coal_surf)
{
	//查找所有与钻场关联的钻孔
	AcDbObjectIdArray pores;
	RcuHelper::GetRelatedOpenPores(coal_surf, pores);
	if(pores.isEmpty()) return;

	//删除钻孔
	ArxEntityHelper::EraseObjects(pores, Adesk::kTrue);
}

bool RcuHelper::GetDrillSiteAndCoalSurfDatas(const AcDbObjectId& drill_site, DrillSiteLink& ds_link, CoalSurfaceLink& cs_link)
{
	//提取钻场关联的所有数据并填充到对话框中
	if(!RcuHelper::ReadDrillSiteData(drill_site, ds_link)) return false;

	//查找钻场关联的煤层
	AcDbObjectId coal_surf;
	RcuHelper::GetRelatedCoalSurface(drill_site, coal_surf);
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

bool RcuHelper::CaculCoalSurfParam( DrillSiteLink& ds_link, CoalSurfaceLink& cs_link)
{
	AcGePoint3d orign = ds_link.m_pt;

	//调用rcu类进行计算
	Rcu2 rcu;
	//设置煤层参数(倾角和厚度)
	rcu.setCoalSurf(DegToRad(cs_link.m_angle), cs_link.m_thick);
	//设置钻场巷道断面参数
	rcu.setDrillSite1(ds_link.m_height, ds_link.m_width);
	//设置钻场的上下左右保护帮距
	rcu.setDrillSite2(ds_link.m_top, ds_link.m_bottom, ds_link.m_left, ds_link.m_right);
	//设置钻场距离煤层的最小法距
	rcu.setDrillSite3(ds_link.m_dist);

	//计算煤层面的抽采范围宽度和高度
	rcu.drillExtent(cs_link.m_width, cs_link.m_height);
	//计算煤层面(中心点坐标、法向量、走向向量、倾向向量)
	AcGePoint3d cnt;
	AcGeVector3d normV, headV, dipV;
	if(!rcu.drillSurface(cnt, normV, headV, dipV)) return false;
	cs_link.m_pt = cnt + orign.asVector();
	cs_link.m_normV = normV;
	cs_link.m_headV = headV;
	cs_link.m_dipV = dipV;

	return true;
}

bool RcuHelper::CaculRelativeOpenPorePts(DrillSiteLink& ds_link, AcGePoint3dArray& pts)
{
	//计算钻孔的坐标
	//钻场的宽度上布置多少个钻孔
	int n1 = int(ds_link.m_width/ds_link.m_pore_gap);
	//钻场的高度上布置多少个钻孔
	int n2 = int(ds_link.m_height/ds_link.m_pore_gap);

	AcGePoint3d pt = AcGePoint3d::kOrigin;
	//迎头,将基点处理到左下角
	pt += AcGeVector3d(-0.5*ds_link.m_width, 0, 0);
	for(int i=0;i<n1;i++)
	{
		pt.x += ds_link.m_pore_gap;
		pt.y = 0;
		for(int j=0;j<n2;j++)
		{
			pt.y += ds_link.m_pore_gap;
			pts.append(pt);
		}
	}
	return true;
}

bool RcuHelper::CaculRelativeClosePorePts(CoalSurfaceLink& cs_link, AcGePoint3dArray& pts)
{
	//抽采范围的宽度上布置钻孔个数
	int n1 = int(cs_link.m_width/cs_link.m_gas_radius);
	//抽采范围的高度上布置钻孔个数
	int n2 = int(cs_link.m_height/cs_link.m_gas_radius);

	//矩形左下角的点坐标,从这个点开始计算
	AcGePoint3d origin = AcGePoint3d::kOrigin + cs_link.m_headV*cs_link.m_width*-0.5 + cs_link.m_dipV*cs_link.m_height*-0.5;
	for(int i=0;i<n1;i++)
	{
		//沿着走向方向
		AcGePoint3d pt = origin + cs_link.m_headV*cs_link.m_gas_radius*(i+1);
		for(int j=0;j<n2;j++)
		{
			//沿着倾向方向
			pt += cs_link.m_dipV*cs_link.m_gas_radius;
			pts.append(pt);
		}
	}
	return true;
}

bool RcuHelper::GetDrillSiteInsertPt( const AcDbObjectId& drill_site, AcGePoint3d& insertPt )
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

	insertPt = pDS->getInsertPt();

	actrTransactionManager->endTransaction();
	return true;
}

bool RcuHelper::GetCoalSurfInsertPt( const AcDbObjectId& coal_surf, AcGePoint3d& insertPt )
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return false;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, coal_surf, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	CoalSurface* pCS = CoalSurface::cast( pObj );
	if(pCS == 0)
	{
		actrTransactionManager->abortTransaction();
		return false;
	}

	insertPt = pCS->getInsertPt();

	actrTransactionManager->endTransaction();
	return true;
}

bool RcuHelper::SetDrillSitePt(const AcDbObjectId& drill_site, const AcGePoint3d& insertPt)
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

	actrTransactionManager->endTransaction();
	return true;
}

bool RcuHelper::GetDrillSitePt(const AcDbObjectId& drill_site, AcGePoint3d& insertPt)
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return false;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, drill_site, AcDb::kForRead ) )
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

	insertPt = pDS->getInsertPt();

	actrTransactionManager->endTransaction();
	return true;
}

bool RcuHelper::CreateDrillSite(const AcGePoint3d& pt, DrillSiteLink& ds_link, CoalSurfaceLink& cs_link)
{
	//计算煤层参数
	if(!RcuHelper::CaculCoalSurfParam(ds_link, cs_link)) return false;

	AcGePoint3d origin = ds_link.m_pt;
	AcGePoint3d cnt = cs_link.m_pt;

	//文档锁切换
	DocumentLockSwitch lock_switch;

	//新建钻场并设置插入点坐标
	DrillSite* pDS = new DrillSite();
	pDS->setInsertPt(pt);
	//pDS->enableFollow(true); // 开启跟随效果

	CoalSurface* pCS = new CoalSurface();
	pCS->setInsertPt(pt + (cnt - origin));
	//pCS->enableFollow(true); // 开启跟随效果

	//添加钻场到cad图形数据库
	if(!ArxUtilHelper::PostToModelSpace(pDS))
	{
		delete pDS; pDS = 0;
		return  false;
	}

	//煤层关联到钻场
	pCS->setRelatedGE(pDS->objectId());
	//添加煤层到cad图形数据库
	if(!ArxUtilHelper::PostToModelSpace(pCS))
	{
		delete pCS; pCS = 0;
		//煤层提交失败,则删除已添加的钻场
		ArxEntityHelper::EraseObject(pDS->objectId(), true);
		return false;
	}

	//通过DataLink修改钻场和煤层的数据
	ds_link.setDataSource(pDS->objectId());
	ds_link.updateData(true);
	cs_link.setDataSource(pCS->objectId());
	cs_link.updateData(true);

	//添加煤层钻孔(终孔)
	if(!RcuHelper::CreateClosePores(pCS->objectId(), cs_link)) return false;

	//计算煤层面钻孔的
	return true;
}


bool RcuHelper::ModifyDrillSitePt(const AcDbObjectId& drill_site, DrillSiteLink& ds_link)
{
	if(drill_site.isNull()) return false;

	//AcDbObjectId drill_site;
	//if(!DrawHelper::GetHostGE(drill_site, drill_site)) return false;

	if(!RcuHelper::ReadDrillSiteData(drill_site, ds_link)) return false;

	//获取钻场的插入点坐标
	AcGePoint3d pt;
	if(!RcuHelper::GetDrillSiteInsertPt(drill_site, pt)) return false;

	AcGePoint3d insertPt;
	// 	if(!RcuHelper::CaculDrillSitePt(ds_link, ds_link, pt, insertPt, linkPt)) return false;

	//修改钻场图元的几何点坐标
	return RcuHelper::SetDrillSitePt(drill_site, insertPt);
}

bool RcuHelper::CreateOpenPores(const AcDbObjectId& drill_site, DrillSiteLink& ds_link)
{
	//获取钻场的几何坐标
	AcGePoint3d insertPt;
	if(!RcuHelper::GetDrillSitePt(drill_site, insertPt)) return false;

	//计算相对坐标
	AcGePoint3dArray pts;
	if(!RcuHelper::CaculRelativeOpenPorePts(ds_link, pts)) return false;

	//计算底帮的基点(连接点向下偏移一半的高度距离)
	AcGePoint3d xoy_origin = insertPt;
	//新建钻场并设置插入点坐标
	for(int i=0;i<pts.length();i++)
	{
		//重新调整坐标
		AcGePoint3d pt = pts[i];

		Pore* pPore = new Pore();
		//基于xoy平面
		pPore->setInsertPt(xoy_origin + pt.asVector());
		pPore->setRelatedGE(drill_site);

		//添加钻场到cad图形数据库
		if(!ArxUtilHelper::PostToModelSpace(pPore))
		{
			delete pPore; pPore = 0;
			continue;
		}
		else
		{
			//这里涉及到2个坐标:
			//平面投影坐标基于xoy平面，而钻场打钻的那面墙基于xoz平面
			AcGePoint3d xoz_origin = ds_link.m_pt;

			PoreLink pore_link;
			pore_link.setDataSource(pPore->objectId());
			pore_link.m_pore_num = i + ds_link.m_start;
			pore_link.m_pore_size = ds_link.m_pore_size;
			//基于xoz平面,所以要把点pt的y和z互换
			std::swap(pt.y, pt.z);
			pore_link.m_pt = xoz_origin + pt.asVector();
			pore_link.updateData(true);
		}
	}

	return true;
}

bool RcuHelper::CreateClosePores(const AcDbObjectId& coal_surf, CoalSurfaceLink& cs_link)
{
	//计算煤层面的钻孔坐标
	AcGePoint3dArray pts;
	if(!RcuHelper::CaculRelativeClosePorePts(cs_link, pts)) return false;

	//获取煤层的插入点坐标
	AcGePoint3d insertPt;
	if(!RcuHelper::GetCoalSurfInsertPt(coal_surf, insertPt)) return false;

	//新建钻场并设置插入点坐标
	for(int i=0;i<pts.length();i++)
	{
		//重新调整坐标
		AcGePoint3d pt = pts[i];

		Pore* pPore = new Pore();
		//基于xoy平面
		pPore->setInsertPt(insertPt + pt.asVector());
		pPore->setRelatedGE(coal_surf);

		//添加钻场到cad图形数据库
		if(!ArxUtilHelper::PostToModelSpace(pPore))
		{
			delete pPore; pPore = 0;
			continue;
		}
		else
		{
			PoreLink pore_link;
			pore_link.setDataSource(pPore->objectId());
			pore_link.m_pore_num = i + 1;
			pore_link.m_pore_size = cs_link.m_gas_radius;
			pore_link.m_pt = cs_link.m_pt + pt.asVector();
			pore_link.updateData(true);
		}
	}

	return true;
}

bool RcuHelper::ModifyDrillSiteRelatedGEs(const AcDbObjectId& drill_site, DrillSiteLink& ds_link)
{
	return CreateOpenPores(drill_site, ds_link);
}

bool RcuHelper::ModifyCoalSurfRelatedGEs(const AcDbObjectId& coal_surf, CoalSurfaceLink& cs_link)
{
	return CreateClosePores(coal_surf, cs_link);
}

bool RcuHelper::ModifyDrillSiteRelatedGEs(const AcDbObjectId& drill_site, DrillSiteLink& ds_link, CoalSurfaceLink& cs_link)
{
	//查找钻场关联的煤层
	AcDbObjectId coal_surf;
	RcuHelper::GetRelatedCoalSurface(drill_site, coal_surf);
	if(coal_surf.isNull()) return false;

	if(!drill_site.isNull()) 
	{
		ClearRelatedOpenPores(drill_site);
		ClearRelatedClosePores(coal_surf);
	}

	//重新计算煤层参数
	if(!RcuHelper::CaculCoalSurfParam(ds_link, cs_link)) return false;

	//更新数据到煤层图元里
	if(!cs_link.updateData(true)) return false;

	//修改煤层关联的图元
	if(!ModifyCoalSurfRelatedGEs(coal_surf, cs_link)) return false;

	ds_link.setDataSource(drill_site);
	ds_link.updateData(false);

	//更新数据到钻场图元里
	if(ds_link.updateData(true))
	{
		//修改钻场关联的图元
		ModifyDrillSiteRelatedGEs(drill_site, ds_link);
	}

	return true;
}
