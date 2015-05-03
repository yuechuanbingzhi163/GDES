#include "StdAfx.h"
#include "MineGEErase_DbReactor.h"

#include "TagGE.h"
#include "ModelGE.h"
#include "DataObject.h"

#include "../ArxHelper/HelperClass.h"

MineGEErase_DbReactor::MineGEErase_DbReactor ( AcDbDatabase* pDb ) : AcDbDatabaseReactor(), mpDatabase( pDb )
{
    if ( pDb )
    {
        //acutPrintf(_T("\nMineGEErase_DbReactor : %ld"), (long)pDb);
        pDb->addReactor ( this ) ;
    }
}

MineGEErase_DbReactor::~MineGEErase_DbReactor ()
{
    Detach () ;
}

void MineGEErase_DbReactor::Attach ( AcDbDatabase* pDb )
{
    Detach () ;
    if ( mpDatabase == NULL )
    {
        if ( ( mpDatabase = pDb ) != NULL )
            pDb->addReactor ( this ) ;
    }
}

void MineGEErase_DbReactor::Detach ()
{
    if ( mpDatabase )
    {
        mpDatabase->removeReactor ( this ) ;
        mpDatabase = NULL ;
    }
}

AcDbDatabase* MineGEErase_DbReactor::Subject () const
{
    return ( mpDatabase ) ;
}

bool MineGEErase_DbReactor::IsAttached () const
{
    return ( mpDatabase != NULL ) ;
}

static void FilterTagGE( const AcDbObjectId& objId,
                         const AcDbObjectIdArray& allObjIds,
                         Adesk::Boolean pErased,
                         AcDbObjectIdArray& objIds )
{
    if( allObjIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = allObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, allObjIds[i], AcDb::kForRead, !pErased ) ) continue;

        TagGE* pTag = TagGE::cast( pObj );
        if( pTag == 0 ) continue;

        if( pTag->getRelatedGE() == objId )
        {
            objIds.append( allObjIds[i] );
        }
    }
    actrTransactionManager->endTransaction();
}

static void FilterModelGE( const AcDbObjectId& objId,
                           const AcDbObjectIdArray& allObjIds,
                           Adesk::Boolean pErased,
                           AcDbObjectIdArray& objIds )
{
    if( allObjIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = allObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, allObjIds[i], AcDb::kForRead, !pErased ) ) continue;

        ModelGE* pM = ModelGE::cast( pObj );
        if( pM == 0 ) continue;

        if( pM->getDataObject() == objId )
        {
            objIds.append( allObjIds[i] );
        }
    }
    actrTransactionManager->endTransaction();
}

static void GetEntsByType_Helper( const CString& type, Adesk::Boolean pErased, AcDbObjectIdArray& allObjIds )
{
    if( pErased )
    {
        ArxDataTool::GetEntsByType( type, allObjIds, true );
    }
    else
    {
        ArxDataTool::GetErasedEntsByType( type, allObjIds, true );
    }
}

static void GetAllTagGEById( const AcDbObjectId& objId, Adesk::Boolean pErased, AcDbObjectIdArray& objIds )
{
    //acutPrintf(_T("\n开始获取id..."));
    if( objId.isNull() ) return;

    AcDbObjectIdArray allObjIds;
    GetEntsByType_Helper( _T( "TagGE" ), pErased, allObjIds );

    // 过滤筛选出于objId关联的TagGE
    FilterTagGE( objId, allObjIds, pErased, objIds );

    //acutPrintf(_T("\n结束获取id..."));
}

static void GetAllModelGEById( const AcDbObjectId& objId, Adesk::Boolean pErased, AcDbObjectIdArray& objIds )
{
    //acutPrintf(_T("\n开始获取id..."));
    if( objId.isNull() ) return;

    AcDbObjectIdArray allObjIds;
    GetEntsByType_Helper( _T( "ModelGE" ), pErased, allObjIds );

    // 过滤筛选出于objId关联的ModelGE
    FilterModelGE( objId, allObjIds, pErased, objIds );

    //acutPrintf(_T("\n结束获取id..."));
}

static void EraseAllTagGE( const AcDbObjectId& objId, Adesk::Boolean pErased )
{
    AcDbObjectIdArray objIds;
    GetAllTagGEById( objId, pErased, objIds );
    ArxEntityHelper::EraseObjects2( objIds, pErased );
}

static void EraseAllModelGE( const AcDbObjectId& objId, Adesk::Boolean pErased )
{
    AcDbObjectIdArray objIds;
    GetAllModelGEById( objId, pErased, objIds );
    ArxEntityHelper::EraseObjects2( objIds, pErased );
}

void MineGEErase_DbReactor::objectErased( const AcDbDatabase* dwg, const AcDbObject* dbObj, Adesk::Boolean pErased )
{
    AcDbDatabaseReactor::objectErased ( dwg, dbObj, pErased );

    //acutPrintf(_T("\n开始 ==> 数据库监视：id:%ld  %s"), dbObj->objectId(), (pErased?_T("删除"):_T("反删除")));
    if( !pErased ) return;

    // 查找图元需要遍历整个cad数据库，因此效率较低
    // 因此，一定要进行判断，否者会影响到正常图元(例如删除1000条直线)的删除
    if( dbObj->isKindOf( MineGE::desc() ) )
    {
        // 删除图元关联的所有TagGE
        EraseAllTagGE( dbObj->objectId(), pErased );

        // 删除图元的数据对象关联的所有ModelGE
        MineGE* pGE = MineGE::cast( dbObj );
        EraseAllModelGE( pGE->getDataObject(), pErased );
    }
    else if( dbObj->isKindOf( ModelGE::desc() ) )
    {
        // do nothing
    }
    else if( dbObj->isKindOf( DataObject::desc() ) )
    {
        DataObject* pDO = DataObject::cast( dbObj );
        if( !pDO->getGE().isNull() )
        {
            acutPrintf( _T( "\n【警告】：目前暂不允许删除与图元关联的数据对象" ) );
        }
    }

    //acutPrintf(_T("\n结束 ==> 数据库监视：id:%ld  %s"), dbObj->objectId(), (pErased?_T("删除"):_T("反删除")));
}

// 在objectModified中无法启动事务
void MineGEErase_DbReactor::objectModified( const AcDbDatabase* dwg, const AcDbObject* dbObj )
{
    AcDbDatabaseReactor::objectModified ( dwg, dbObj );
    if( dbObj->isKindOf( DataObject::desc() ) )
    {
        DataObject* pDO = DataObject::cast( dbObj );
        ArxEntityHelper::UpdateEntity( pDO->getGE() ); // 强制更新显示效果
    }
}