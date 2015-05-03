#include "stdafx.h"
#include "DrawCmd.h"

#include "../MineGE/MineGE.h"
#include "../MineGE/LinkedGE.h"
#include "../MineGE/TagGE.h"
#include "../DefGE/ArcTunnel.h"
#include "../DefGE/Chimney.h"
#include "../DefGE/QTagGE.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

/* 全局函数(实现在PromptTool.cpp) */
extern bool GetClosePtAndAngle( const AcDbObjectId& objId, AcGePoint3d& pt, double& angle );
extern bool GetSEPt( const AcDbObjectId& objId, AcGePoint3d& spt, AcGePoint3d& ept );

static bool GetInsertPt( const AcDbObjectId& objId, AcGePoint3d& pt )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    DirectionTagGE* pDir = DirectionTagGE::cast( pObj );
    if( pDir == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    pt = pDir->getInsertPt();

    actrTransactionManager->endTransaction();

    return true;
}

static void CacLineClosePt( const AcGePoint3d& spt, const AcGePoint3d& ept,
                            AcGePoint3d& pt, double& angle )
{
    // 构造一条几何线段
    AcGeLineSeg3d line( spt, ept );
    pt = line.closestPointTo( pt ); // 计算距离中线最近的点

    AcGeVector3d v = ept - spt;
    angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
}

//static double getArcAngle(const AcGePoint3d& spt, const AcGePoint3d& ept, const AcGePoint3d& thirdPt,AcGePoint3d insertPt)
//{
//	AcGeCircArc3d arc(spt,thirdPt,ept);
//	AcGePoint3d cnt = arc.center();
//
//	AcGeVector3d cntToPtVec = insertPt - cnt;
//	cntToPtVec.rotateBy(PI*0.5,AcGeVector3d::kZAxis);
//	cntToPtVec.normalize();
//
//	AcGeVector3d sptToInsertPtVec = insertPt - spt;
//	double sptInsertPtLenth = sptToInsertPtVec.lengthSqrd();
//
//	AcGeVector3d eptToInsertPtVec = insertPt - ept;
//	double eptInsertPtLenth = eptToInsertPtVec.lengthSqrd();
//
//	if (sptInsertPtLenth > eptInsertPtLenth)
//	{
//		AcGeVector3d thirdPtToEptVec = ept - thirdPt;
//		thirdPtToEptVec.normalize();
//		double angleTE = cntToPtVec.angleTo(thirdPtToEptVec,-AcGeVector3d::kZAxis);
//		if (angleTE > 0.5 * PI)
//		{
//			cntToPtVec = - cntToPtVec;
//		}
//
//	}
//
//	else
//	{
//		AcGeVector3d sptToThirdPtVec = thirdPt - spt;
//		sptToThirdPtVec.normalize();
//		double angleST = cntToPtVec.angleTo(sptToThirdPtVec,-AcGeVector3d::kZAxis);
//		if (angleST > 0.5 * PI)
//		{
//			cntToPtVec = - cntToPtVec;
//		}
//
//	}
//
//	return cntToPtVec.angleTo(AcGeVector3d::kXAxis,-AcGeVector3d::kZAxis);
//
//}

//static void CacArcClosePt( const AcGePoint3d& spt, const AcGePoint3d& ept, const AcGePoint3d& thirdPt,
//                           AcGePoint3d& pt, double& angle )
//{
//    AcGeCircArc3d arc( spt, thirdPt, ept );
//    AcGePoint3d cnt = arc.center();
//    double radius = arc.radius();
//
//    AcGeVector3d v = ept - cnt;
//    AcGeVector3d v2 = ept - spt;
//    AcGeVector3d v3 = v.crossProduct( v2 );
//    int c = ( v3.z > 0 ? 1 : -1 );
//
//    // 距离圆弧最近点
//    pt = arc.closestPointTo( pt );
//
//    // 计算角度
//    v = pt - cnt;
//    v.rotateBy( c * PI / 2, AcGeVector3d::kZAxis );
//
//    //angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
//	angle = getArcAngle(spt,ept,thirdPt,pt);
//}

static void ResetDirectionGE( const AcDbObjectId& objId, const AcDbObjectId& host, const AcGePoint3d& pt, double angle )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }
    DirectionTagGE* pDirTag = DirectionTagGE::cast( pObj );
    if( pDirTag == 0 )
    {
        actrTransactionManager->abortTransaction();
        return;
    }
    pDirTag->setInsertPt( pt );
    pDirTag->setDirectionAngle( angle );
    pDirTag->setRelatedGE( host ); // 关联图元

    actrTransactionManager->endTransaction();
}

void DrawCmd::JunctionEnclose( void )
{
    acutPrintf( _T( "\n处理闭合情况...\n" ) );

    AcGePoint3d pt;
    if( !ArxUtilHelper::PromptPt( _T( "\n选择需要闭合处理的坐标：" ), pt ) ) return;

    // 处理闭合
    DrawHelper::LinkedGEJunctionClosure( pt );
}

static void ResetArcDirection(AcDbObjectId objId)
{

	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return;
	}

	ArcTunnel* pArcTunnel = ArcTunnel::cast( pObj );
	if( pArcTunnel == 0 )
	{
		actrTransactionManager->abortTransaction();
		return;
	}

	pArcTunnel->reverse();  // 交换始末点坐标

	double angle; // 反向后的角度

	AcDbObjectIdArray objIds;
	DrawHelper::GetTagGEById2( objId, _T( "DirectionTagGE" ), objIds );
	int len = objIds.length();
	for( int i = 0; i < len; i++ )
	{
		if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

		DirectionTagGE* pDir = DirectionTagGE::cast( pObj );
		AcGePoint3d insertPt = pDir->getInsertPt();
		angle = pArcTunnel->getArcAngle(insertPt);
		//2014/8/22修改增加下面语句，为了使压入为主的时候方向相反
		angle = DrawCmd::ControlDirByMethods(objId,angle);

		pDir->setDirectionAngle( angle );
	}

	actrTransactionManager->endTransaction();
}

static void ReverseTunnelDirection( const AcDbObjectId& objId )
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return;
	}

	LinkedGE* pEdge = LinkedGE::cast( pObj );
	if( pEdge == 0 )
	{
		actrTransactionManager->abortTransaction();
		return;
	}

	pEdge->reverse();  // 交换始末点坐标

	double angle = pEdge->getAngle(); // 反向后的角度


	// 调整所有方向类图元方向，包括风流方向
	AcDbObjectIdArray objIds;
	DrawHelper::GetTagGEById2( objId, _T( "DirectionTagGE" ), objIds );
	int len = objIds.length();
	for( int i = 0; i < len; i++ )
	{
		if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

		DirectionTagGE* pDir = DirectionTagGE::cast( pObj );

		//2014/8/22修改增加下面语句，为了使压入为主的时候方向相反
		angle = DrawCmd::ControlDirByMethods(objId,angle);

		pDir->setDirectionAngle( angle );
	}

	actrTransactionManager->endTransaction();
}

void DrawCmd::ReverseDirection()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "\n请选择一个分支类图元 [巷道、瓦斯管路等]：" ) );
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

	if( ArxUtilHelper::IsEqualType( _T( "ArcTunnel" ), objId ) )
	{
		ResetArcDirection(objId);
	}

	else
	{
		//DrawHelper::ReverseDirection( objId );
		ReverseTunnelDirection( objId );
	}

}

void DrawCmd::ReBindEdge()
{
    CString msg = _T( "请选择一个标签图元[风门/调节风窗/挡风墙/风机/测风站/风流方向/风桥]:" );
    AcDbObjectId objId = ArxUtilHelper::SelectObject( msg );
    if( objId.isNull() ) return;
    if( !ArxUtilHelper::IsEqualType( _T( "DirectionTagGE" ), objId ) ) return;

    CString msg2 = _T( "请选择一个分支图元[巷道/采掘工作面/硐室]:" );
    AcDbObjectId host = ArxUtilHelper::SelectObject( msg2 );
    if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), host ) ) return;

    // 获取标签图元的插入点坐标
    AcGePoint3d insertPt;
    if( !GetInsertPt( objId, insertPt ) ) return;

    AcGePoint3d pt;
    if( !ArxUtilHelper::PromptPt2( _T( "\n请在分支图元附近选择一个插入点: " ), insertPt, pt ) ) return;

    double angle;
    GetClosePtAndAngle( host, pt, angle );

    // 重新关联tag
    ResetDirectionGE( objId, host, pt, angle );
}

void DrawCmd::ZoomMineGE()
{
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个图元实体:" ) );
    if( objId.isNull() ) return;

    ArxEntityHelper::ZoomToEntity( objId );
}

static bool GetEntStyle( const AcDbObjectId& objId, int& colorIndx, AcDbObjectId& layerId, AcDbObjectId& lineTypeId )
{

	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return false;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}
	LinkedGE* pEdge = LinkedGE::cast( pObj );
	if( pEdge == 0 )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}
	//if( !ArxUtilHelper::IsEqualType( _T( "ArcTunnel" ), objId ) )
	colorIndx = pEdge->colorIndex();
	layerId = pEdge->layerId();
	lineTypeId = pEdge->linetypeId();
	actrTransactionManager->endTransaction();
	return true;
}


static bool SetSEPt( const AcDbObjectId& objId, const AcGePoint3d& spt, const AcGePoint3d& ept )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    LinkedGE* pEdge = LinkedGE::cast( pObj );
    if( pEdge == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
	//if( !ArxUtilHelper::IsEqualType( _T( "ArcTunnel" ), objId ) )
    pEdge->setSEPoint( spt, ept );
    actrTransactionManager->endTransaction();

    return true;
}

static bool SetArcSEPt( const AcDbObjectId& objId, const AcGePoint3d& spt, const AcGePoint3d& ept,const AcGePoint3d& thirdPt )
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return false;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}
	ArcTunnel* pArcTunnel = ArcTunnel::cast( pObj );
	if( pArcTunnel == 0 )
	{
		actrTransactionManager->abortTransaction();
		return false;
	}
	//if( !ArxUtilHelper::IsEqualType( _T( "ArcTunnel" ), objId ) )
	pArcTunnel->setSEPoint( spt, ept );
	pArcTunnel->setThirdPt(thirdPt);
	actrTransactionManager->endTransaction();

	return true;
}

static bool GetSplitPoint( const AcGePoint3d& spt, const AcGePoint3d& ept, AcGePoint3d& pt )
{
    acedInitGet( RSG_NONULL, _T( "Dist" ) );

    ads_point _pt;
    int rt = acedGetPoint( asDblArray( spt ), _T( "请指定分割点坐标: [距离(D)]" ), _pt );

    if( rt == RTKWORD )
    {
        acedInitGet( RSG_NONULL | RSG_NOZERO | RSG_NONEG, NULL );
        double d = 0;
        if( RTNORM != acedGetDist( asDblArray( spt ), _T( "请输入距离: " ), &d ) ) return false;

        AcGeVector3d v = ept - spt;
        v.normalize();
        pt = spt + v * d;
    }
    else if( rt == RTNORM )
    {
        pt = asPnt3d( _pt );
    }
    else
    {
        return false;
    }

    return true;
}

static AcGePoint3d getOnePtOnArc(AcGePoint3d startPt,AcGePoint3d endPt,AcGeCircArc3d arc)
{
	//定义一个点，返回用于创建弧线，即第三点
	AcGePoint3d thirdPt;

	//始末节点的中点
	AcGePoint3d cntArcPt = AcGePoint3d((startPt.x+endPt.x)/2,(startPt.y+endPt.y)/2,0);

	//获得经过圆心和弦中点的单位向量，用于构造相交线
	AcGeVector3d v = endPt - startPt;
	v.rotateBy(PI*0.5,AcGeVector3d::kZAxis);
	v.normalize();

	//与弧线相交的直线
	AcGeLine3d line(cntArcPt,v);

	//n为交点的个数,getPt1,getPt2分别为第一个交点和第二个交点,如果n为1，则getPt2无效
	int n;
	AcGePoint3d getPt1,getPt2;

	while (Adesk::kTrue == arc.intersectWith(line,n,getPt1,getPt2))
	{
		//直线每次旋转0.05*180/PI度，然后继续按条件搜寻
		v.rotateBy(0.05,AcGeVector3d::kZAxis);
		line.set(cntArcPt,v);

		if( 1 == n)
		{
			thirdPt = getPt1;
			break;
		}
	}

	return thirdPt;
}

static void splitArcTunnel(AcDbObjectId objId,AcGePoint3d spt,AcGePoint3d ept,AcGePoint3d pt)
{
	AcDbObject* pObj;
	if(Acad::eOk != acdbOpenObject( pObj, objId, AcDb::kForWrite ))return;
	ArcTunnel *pArcTunenl = ArcTunnel::cast(pObj);
	pObj->close();
	AcDbObjectId layerId,lineTypeId;
	int colorIndx;
	GetEntStyle(objId,colorIndx,layerId,lineTypeId);

	AcGeCircArc3d arc( spt, pArcTunenl->getArcCntPt(), ept);
	AcGePoint3d cnt = arc.center();	//圆弧原点
	double radius = arc.radius();	//圆弧半径

	AcGePoint3d thirdPt = getOnePtOnArc(pt,ept,arc);

	CString type;
	DataHelper::GetTypeName( objId, type );

	//创建插入点到末点的一段弧线
	ArcTunnel* pat = ArcTunnel::cast( ArxClassHelper::CreateObjectByType( type ) );
	pat->setSEPoint(pt,ept);
	pat->setThirdPt(thirdPt);
	pat->setLinetype(lineTypeId);
	pat->setColorIndex(colorIndx);
	pat->setLayer(layerId);
	if( !ArxUtilHelper::PostToModelSpace( pat ) )
	{
		// 添加到数据库失败
		delete pat;
	}

	else
	{
		//SetArcSEPt(objId,spt,pt,thirdPt2);
		//SetSEPt( objId, spt, pt );
		
		//删除原来的弧线
		ArxEntityHelper::EraseObject( objId, true );

		thirdPt = getOnePtOnArc(spt,pt,arc);

		//创建始点到插入点的一段弧
		ArcTunnel* pat0 = ArcTunnel::cast( ArxClassHelper::CreateObjectByType( type ) );
		pat0->setSEPoint(spt,pt);
		pat0->setThirdPt(thirdPt);
		pat0->setLinetype(lineTypeId);
		pat0->setColorIndex(colorIndx);
		pat0->setLayer(layerId);

		if( !ArxUtilHelper::PostToModelSpace( pat0 ) )
		{
			// 添加到数据库失败
			delete pat0;
		}
	}
}

void DrawCmd::SplitDeal()
{
	AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择需要分割的图元:" ) );
	if( !ArxUtilHelper::IsEqualType( _T( "LinkedGE" ), objId ) ) return;

	AcGePoint3d spt, ept;
	if( !GetSEPt( objId, spt, ept ) )
	{
		acutPrintf( _T( "\n无法获取始末点坐标" ) );
		return;
	}

	AcGePoint3d pt;
	if( !GetSplitPoint( spt, ept, pt ) ) return;
	//ArxUtilHelper::PromptPt(_T("请指定分割位置: "), pt);

	if( pt == spt || pt == ept )
	{
		acutPrintf( _T( "\n始末点不能作为分割点!!!" ) );
		return;
	}

	SplitByPoint(objId,spt,ept,pt);
}

void DrawCmd::SplitByPoint(AcDbObjectId objId,AcGePoint3d spt,AcGePoint3d ept,AcGePoint3d pt)
{


    double angle;
    GetClosePtAndAngle( objId, pt, angle );
    /*AcGeLine3d L(spt, ept);
    if(!L.isOn(pt))
    {
    	acutPrintf(_T("巷道/工作面之外的点不能作为分割点!!!..."));
    	return;
    }*/

	//LinkedGE* oldEdge = LinkedGE::cast()
	AcDbObjectId layerId,lineTypeId;
	int colorIndx;
	GetEntStyle(objId,colorIndx,layerId,lineTypeId);
    // 新建巷道
    CString type;
    DataHelper::GetTypeName( objId, type );
    LinkedGE* pEdge = LinkedGE::cast( ArxClassHelper::CreateObjectByType( type ) );
    if( pEdge == 0 ) return; // 创建巷道失败
	
	pEdge->setColorIndex(colorIndx);
	pEdge->setLayer(layerId);
	pEdge->setLinetype(lineTypeId);

	//如果是弧线巷道
	if( ArxUtilHelper::IsEqualType( _T( "ArcTunnel" ), objId ) )
	{
		splitArcTunnel(objId,spt,ept,pt);
	}
	else
	{
		pEdge->setSEPoint( pt, ept );
		if( !ArxUtilHelper::PostToModelSpace( pEdge ) )
		{
			// 添加到数据库失败
			delete pEdge;
		}
		else
		{
			// 成功添加到数据库
			// 同时修改原有巷道坐标
			SetSEPt( objId, spt, pt );
			DrawHelper::LinkedGEJunctionClosure( pt );
		}
	}
}

static Chimney* getChimObj(AcDbObjectId objId)
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return NULL;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return NULL;
	}

	Chimney* pChim = Chimney::cast( pObj );
	if( pChim == 0 )
	{
		actrTransactionManager->abortTransaction();
		return NULL;
	}
	actrTransactionManager->endTransaction();
	
	return pChim;
}

static void UpdateChimney(AcDbObjectId objId)
{
	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return;

	AcDbObject* pObj;
	if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
	{
		actrTransactionManager->abortTransaction();
		return;
	}

	Chimney* pChim = Chimney::cast( pObj );
	if( pChim == 0 )
	{
		actrTransactionManager->abortTransaction();
		return;
	}
	pChim->recordGraphicsModified(true);
	actrTransactionManager->endTransaction();

}

void DrawCmd::Merging(AcDbObjectId sourceId,AcDbObjectId mergeId)
{

	Chimney *sourceChim = getChimObj(sourceId);
	Chimney *mergeChim = getChimObj(mergeId);

	AcGePoint3dArray sourcePts = sourceChim->getControlPoint();
	AcGePoint3dArray mergePts = mergeChim->getControlPoint();

	int sourcePtsNum = sourcePts.length();
	int mergePtsNum = mergePts.length();

	AcGePoint3dArray findedPts;

	//首首相连
	if (sourcePts[0] == mergePts[0])
	{
		AcGePoint3dArray newPts;
		for (int i = sourcePtsNum-1; i >=0 ; i--)
		{
			newPts.append(sourcePts[i]);
		}
		for (int i = 0; i < mergePtsNum; i++)
		{
			newPts.append(mergePts[i]);
		}
		sourceChim->setControlPoint(newPts);
		ArxEntityHelper::EraseObject(mergeId,true);
		UpdateChimney(sourceId);
	}

	//第一段的首节点和第二段的末节点
	else if (sourcePts[0] == mergePts[mergePtsNum-1])
	{
		for (int i = 0; i < sourcePtsNum; i++)
		{
			mergePts.append(sourcePts[i]);
		}
		mergeChim->setControlPoint(mergePts);
		ArxEntityHelper::EraseObject(sourceId,true);
		UpdateChimney(mergeId);

	}
	//第二段的末节点和第一段的首节点
	else if(sourcePts[sourcePtsNum-1] == mergePts[0])
	{
		for (int i = 0; i < mergePtsNum; i++)
		{
			sourcePts.append(mergePts[i]);
		}

		sourceChim->setControlPoint(sourcePts);
		ArxEntityHelper::EraseObject(mergeId,true);
		UpdateChimney(sourceId);

	}

	//尾尾相连
	else if (sourcePts[sourcePtsNum-1] == mergePts[mergePtsNum-1])
	{
		for (int i = mergePtsNum-1; i >=0 ; i--)
		{
			sourcePts.append(mergePts[i]);
		}
		sourceChim->setControlPoint(sourcePts);
		ArxEntityHelper::EraseObject(mergeId,true);
		UpdateChimney(sourceId);
	}

	else
	{
		//AfxMessageBox(_T("不是首尾相连的巷道不能合并!"));
		return;
	}

}

void DrawCmd::MergeChimney()
{
	AcDbObjectId sourceObjId = ArxUtilHelper::SelectObject( _T( "请选择需要合并的第一段风筒:" ) );
	if( !ArxUtilHelper::IsEqualType( _T( "Chimney" ), sourceObjId ) ) return;

	AcDbObjectId mergeObjId = ArxUtilHelper::SelectObject( _T( "请选择需要合并的第二段风筒:" ) );
	if( !ArxUtilHelper::IsEqualType( _T( "Chimney" ), mergeObjId ) ) return;

	if (sourceObjId == mergeObjId)
	{
		AfxMessageBox(_T("不能选择相同的风筒合并!"),MB_OK | MB_ICONSTOP );
		return;
	}
	Merging(sourceObjId,mergeObjId);
}

double DrawCmd::ControlDirByMethods(AcDbObjectId objId, double angle)
{
	CString name;
	CString method;

	 bool isOppositeDir = false;
	DataHelper::GetPropertyData( objId, _T( "名称" ), name );
	if (ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId ))
	{
		DataHelper::GetPropertyData( objId, _T( "通风方法" ), method );
	}
	else
	{
		AcDbObjectId tWorkId = DrawHelper::GetRelatedTW(objId);		
		DataHelper::GetPropertyData( tWorkId, _T( "通风方法" ), method );
	}

	if ( _T("进风巷") != name && _T("回风巷") != name)
	{
		if (_T("压入式") == method || _T("长压短抽") == method)
		{
			isOppositeDir = true;
		}
	}

	if (true == isOppositeDir)
	{
		angle = PI + angle;
	}

	return angle;
}

void DrawCmd::DeleteDirection()
{
	AcDbObjectIdArray objIds;
	DrawHelper::FindMineGEs(_T("WindDirection"),objIds);
	for(int i = 0; i < objIds.length(); i++)
	{
		ArxEntityHelper::EraseObject( objIds[i], true );
	}
}

void DrawCmd::DeleteAllGasFlow()
{
	AcDbObjectIdArray objIds;
	DrawHelper::FindMineGEs(_T("GasFlowTagGE"),objIds);
	for(int i = 0; i < objIds.length(); i++)
	{
		ArxEntityHelper::EraseObject( objIds[i], true );
	}
}

void DrawCmd::DeleteFanTag()
{
	AcDbObjectIdArray objIds;
	DrawHelper::FindMineGEs(_T("FanTagGE"),objIds);
	for(int i = 0; i < objIds.length(); i++)
	{
		ArxEntityHelper::EraseObject( objIds[i], true );
	}

}

void DrawCmd::DeleteQTag()
{
	AcDbObjectIdArray objIds;
	DrawHelper::FindMineGEs(_T("QTagGE"),objIds);
	for(int i = 0; i < objIds.length(); i++)
	{
		ArxEntityHelper::EraseObject( objIds[i], true );
	}

}

void DrawCmd::DeleteEDTag()
{
	AcDbObjectIdArray objIds;
	DrawHelper::FindMineGEs(_T("EffectRanTagGE"),objIds);
	for(int i = 0; i < objIds.length(); i++)
	{
		ArxEntityHelper::EraseObject( objIds[i], true );
	}

}

#include "../DefGE/Joint.h"
void DrawCmd::testHD()
{
	AcDbObjectIdArray objIds;
	ArxUtilHelper::GetPickSetEntity( objIds );
	if( objIds.length() != 1 ) return;
	for(int i = 0; i < objIds.length();i++)
	{
		AcTransaction *pTran = actrTransactionManager->startTransaction();
		if(0 == pTran) return;
		AcDbObject *pObjt;
		if(Acad::eOk != pTran->getObject(pObjt,objIds[i],AcDb::kForWrite)) return;
		Test *test = Test::cast(pObjt);
		actrTransactionManager->endTransaction();

		if( !ArxUtilHelper::PostToModelSpace( test ) )
		{
			// 添加到数据库失败
			delete test;
		}

	}
	//Tunnel *pTunnel = Tunnel::cast()

}