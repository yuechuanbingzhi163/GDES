#include "StdAfx.h"
#include "ArxEntityHelper.h"

/* 全局函数(实现在ZoomHelper.cpp中) */
extern void ExecuteZoomCommand1( const AcGePoint3d& minPt, const AcGePoint3d& maxPt );
extern void ExecuteZoomCommand2( const AcGePoint3d& minPt, const AcGePoint3d& maxPt );
//extern void ZommWindowUseCom(const AcGePoint3d& minPt, const AcGePoint3d& maxPt);
extern void ZoomWindowUseView( const AcGePoint3d& minPt, const AcGePoint3d& maxPt );
// 无效
extern void ZoomWindowUseGSView( const AcGePoint3d& minPt, const AcGePoint3d& maxPt );


void ArxEntityHelper::EraseObject( const AcDbObjectId& objId, Adesk::Boolean erasing )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk == pTrans->getObject( pObj, objId, AcDb::kForWrite, !erasing ) )
    {
        pObj->erase( erasing ); // (反)删除图元
    }
    actrTransactionManager->endTransaction();
}

void ArxEntityHelper::EraseObject2( const AcDbObjectId& objId, Adesk::Boolean erasing )
{
    AcDbObject* pObj;
    if( Acad::eOk == acdbOpenAcDbObject( pObj, objId, AcDb::kForWrite, !erasing ) )
    {
        pObj->erase( erasing );
        pObj->close();
        //acutPrintf(_T("\n使用Open/close机制删除对象成功"));
    }
}

void ArxEntityHelper::EraseObjects( const AcDbObjectIdArray& objIds, Adesk::Boolean erasing )
{
    if( objIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite, !erasing ) ) continue;
        pObj->erase( erasing ); // (反)删除图元
    }
    actrTransactionManager->endTransaction();
}

void ArxEntityHelper::EraseObjects2( const AcDbObjectIdArray& objIds, Adesk::Boolean erasing )
{
    if( objIds.isEmpty() ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        EraseObject2( objIds[i], erasing );
    }
}

void ArxEntityHelper::TransformEntities2( const AcDbObjectIdArray& objIds, const AcGeMatrix3d& xform )
{
    if( objIds.isEmpty() ) return;

	int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
		AcDbEntity* pEnt;
		if( Acad::eOk == acdbOpenAcDbEntity( pEnt, objIds[i], AcDb::kForWrite ) )
		{
			pEnt->transformBy( xform );
			pEnt->close();
		}
    }
}

void ArxEntityHelper::TransformEntities( const AcDbObjectIdArray& objIds, const AcGeMatrix3d& xform )
{
	if( objIds.isEmpty() ) return;

	AcTransaction* pTrans = actrTransactionManager->startTransaction();
	if( pTrans == 0 ) return;

	int len = objIds.length();
	for( int i = 0; i < len; i++ )
	{
		AcDbObject* pObj;
		if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

		AcDbEntity* pEnt = AcDbEntity::cast( pObj );
		if( pEnt == 0 ) continue;

		pEnt->transformBy( xform );
	}
	actrTransactionManager->endTransaction();
}

void ArxEntityHelper::DrawEntities( const AcDbObjectIdArray& objIds, AcGiWorldDraw* mode )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        AcDbEntity* pEnt = AcDbEntity::cast( pObj );
        if( pEnt == 0 ) continue;

        //acutPrintf(_T("\n绘制第%d个图元"), i);
        pEnt->worldDraw( mode );
    }

    actrTransactionManager->endTransaction();
}

void ArxEntityHelper::ShowEntities( const AcDbObjectIdArray& objIds, bool isVisible )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;;

        AcDbEntity* pEnt = AcDbEntity::cast( pObj );
        if( pEnt == 0 ) continue;

        pEnt->setVisibility( isVisible ? AcDb::kVisible : AcDb::kInvisible ); // 显示或隐藏图元(修改)
    }

    actrTransactionManager->endTransaction();
}

void ArxEntityHelper::ShowEntity( const AcDbObjectId& objId, bool isVisible )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    AcDbEntity* pEnt = AcDbEntity::cast( pObj );
    if( pEnt == 0 )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    pEnt->setVisibility( isVisible ? AcDb::kVisible : AcDb::kInvisible ); // 显示或隐藏图元(修改)

    actrTransactionManager->endTransaction();
}

// 使用事务
static void UpdateEntity1( const AcDbObjectId& objId )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) ) // 打开图元失败
    {
        actrTransactionManager->abortTransaction();
        return;
    }
    AcDbEntity* pEnt = AcDbEntity::cast( pObj );
    if( pEnt == 0 )
    {
        actrTransactionManager->abortTransaction();
        return;
    }
    pEnt->recordGraphicsModified( true ); // 标签图元状态已修改，需要更新图形
    actrTransactionManager->endTransaction();
}

// 使用常规的Open/Close机制
static void UpdateEntity2( const AcDbObjectId& objId )
{
    AcDbEntity* pEnt;
    if( Acad::eOk != acdbOpenObject( pEnt, objId, AcDb::kForWrite ) ) return;

    pEnt->recordGraphicsModified( true ); // 标签图元状态已修改，需要更新图形
    pEnt->close();
}

void ArxEntityHelper::UpdateEntity( const AcDbObjectId& objId )
{
    if( objId.isNull() ) return;
    /*
     * 使用常规的Open/Close机制更新实体
     * 注：使用事务机制更新实体有时候好使, 有时候不好使
     *      原因暂时不明
     */
    UpdateEntity2( objId );
}

static void ForceUpdate()
{
    actrTransactionManager->queueForGraphicsFlush();
    actrTransactionManager->flushGraphics();
    acedUpdateDisplay();
}

static void ZoomEntity_Helper( const AcGePoint3d& minPt, const AcGePoint3d& maxPt )
{
    //acutPrintf(_T("\n最小点:(%.3f,%.3f,%.3f)\t最大点:(%.3f,%.3f,%.3f)\n"), minPt.x, minPt.y, minPt.z, maxPt.x, maxPt.y, maxPt.z);

    AcGeVector3d v = maxPt - minPt;

    // 沿着对角线由内向外放大(最小点变小，最大点变大)
    double c = 0.618; // 黄金比例值

    // 1) 使用sendStringToExecute方法
    // 缺点:在命令行显示一些提示字符串，有点烦人(例如: "命令：zoom w 指定第一个位置 ... ")
    //ExecuteZoomCommand1(minPt-c*v, maxPt+c*v);

    // 2) 使用acedCommand方法
    // 缺点: 在modelss dialog中无法使用(具体的说是在application context下无法执行)
    // 参见：arxdoc.chm->Advanced Topics->The Multiple Document Interface
    //        ->Application Execution Context
    //        ->Code Differences under the Application Execution Context
    //ExecuteZoomCommand2(minPt-c*v, maxPt+c*v);

    // 3) 使用com
    // 缺点: 显示一些空的命令提示(例如"命令: ")
    //ZommWindowUseCom(minPt-c*v, maxPt+c*v);

    // 4) 使用view
    // 缺点：如果在缩放的时候，对图元进行了修改，例如修改颜色，并不会马上更新
    //        只有当前焦点在cad的绘图窗口时，才会更新
    // 参见：<<ObjectARX开发实例教程-20090826>>中的"4.4 视图"小节
    ZoomWindowUseView( minPt - c * v, maxPt + c * v );
    acedGetAcadFrame()->SetFocus(); // 切换焦点(解决图形修改的问题)

    // 5) 使用AcGsView(失败)
    // AcGsView只能用于3d模式
    //ZoomWindowUseGSView(minPt-c*v, maxPt+c*v);
}

static bool IsValidExtent( const AcDbExtents& ext )
{
    AcGeVector3d v = ext.maxPoint() - ext.minPoint();
    //acutPrintf(_T("\nextents:x=%.3f,%y=%.3f,z=%.3f\n"), v.x, v.y, v.z);
    return ( v.x >= 0 && v.y >= 0 && v.z >= 0 ); // x,y,z的差值必须>=0
}

void ArxEntityHelper::ZoomToEntityForModeless( const AcDbObjectId& objId )
{
	// 在非模态对话框下无法使用
	//ads_name en;
	//if(Acad::eOk != acdbGetAdsName(en, objId)) return;
	//acedCommand(RTSTR, _T("ZOOM"), RTSTR, _T("O"), RTENAME, en, RTSTR, _T(""), 0);

	// 临时使用sendStringToExecute解决缩放定位问题
	CString cmd;
	cmd.Format( _T( "ZOOM O \003" ) ); // 按空格结束选择对象，然后esc(防止多余的空格重复执行命令)
	acDocManager->sendStringToExecute( curDoc(), cmd, true, false, false );
}

void ArxEntityHelper::ZoomToEntity( const AcDbObjectId& objId )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    AcDbEntity* pEnt = AcDbEntity::cast( pObj );
    if( pEnt == 0 )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    AcDbExtents extents;
    bool ret = ( Acad::eOk == pEnt->getGeomExtents( extents ) );

    actrTransactionManager->endTransaction();

    if( ret && IsValidExtent( extents ) )
    {
        ZoomEntity_Helper( extents.minPoint(), extents.maxPoint() );
    }
}

void ArxEntityHelper::ZoomToEntities( const AcDbObjectIdArray& objIds )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbExtents exts;
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        AcDbEntity* pEnt = AcDbEntity::cast( pObj );
        if( pEnt == 0 ) continue;

        AcDbExtents extents;
        if( Acad::eOk != pEnt->getGeomExtents( extents ) ) continue;

        exts.addPoint( extents.minPoint() );
        exts.addPoint( extents.maxPoint() );
    }

    actrTransactionManager->endTransaction();

    AcGePoint3d minPt = exts.minPoint();
    AcGePoint3d maxPt = exts.maxPoint();
    if( minPt.x <= maxPt.x && minPt.y <= maxPt.y && minPt.z <= maxPt.z )
    {
        ZoomEntity_Helper( minPt, maxPt );
    }
}

bool ArxEntityHelper::GetEntityColor( const AcDbObjectId& objId, Adesk::UInt16& colorIndex )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    AcDbEntity* pEnt = AcDbEntity::cast( pObj );
    if( pEnt == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    colorIndex = pEnt->colorIndex();
    actrTransactionManager->endTransaction();

    return true;
}

bool ArxEntityHelper::GetEntitiesColor( const AcDbObjectIdArray& objIds, AcArray<Adesk::UInt16>& colors )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    bool ret = true;  // 默认返回true
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) )
        {
            actrTransactionManager->abortTransaction();
            ret = false;
            colors.removeAll(); // 清空
            break;
        }
        AcDbEntity* pEnt = AcDbEntity::cast( pObj );
        if( pEnt == 0 )
        {
            actrTransactionManager->abortTransaction();
            ret = false;
            colors.removeAll(); // 清空
            break;
        }
        Adesk::UInt16 ci = pEnt->colorIndex();
        colors.append( ci ); // 记录原有的颜色
    }
    actrTransactionManager->endTransaction();
    return ret;
}

bool ArxEntityHelper::SetEntityColor( const AcDbObjectId& objId, unsigned short colorIndex )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    AcDbEntity* pGE = AcDbEntity::cast( pObj );
    if( pGE == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    // 设置颜色
    pGE->setColorIndex( colorIndex );
    actrTransactionManager->endTransaction();
    return true;
}

bool ArxEntityHelper::SetEntitiesColor( AcDbObjectIdArray& objIds, unsigned short colorIndex )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    bool ret = true; // 默认返回true
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) )
        {
            actrTransactionManager->abortTransaction();
            ret = false;
            break;
        }

        AcDbEntity* pGE = AcDbEntity::cast( pObj );
        if( pGE == 0 )
        {
            actrTransactionManager->abortTransaction();
            ret = false;
            break;
        }

        // 设置颜色
        pGE->setColorIndex( colorIndex );
    }
    actrTransactionManager->endTransaction();

    return ret;
}

bool ArxEntityHelper::SetEntitiesColor2( const AcDbObjectIdArray& objIds, const AcArray<Adesk::UInt16>& colors )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    bool ret = true; // 默认返回true
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) )
        {
            actrTransactionManager->abortTransaction();
            ret = false;
            break;
        }
        AcDbEntity* pEnt = AcDbEntity::cast( pObj );
        if( pEnt == 0 )
        {
            actrTransactionManager->abortTransaction();
            ret = false;
            break;
        }
        pEnt->setColorIndex( colors[i] ); // 恢复原先的颜色
    }
    actrTransactionManager->endTransaction();
    return ret;
}


/*
 * 帮助文档中acedSSSetFirst以及acedSSGetFirst
 * 要求使用它的命令应该开启ACRX_CMD_USEPICKSET和ACRX_CMD_REDRAW选项
 * 但测试结果显示，貌似不使用也可以??????
 */
 bool ArxEntityHelper::SelectEntity( const AcDbObjectId& objId )
{
    //acedSSSetFirst(NULL, NULL);

    if( objId.isNull() ) return false;

    ads_name ename;
    if( Acad::eOk != acdbGetAdsName( ename, objId ) ) return false;;

    ads_name ss;
    if( RTNORM != acedSSAdd( ename, NULL, ss ) ) return false; // 创建选择集

    acedSSSetFirst( ss, NULL ); // 高亮选中
    acedSSFree( ss );           // 释放选择集

    return true;
}

 bool ArxEntityHelper::SelectEntities( const AcDbObjectIdArray& objIds )
{
	//acedSSSetFirst(NULL, NULL);

	if( objIds.isEmpty() ) return false;

	ads_name ss;
	//创建一个空的选择集
	if(RTNORM != acedSSAdd( NULL, NULL, ss )) return false;
	bool ret = true;
	for(int i=0;i<objIds.length();i++)
	{
		ads_name ename;
		if( Acad::eOk != acdbGetAdsName( ename, objIds[i] ) ) 
		{ 
			ret = false;;
			break;
		}
		if( RTNORM != acedSSAdd( ename, ss, ss ) )  // 添加到选择集
		{
			ret = false;;
			break;
		}
	}

	if(ret)
	{
		acedSSSetFirst( ss, NULL ); // 高亮选中
	}
	acedSSFree( ss );           // 释放选择集

	return ret;
}
