#include "StdAfx.h"
#include "NetWorkData.h"
#include "../ArxHelper/ArxUtilHelper.h"
#include "../MineGE/DrawHelper.h"
#include "../MineGE/HelperClass.h"
#include "../DefGE/Chimney.h"
#include "../DefGE/Fan.h"

TWS_DataLink* TWSDatas(	AcDbObjectId objId)
{
	TWS_DataLink *ts = new TWS_DataLink;
	ts->setDataSource(objId);
	ts->updateData(false);

	return ts;
}

Chimney_DataLink* ChimneyDatas(AcDbObjectId objId )
{
	//AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "选择一套风筒:" ) );
	Chimney_DataLink *ts = new Chimney_DataLink;
	ts->setDataSource(objId);
	ts->updateData(false);
	return ts;
}

void TunnelDatas()
{
	AcDbObjectIdArray objIds;
	DrawHelper::FindMineGEs(_T("Tunnel"), objIds);
	int len = objIds.length();
	acutPrintf(_T("\n\n巷道的参数:\n"));
	for(int i = 0; i < len; i++)
	{
		acutPrintf(_T("\n+++++++++++++++++++\n"));
		Tunnel_DataLink *ts = new Tunnel_DataLink;
		ts->setDataSource(objIds[i]);
		ts->updateData(false);
		acutPrintf(_T("\n名称:%s"),ts->name);
		acutPrintf(_T("\n长度:%f"),ts->lenth);
		acutPrintf(_T("\n断面面积:%f"),ts->area);
		ts->updateData(true);
	}
}

static AcGePoint3dArray getControlPts(AcDbObjectId objId)
{
	AcTransaction *pTrans = actrTransactionManager->startTransaction();
	if (0 == pTrans) return NULL;
	AcDbObject *pObj;
	if (Acad::eOk != pTrans->getObject(pObj,objId,AcDb::kForRead)) return NULL;
	Chimney *chim = Chimney::cast(pObj);
	AcGePoint3dArray pts = chim->getControlPoint();
	actrTransactionManager->endTransaction();

	return pts;
}

static AcGePoint3d getFanInsertPt(AcDbObjectId objId)
{
	AcTransaction *pTrans = actrTransactionManager->startTransaction();
	//if (0 == pTrans) return;
	AcDbObject *pObj;
	/*if (Acad::eOk != */pTrans->getObject(pObj,objId,AcDb::kForRead)/*) return*/;
	LocalFan *fan = LocalFan::cast(pObj);
	AcGePoint3d pt = fan->getInsertPt();
	actrTransactionManager->endTransaction();

	return pt;
}


//void SumChimLenth(AcDbObjectId fanObjId)
//{
//	//AcDbObjectId fanObjId = ArxUtilHelper::SelectObject( _T( "请选择一个局扇:" ) );
//	//if( fanObjId.isNull() ) return;
//	//if( !ArxUtilHelper::IsEqualType( _T( "LocalFan" ), fanObjId ) ) 
//	//{
//	//	acutPrintf(_T("\n选择的不是局扇，请确定选择局扇！"));
//	//	return;
//	//}
//
//	//获取风机的插入点作为搜索的第一点
//	AcGePoint3d insertPt = getFanInsertPt(fanObjId);
//
//	AcDbObjectIdArray objIds;
//	DrawHelper::FindMineGEs(_T("Chimney"),objIds);
//	int len = objIds.length();
//
//	//风筒所有的控制点
//	AcGePoint3dArray pts;
//
//	//已经查到在风筒上的点
//	AcGePoint3dArray findedPts;
//	findedPts.append(insertPt);
//
//	//风筒总长度
//	double sumLenth = 0;
//
//	//acutPrintf(_T("\n总段数:%d"),len);
//	for (int i = 0;i < len; i++)
//	{
//		pts = getControlPts(objIds[i]);
//		int numPts = pts.length();
//		Chimney_DataLink *ts = new Chimney_DataLink;
//		ts->setDataSource(objIds[i]);
//		ts->updateData(false);
//
//		if (findedPts.contains(pts[0]) && !findedPts.contains(pts[numPts-1]))
//		{
//			findedPts.append(pts[numPts-1]);
//			sumLenth += ts->lenth;
//			i = -1;
//		}
//		else if (findedPts.contains(pts[numPts-1]) && !findedPts.contains(pts[0]))
//		{
//			findedPts.append(pts[0]);
//			sumLenth += ts->lenth;
//			i = -1;
//		}	
//	}
//	acutPrintf(_T("\n总长度:%f"),sumLenth);
//}
BaseparamDataLink* BaseparamDatas(AcDbObjectId objId)
{
	BaseparamDataLink *bpl = new BaseparamDataLink;
	bpl->setDataSource(objId);
	bpl->updateData(false);
	return bpl;
}

Vent_DataLink* CaculParamDatas(AcDbObjectId objId)
{
	Vent_DataLink *vt = new Vent_DataLink;
	vt->setDataSource(objId);
	vt->updateData(false);
	return vt;
}

double GetMaxValues(double a, double b)
{
	return (a >= b)?a:b;
}

LeakageCacul_DataLink* LeakageCaculDatas(AcDbObjectId objId)
{
	LeakageCacul_DataLink *lcdl = new LeakageCacul_DataLink;
	lcdl->setDataSource(objId);
	lcdl->updateData(false);
	return lcdl;
}