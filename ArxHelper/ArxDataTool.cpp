#include "StdAfx.h"
#include "ArxDataTool.h"

void ArxDataTool::RegAppName( AcDbDatabase* db, const CString& appName )
{
    // make sure application name is registered
    AcDbRegAppTable* regAppTbl;
    Acad::ErrorStatus es = db->getSymbolTable( regAppTbl, AcDb::kForRead );
    if ( es == Acad::eOk )
    {
        if ( regAppTbl->has( appName ) == false )
        {
            es = regAppTbl->upgradeOpen();
            if ( es == Acad::eOk )
            {
                AcDbRegAppTableRecord* newRec = new AcDbRegAppTableRecord;
                es = newRec->setName( appName );
                if ( es == Acad::eOk )
                {
                    es = regAppTbl->add( newRec );
                    if ( es == Acad::eOk )
                        newRec->close();
                    else
                    {
                        ASSERT( 0 );
                        delete newRec;
                    }
                }
                else
                {
                    ASSERT( 0 );
                    delete newRec;
                }
            }
            else
            {
                ASSERT( 0 );
            }
        }
        regAppTbl->close();
    }
    else
    {
        ASSERT( 0 );
    }
}

bool ArxDataTool::GetDataFromXData( AcDbObject* pObj, const CString& appName, int index, CString& value )
{
    resbuf* pAppNode = pObj->xData( appName ); // 复制了一份扩展数据链表
    if( pAppNode == 0 ) return false;

    int i = 0;
    resbuf* pTemp;
    for( pTemp = pAppNode; pTemp != NULL; pTemp = pTemp->rbnext )
    {
        if( i == index ) break;
        i++;
    }
    value = pTemp->resval.rstring;
    acutRelRb( pAppNode );

    return true;
}

bool ArxDataTool::SetDataToXData( AcDbObject* pObj, const CString& appName, int index, const CString& value )
{
    resbuf* pAppNode = pObj->xData( appName );
    if( pAppNode == 0 ) return false;

    int i = 0;
    resbuf* pTemp;
    for( pTemp = pAppNode; pTemp != NULL; pTemp = pTemp->rbnext )
    {
        if( i == index ) break;
        i++;
    }

    acutNewString( value, pTemp->resval.rstring );

    pObj->setXData( pAppNode );
    acutRelRb( pAppNode );

    return true;
}

// 注意：xdata额外多了一个 regAppName节点
void ArxDataTool::AddDataToXData( AcDbObject* pObj, const CString& appName )
{
    // 必须保证图元处于write状态
    resbuf* pAppNode = pObj->xData( appName );
    if( pAppNode == 0 )
    {
        pAppNode = acutBuildList( AcDb::kDxfRegAppName, appName, AcDb::kDxfXdAsciiString, _T( "" ), 0 );
    }
    else
    {
        resbuf* pTemp;
        for( pTemp = pAppNode; pTemp->rbnext != NULL; pTemp = pTemp->rbnext )
        {
            ;    // 循环到末尾
        }
        pTemp->rbnext = acutBuildList( AcDb::kDxfXdAsciiString, _T( "" ), 0 ); // 增加一个字符串
    }

    pObj->setXData( pAppNode );
    acutRelRb( pAppNode );
}

// 注意：xdata额外多了一个 regAppName节点
void ArxDataTool::RemoveDataFromXData( AcDbObject* pObj, const CString& appName, int index )
{
    // 必须保证图元处于write状态
    resbuf* pAppNode = pObj->xData( appName );
    if( pAppNode == 0 ) return ;

    int i = 0;
    resbuf* pPos, *pTemp;
    for( pTemp = pAppNode; pTemp != NULL; pTemp = pTemp->rbnext )
    {
        if( i + 1 == index ) break;
        i++;
    }
    pPos = pTemp->rbnext;
    pTemp->rbnext = pPos->rbnext;

    pObj->setXData( pAppNode );
    acutRelRb( pAppNode );

    pPos->rbnext = 0;
    acutRelRb( pPos );
}

void ArxDataTool::AddDataToObjects( const AcDbObjectIdArray& objIds, const CString& appName )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds.at( i ), AcDb::kForWrite ) ) continue;
        AddDataToXData( pObj, appName );
    }
    actrTransactionManager->endTransaction();
}

void ArxDataTool::RemoveDataFromObjects( const AcDbObjectIdArray& objIds, const CString& appName, int index )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds.at( i ), AcDb::kForWrite ) ) continue;
        RemoveDataFromXData( pObj, appName, index );
    }
    actrTransactionManager->endTransaction();
}

bool ArxDataTool::IsDictExist( const CString& dictName )
{
    AcDbDictionary* pNamedobj;
    acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary( pNamedobj, AcDb::kForRead );

    bool ret = pNamedobj->has( dictName );
    pNamedobj->close();

    return ret;
}

void ArxDataTool::RegDict( const CString& dictName )
{
    // 初始化工作，建立存储词典
    AcDbDictionary* pNamedobj;
    acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary( pNamedobj, AcDb::kForWrite );

    AcDbObject* pObj;
    Acad::ErrorStatus es = pNamedobj->getAt( dictName, pObj, AcDb::kForRead );
    if( Acad::eOk ==  es )
    {
        pObj->close();
    }
    else if( Acad::eKeyNotFound == es )
    {
        AcDbDictionary* pDict = new AcDbDictionary();
        AcDbObjectId dictId;
        if( Acad::eOk != pNamedobj->setAt( dictName, pDict, dictId ) )
        {
            delete pDict;
        }
        else
        {
            pDict->close();
        }
    }
    pNamedobj->close();
}

static bool IsEqualObject( AcRxClass* pClass, AcDbObject* pObj, bool isDerivedFromParent )
{
    bool ret;
    if( !isDerivedFromParent )
    {
        ret = ( pObj->isA() == pClass ); // 只选择特定类型图元
    }
    else
    {
        ret = pObj->isKindOf( pClass ); // 同时还选择派生类
    }
    return ret;
}

// 使用open/close机制
static bool IsEqualType1( const CString& type, const AcDbObjectId& objId, bool isDerivedFromParent )
{
    AcRxClass* pClass = AcRxClass::cast( acrxClassDictionary->at( type ) );
    if( pClass == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != acdbOpenObject( pObj, objId, AcDb::kForRead ) ) return false;

    bool ret = IsEqualObject( pClass, pObj, isDerivedFromParent );
    pObj->close();

    return ret;
}

// 使用事务
static bool IsEqualType2( const CString& type, const AcDbObjectId& objId, bool isDerivedFromParent )
{
    AcRxClass* pClass = AcRxClass::cast( acrxClassDictionary->at( type ) );
    if( pClass == 0 ) return false;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 )
    {
        acutPrintf( _T( "\n判断类型:%s时获取transaction出错..." ), type );
        return false;
    }

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    bool ret = IsEqualObject( pClass, pObj, isDerivedFromParent );

    actrTransactionManager->endTransaction();

    return ret;
}

//CString ArxDataTool::GetTypeName( const AcDbObjectId& objId )
//{
//	AcDbObject* pObj;
//	if(Acad::eOk != acdbOpenObject(pObj, objId, AcDb::kForRead)) return _T("");
//
//	pObj->close();
//
//	AcRxClass* pClass = AcRxClass::cast(acrxClassDictionary->at(type));
//	if(pClass == 0) return _T("");
//	return CString(pClass->name());
//}

bool ArxDataTool::IsEqualType( const CString& type, const AcDbObjectId& objId, bool isDerivedFromParent )
{
    //return IsEqualType1(type, objId, isDerivedFromParent);
    return IsEqualType2( type, objId, isDerivedFromParent );
}

// 如果skipDeleted = true，则只查找可见的图元，即没有删除的图元
// 如果skipDeleted = false，则查找所有的图元
// 下述的用法是错误的
// iterator也提供了打开对象指针的方法，
// 如果获取id，再去用open/close机制(或者事务transaction)打开对象
// 从逻辑上就有些矛盾，也可能会导致一些莫名奇妙的错误
// 例如startTransaction返回NULL，无法启动事务
/*AcDbObjectId objId;
if(Acad::eOk != pBlkTblRcdItr->getEntityId(objId)) continue;

if(ArxDataTool::IsEqualType(type, objId, isDerivedFromParent))
{
objIds.append(objId);
}*/
static void GetEntsByType_Helper( const CString& type, AcDbObjectIdArray& objIds, bool isDerivedFromParent, bool skipDeleted )
{
    AcRxClass* pClass = AcRxClass::cast( acrxClassDictionary->at( type ) );
    if( pClass == 0 ) return;

    AcDbBlockTable* pBlkTbl;
    acdbHostApplicationServices()->workingDatabase()->getSymbolTable( pBlkTbl, AcDb::kForRead );

    AcDbBlockTableRecord* pBlkTblRcd;
    Acad::ErrorStatus es = pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();

    if( Acad::eOk != es ) return;

    AcDbBlockTableRecordIterator* pBlkTblRcdItr;
    if( Acad::eOk == pBlkTblRcd->newIterator( pBlkTblRcdItr, true, skipDeleted ) )
    {
        for ( pBlkTblRcdItr->start( true, skipDeleted ); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step( true, skipDeleted ) )
        {
            AcDbEntity* pEnt;
            if( Acad::eOk != pBlkTblRcdItr->getEntity( pEnt, AcDb::kForRead ) ) continue;

            if( IsEqualObject( pClass, pEnt, isDerivedFromParent ) )
            {
                objIds.append( pEnt->objectId() );
            }
            pEnt->close();
        }
        delete pBlkTblRcdItr;
    }
    pBlkTblRcd->close();
}

void ArxDataTool::GetEntsByType( const CString& type, AcDbObjectIdArray& objIds, bool isDerivedFromParent )
{
    GetEntsByType_Helper( type, objIds, isDerivedFromParent, true );
}

void ArxDataTool::GetErasedEntsByType( const CString& type, AcDbObjectIdArray& objIds, bool isDerivedFromParent )
{
    AcDbObjectIdArray allObjIds;
    GetEntsByType_Helper( type, allObjIds, isDerivedFromParent, false );
    if( allObjIds.isEmpty() ) return;
    //acutPrintf(_T("\n所有图元个数(包括已删除图元):%d"), allObjIds.length());

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = allObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, allObjIds[i], AcDb::kForRead ) ) continue;

        if( pObj->isErased() )
        {
            objIds.append( allObjIds[i] ); // 只添加删除了的图元
        }
    }
    actrTransactionManager->endTransaction();
}

void ArxDataTool::FilterEntsByType( const CString& type, const AcDbObjectIdArray& allObjIds, AcDbObjectIdArray& objIds )
{
    int n = allObjIds.length();
    for( int i = 0; i < n; i++ )
    {
        if( IsEqualType( type, allObjIds[i] ) )
        {
            objIds.append( allObjIds[i] );
        }
    }
}