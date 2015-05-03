#include "stdafx.h"
#include "DrawCmd.h"

// 核心模块MineGE辅助类
#include "../MineGE/LinkedGE.h"
#include "../MineGE/TagGE.h"
#include "../DefGE/WindDirection.h"
#include "../DefGE/ArcTunnel.h"
#include "../DefGE/TTunnel.h"
#include "../DefGE/GasFlowTagGE.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

//#include "../MineGE/PropertyDataUpdater.h"

#include <cmath>


static void GetSEPts( const AcDbObjectIdArray& objIds, AcGePoint3dArray& spts, AcGePoint3dArray& epts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        LinkedGE* pEdge = LinkedGE::cast( pObj );
        if( pEdge == 0 ) continue;

        AcGePoint3d spt, ept;
        pEdge->getSEPoint( spt, ept );

        spts.append( spt );
        epts.append( ept );
    }
    actrTransactionManager->endTransaction();
}

static void EraseAllDirections()
{
    AcDbObjectIdArray dirs;
    DrawHelper::FindMineGEs( _T( "WindDirection" ), dirs );
	if( 0 >= dirs.length()) return;
    ArxEntityHelper::EraseObjects( dirs, true );
}

static void EraseAllGasFlow()
{
	AcDbObjectIdArray dirs;
	DrawHelper::FindMineGEs( _T( "GasFlowTagGE" ), dirs );
	if( 0 >= dirs.length()) return;
	ArxEntityHelper::EraseObjects( dirs, true );
}

static void AddDirection( const AcDbObjectId& objId, const AcGePoint3d& spt, const AcGePoint3d& ept,int colorIndx )
{
    AcGeVector3d v = ept - spt;
    double angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
	AcDbObject* pObj;
	if( Acad::eOk != acdbOpenObject( pObj, objId, AcDb::kForRead ) ) 
	{
		acutPrintf(_T("\n操作失败!"));
		return;
	}

	//TTunnel* pTTunel = TTunnel::cast( pObj );
	//把对象强制转换成圆弧巷道对象
	ArcTunnel* pArcTunnel = ArcTunnel::cast( pObj );
	pObj->close();

	AcGePoint3d insertPt;
	
	//bool isOppositeDir = false;
	
	if( pArcTunnel == 0 )//不是圆弧巷道
	{
		insertPt = spt + v * 0.5;
	}

	else//圆弧巷道
	{
		insertPt = pArcTunnel->getArcCntPt();
	}

	angle = DrawCmd::ControlDirByMethods(objId,angle);
	WindDirection* pDir = new WindDirection( insertPt, angle ); // 巷道中心位置
	pDir->setRelatedGE( objId );
	pDir->setColorIndex(colorIndx);
	ArxUtilHelper::PostToModelSpace( pDir );	
}

static void AddGasFlow( const AcDbObjectId& objId, const AcGePoint3d& spt, const AcGePoint3d& ept )
{
	AcGeVector3d v = ept - spt;
	double angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
	AcDbObject* pObj;
	if( Acad::eOk != acdbOpenObject( pObj, objId, AcDb::kForRead ) ) 
	{
		acutPrintf(_T("\n操作失败!"));
		return;
	}

	pObj->close();
	AcGePoint3d insertPt = spt + v * 0.5;


	angle = DrawCmd::ControlDirByMethods(objId,angle);
	GasFlowTagGE* pFL = new GasFlowTagGE( insertPt, angle );
	pFL->setRelatedGE( objId );
	ArxUtilHelper::PostToModelSpace( pFL );	
}

static int getColorIndx(const AcDbObjectId& objId)
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return 7;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) ) return 7;

	LinkedGE* pEdge = LinkedGE::cast( pObj );
	if( pEdge == 0 ) return 7;

	int colorIndx = pEdge->colorIndex();
	//AcGePoint3d spt, ept;
	//pEdge->getSEPoint( spt, ept );
	actrTransactionManager->endTransaction();
	return colorIndx;
}

static void ExcludeGE(const CString& className,AcDbObjectIdArray& objIds)
{
	for(int i = 0; i < objIds.length(); i++)
	{
		if(ArxUtilHelper::IsEqualType(className,objIds[i]))
		{
			objIds.removeAt(i);
		}
	}
}

void DrawCmd::AutoDirection()
{
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "LinkedGE" ), objIds );

	ExcludeGE(_T("GasPumpGE"),objIds);
	ExcludeGE(_T( "DrillGE" ),objIds);

    // 删除所有的风流方向
    EraseAllDirections();

    AcGePoint3dArray spts, epts;
    GetSEPts( objIds, spts, epts );

    int len = objIds.length();

	for (int i = 0;i < len;i++)
	{
		int colorIndx = getColorIndx(objIds[i]);
		AddDirection(objIds[i],spts[i],epts[i],colorIndx);
	}
}

void DrawCmd::AutoGasFlow()
{
	AcDbObjectIdArray objIds;
	DrawHelper::FindMineGEs( _T( "GasTube" ), objIds );
	if( objIds.isEmpty() ) return;

	// 删除所有的瓦斯流量
	EraseAllGasFlow();

	AcGePoint3dArray spts, epts;
	GetSEPts( objIds, spts, epts );

	int len = objIds.length();
	
	//PropertyDataUpdater::UpdateAllWindGasFlow();

	for (int i = 0;i < len;i++)
	{
		AddGasFlow(objIds[i],spts[i],epts[i]);
	}
}

