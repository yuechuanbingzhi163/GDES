#include "StdAfx.h"
#include "Rcu2.h"
#include "RcuDataLink.h"
#include "SwitchHelper.h"
#include "RcuHelper.h"
#include "CreatReport.h"
#include <algorithm>

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

#include "../DefGE/RcuGE.h"
#include "../DefGE/Pore.h"
#include "../DefGE/DrillSite.h"
#include "../DefGE/CoalSurface.h"

void RcuHelper::FindAllDrillSite(AcDbObjectIdArray& drill_sites)
{
	ArxDataTool::GetEntsByType(_T("DrillSite"), drill_sites, true);
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
	//向量在xoy平面上的正交投影
	AcGeVector3d u = v.orthoProject(AcGeVector3d::kZAxis);
	//acutPrintf(_T("\n投影值:x=%.3f, y=%.3f z=%.3f"), u.x, u.y, u.z);

	//angleTo求的是夹角，范围在[0,180]
	//计算向量u与Y轴的夹角(偏角)
	ang1 = u.angleTo(AcGeVector3d::kYAxis);
	if(u.x < 0) ang1 *= -1;

	//计算向量u与v的夹角(仰角)，范围在[0,180]
	ang2 = u.angleTo(v);
	if(v.z < 0) ang2 *= -1;
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
	//设置钻场距离煤层的垂距
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

bool RcuHelper::CaculRelativeOpenPorePts(CoalSurfaceLink& cs_link, DrillSiteLink& ds_link, AcGePoint3dArray& pts)
{
	//计算钻孔的行和列上的分布个数
	int n1,n2;
	CaculPoreNums(n1,n2,cs_link.m_width,cs_link.m_height,2*cs_link.m_gas_radius);

	//计算钻孔的坐标
	double gap1 = ds_link.m_width/(n1+1);
	double gap2 = ds_link.m_height/(n2+1);
	//acutPrintf(_T("\ngap1:%.3f, gap2:%.3f"), gap1, gap2);

	AcGeVector3d headV(AcGeVector3d::kXAxis);
	AcGeVector3d dipV(AcGeVector3d::kZAxis);

	//以矩形左下角为基点(便于计算)
	AcGePoint3d origin = AcGePoint3d::kOrigin + headV*(-0.5)*ds_link.m_width + dipV*0;
	for(int i=0;i<n1;i++)
	{
		AcGePoint3d pt = origin + headV*gap1*(i + 1);
		for(int j=0;j<n2;j++)
		{
			pt += dipV * gap2;
			pts.append(pt);
		}
	}
	return true;
}

bool RcuHelper::CaculRelativeClosePorePts(CoalSurfaceLink& cs_link, AcGePoint3dArray& pts)
{
	//计算行和列上钻孔的个数
	int n1,n2;
	CaculPoreNums(n1,n2,cs_link.m_width,cs_link.m_height,2*cs_link.m_gas_radius);

	//矩形左下角的点坐标,从这个点开始计算
	AcGePoint3d origin = AcGePoint3d::kOrigin + cs_link.m_headV*cs_link.m_width*-0.5 + cs_link.m_dipV*cs_link.m_height*-0.5;
	for(int i=0;i<n1;i++)
	{
		//沿着走向方向
		AcGePoint3d pt = origin + 2*cs_link.m_headV*cs_link.m_gas_radius*(i+1);
		for(int j=0;j<n2;j++)
		{
			//沿着倾向方向
			pt += 2*cs_link.m_dipV*cs_link.m_gas_radius;
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


bool RcuHelper::SetCoalSurfParams( const AcDbObjectId& coal_surf, AcGePoint3d& insertPt )
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

	pCS->setInsertPt(insertPt);
	//pCS->setHeight(h);
	//pCS->setWidth(w);
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

	//文档锁切换
	DocumentLockSwitch lock_switch;

	//新建钻场并设置插入点坐标
	DrillSite* pDS = new DrillSite();
	pDS->setInsertPt(pt);
	//pDS->enableFollow(true); // 开启跟随效果

	CoalSurface* pCS = new CoalSurface();
	//pCS->setInsertPt(pt + (cnt - origin));
	cs_link.m_GEwidth = ds_link.m_width + ds_link.m_left + ds_link.m_right;
	cs_link.m_GEheight = ds_link.m_height + ds_link.m_top + ds_link.m_bottom;
	cs_link.updateData(true);
	pCS->setInsertPt(pt + AcGeVector3d(0,1,0)*(cs_link.m_GEheight*0.5+ds_link.m_height*1.618));
	//pCS->setWidth(w);
	//pCS->setHeight(h);
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
	if(!RcuHelper::CreateClosePores(pCS->objectId(), cs_link, ds_link)) return false;

	//计算煤层面钻孔的
	return true;
}


bool RcuHelper::ModifyCoalSurfParams(const AcDbObjectId& drill_site)
{
	if(drill_site.isNull()) return false;

	AcDbObjectIdArray coal_surfs;
	DrawHelper::GetTagGEById2(drill_site, _T("CoalSurface"),coal_surfs);
	if(coal_surfs.isEmpty()) return false;
	AcDbObjectId coal_surf = coal_surfs[0];

	//获取钻场数据
	DrillSiteLink ds_link;
	if(!RcuHelper::ReadDrillSiteData(drill_site, ds_link)) return false;

	//获取煤层数据
	CoalSurfaceLink cs_link;
	if(!RcuHelper::ReadCoalSurfaceData(coal_surf,cs_link)) return false;

	//重新计算煤层参数
	if(!RcuHelper::CaculCoalSurfParam(ds_link, cs_link)) return false;

	//获取钻场的插入点坐标
	AcGePoint3d pt;
	if(!RcuHelper::GetDrillSiteInsertPt(drill_site, pt)) return false;
	
	cs_link.m_GEwidth = ds_link.m_width + ds_link.m_left + ds_link.m_right;
	cs_link.m_GEheight = ds_link.m_height + ds_link.m_top + ds_link.m_bottom;
	cs_link.updateData(true);
	AcGePoint3d insertPt = pt + AcGeVector3d(0,1,0)*(cs_link.m_GEheight*0.5+ds_link.m_height*1.618);

	if(!RcuHelper::SetCoalSurfParams(coal_surf,insertPt)) return false;
	// 	if(!RcuHelper::CaculDrillSitePt(ds_link, ds_link, pt, insertPt, linkPt)) return false;
	return true;
	//修改钻场图元的几何点坐标
	//return RcuHelper::SetDrillSitePt(drill_site, insertPt);
}

bool RcuHelper::CreateOpenPores(const AcDbObjectId& drill_site, CoalSurfaceLink& cs_link, DrillSiteLink& ds_link)
{
	//获取钻场的几何坐标
	AcGePoint3d insertPt;
	if(!RcuHelper::GetDrillSitePt(drill_site, insertPt)) return false;

	//计算相对坐标
	AcGePoint3dArray pts;
	if(!RcuHelper::CaculRelativeOpenPorePts(cs_link, ds_link, pts)) return false;

	//acutPrintf(ArxUtilHelper::Point3dToString(ds_link.m_pt)+_T("\n"));
	//新建钻场并设置插入点坐标
	for(int i = 0;i < pts.length(); i++)
	{
		//重新调整坐标
		AcGePoint3d pt = pts[i];

		Pore* pPore = new Pore();
		//基于xoz平面,所以要把点pt的y和z互换
		AcGePoint3d pt2 = pt;
		std::swap(pt2.y, pt2.z);
		pPore->setInsertPt(insertPt + pt2.asVector());
		pPore->setRelatedGE(drill_site);

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
			pore_link.m_pore_size = ds_link.m_pore_size;
			//这里要用钻场的底板中心点真实坐标计算钻孔的真实坐标
			pore_link.m_pt = ds_link.m_pt + pt.asVector();
			//acutPrintf(ArxUtilHelper::Point3dToString(pore_link.m_pt)+_T(" "));
			pore_link.updateData(true);
		}
	}

	return true;
}

bool RcuHelper::CreateClosePores(const AcDbObjectId& coal_surf, CoalSurfaceLink& cs_link, DrillSiteLink& ds_link)
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
		AcGeVector3d v = pt.asVector();
		AcGeVector3d normalYV(AcGeVector3d::kYAxis);
		v = v.orthoProject(normalYV);
		std::swap(v.y, v.z);
		pPore->setInsertPt(insertPt + v);
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
			pore_link.m_pore_size = ds_link.m_pore_size;
			pore_link.m_pt = cs_link.m_pt + pt.asVector();
			pore_link.updateData(true);
		}
	}

	return true;
}

bool RcuHelper::ModifyDrillSitePoreGEs( const AcDbObjectId& drill_site, CoalSurfaceLink& cs_link, DrillSiteLink& ds_link )
{
	return CreateOpenPores(drill_site, cs_link, ds_link);
}

bool RcuHelper::ModifyCoalSurfRelatedGEs(const AcDbObjectId& coal_surf, CoalSurfaceLink& cs_link, DrillSiteLink& ds_link)
{
	return CreateClosePores(coal_surf, cs_link,ds_link);
	//return true;
}

bool RcuHelper::ModifyDrillSiteRelatedGEs(const AcDbObjectId& drill_site, CoalSurfaceLink& cs_link, DrillSiteLink& ds_link)
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
	if(!ModifyCoalSurfRelatedGEs(coal_surf, cs_link, ds_link)) return false;

	ds_link.setDataSource(drill_site);
	ds_link.updateData(false);

	//更新数据到钻场图元里
	if(ds_link.updateData(true))
	{
		//修改钻场关联的图元
		ModifyDrillSitePoreGEs(drill_site, cs_link, ds_link);
	}

	return true;
}

void RcuHelper::CaculPoreNums( int& row_num, int& col_num, double width, double height, double gap )
{
	//抽采范围的宽度上布置钻孔个数
	row_num = int(width/gap);
	//抽采范围的高度上布置钻孔个数
	col_num = int(height/gap);
}

static void GetDocPath( CString& defaultPath )
{
	TCHAR pPath[MAX_PATH]={0};
	SHGetSpecialFolderPath(NULL,pPath,CSIDL_PERSONAL,0);

	defaultPath.Format(_T("%s"),pPath);
}

static BOOL SaveAndOpenReport(CString outName,const AcDbObjectId& drill_site)
{
	TCHAR szFileFilter[] = _T("doc文档(*.doc)|*.doc||");
	TCHAR szFileExt[] = _T("doc");

	CString defaultPath;
	GetDocPath(defaultPath);

	CFileDialog dlg(FALSE,szFileExt,defaultPath,OFN_READONLY,szFileFilter);///TRUE为OPEN对话框，FALSE为SAVE AS对话框

	TCHAR* pFileBuf = new TCHAR[MAX_PATH * 1024];
	if (NULL == pFileBuf)
	{
		return FALSE;
	}
	_tcscpy(pFileBuf,outName);

	dlg.m_ofn.lpstrFile = pFileBuf;
	dlg.m_ofn.nMaxFile = MAX_PATH * 1024;
	CString selectedPath;
	if(IDOK == dlg.DoModal())
	{
		selectedPath = dlg.GetPathName();
	}

	else
	{
		return FALSE;
	}

	delete pFileBuf;
	pFileBuf = NULL;

	acutPrintf( _T( "\n报告生成中...\n" ) );
	//初始化com
	if(initword())
	{
		if(!CreatReport(selectedPath,drill_site)) 
		{
			//卸载com
			uninitword();
			acutPrintf( _T( "\n报告生成失败!\n" ) );
			return FALSE;
		}
		//卸载com
		uninitword();
	}
	acutPrintf( _T( "\n报告保存到:%s" ),selectedPath);
	if(IDYES == AfxMessageBox(_T("报告生成成功!是否现在打开?"),MB_YESNO))
	{
		if(initword())
		{
			OpenWordDocument(selectedPath);
			//卸载com
			uninitword();
		}
	}
	return TRUE;
}

void RcuHelper::CreatReportHelper(const AcDbObjectId& drill_site)
{
	CAcModuleResourceOverride myResources;

	TCHAR szFileFilter[] = _T("doc文档(*.doc)|*.doc||");
	TCHAR szFileExt[] = _T("doc");
	CString defaultPath;
	GetDocPath(defaultPath);
	CString outName;
	if(drill_site.isNull())
		outName = _T("石门设计报告");
	else
	{
		//获取钻场数据
		DrillSiteLink ds_link;
		if(!RcuHelper::ReadDrillSiteData(drill_site, ds_link)) return;
		outName.Format(_T("%s钻场--石门设计报告"),ds_link.m_name);
	}
	SaveAndOpenReport(outName,drill_site);
}
