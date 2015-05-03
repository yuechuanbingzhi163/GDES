#include "StdAfx.h"
#include "FieldHelper.h"

#include "config.h"
#include "MineGE.h"
#include "DataObject.h"
#include "FieldInfoHelper.h"

#include "../ArxHelper/HelperClass.h"

static bool IsEmptyString( const CString& str )
{
    return ( str.GetLength() == 0 );
}

static bool FindKey_Helper( const CString& dictName, const CString& type )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    bool ret = pDictTool->findKey( type );
    delete pDictTool;
    return ret;
}

static void RemoveKey_Helper( const CString& dictName, const CString& type )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    pDictTool->removeKey( type );
    delete pDictTool;
}

static void GetAllKeys_Helper( const CString& dictName, AcStringArray& keys )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    pDictTool->getAllKeys( keys );
    delete pDictTool;
}

static bool AddField_Helper( const CString& dictName, const CString& type, const CString& field )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    int index = pDictTool->addEntry( type, field );
    delete pDictTool;
    return ( index != INVALID_ENTRY );
}

static int RemoveField_Helper( const CString& dictName, const CString& type, const CString& field )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    int index = pDictTool->removeEntry( type, field );
    delete pDictTool;
    return index;
}

static int CountFields_Helper( const CString& dictName, const CString& type )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    int count = pDictTool->countEntries( type );
    delete pDictTool;
    return count;
}

static int FindField_Helper( const CString& dictName, const CString& type, const CString& field )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    int index = pDictTool->findEntry( type, field );
    delete pDictTool;
    return index;
}

static void GetAllFields_Helper( const CString& dictName, const CString& type, AcStringArray& fields )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    pDictTool->getAllEntries( type, fields );
    delete pDictTool;
}

// 从图元的扩展词典中查找数据对象
static void GetDataObjectFromExtDict( AcDbObjectIdArray& dbObjIds )
{
    AcDbObjectIdArray allObjIds;
    ArxDataTool::GetEntsByType( _T( "MineGE" ), allObjIds, true );

    // 判断是否数据对象DataObject
    // 且类型名称==type
    if( allObjIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = allObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, allObjIds[i], AcDb::kForRead ) ) continue;

        MineGE* pGE = MineGE::cast( pObj );
        if( pGE == 0 ) continue;

        AcDbObjectId dbObjId = pGE->getDataObject();
        if( !dbObjId.isNull() )
        {
            dbObjIds.append( dbObjId );
        }
    }
    actrTransactionManager->endTransaction();
}

// 获取词典下的所有
static void GetDataObjectFromDict( const CString& dictName, AcDbObjectIdArray& dbObjIds )
{
    AcDbObjectIdArray allObjIds;
    ArxDictTool2* pDict = ArxDictTool2::GetDictTool( dictName );
    pDict->getAllEntries( allObjIds );
    delete pDict;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = allObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, allObjIds[i], AcDb::kForRead ) ) continue;

        if( pObj->isKindOf( DataObject::desc() ) )
        {
            dbObjIds.append( allObjIds[i] );
        }
    }
    actrTransactionManager->endTransaction();
}

// 根据类型名称查找
static void FilterDataObject( const AcDbObjectIdArray& dbObjIds, const CString& type, AcDbObjectIdArray& objIds )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = dbObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, dbObjIds[i], AcDb::kForRead ) ) continue;

        DataObject* pDO = DataObject::cast( pObj );
        if( pDO == 0 ) continue;

        if( type == pDO->getType() )
        {
            objIds.append( dbObjIds[i] );
        }
    }
    actrTransactionManager->endTransaction();
}

static void GetDataObjectsByType( const CString& type, AcDbObjectIdArray& objIds )
{
    AcDbObjectIdArray dbObjIds;

    // 在图元的扩展词典中查找
    GetDataObjectFromExtDict( dbObjIds );
    // 在OBJECT_LIST_DATA_DICT字典中查找
    GetDataObjectFromDict( OBJECT_LIST_DATA_DICT, objIds );
    // 在GLOBAL_SINGLE_INFO_DICT词典中查找
    GetDataObjectFromDict( GLOBAL_SINGLE_INFO_DICT, objIds );

    // 过滤类型不等于type的DataObject
    FilterDataObject( dbObjIds, type, objIds );
}

static void AddFieldToDataObject( const AcDbObjectIdArray& objIds )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        DataObject* pDO = DataObject::cast( pObj );
        if( pDO == 0 ) continue;

        pDO->addData(); // 在末尾增加一个数据
    }
    actrTransactionManager->endTransaction();
}

static void RemoveFieldFromDataObject( const AcDbObjectIdArray& objIds, int index )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        DataObject* pDO = DataObject::cast( pObj );
        if( pDO == 0 ) continue;

        pDO->removeData( index ); // 删除指定位置的数据
    }
    actrTransactionManager->endTransaction();
}

static void CleanAllFieldFromDataObject( const AcDbObjectIdArray& objIds )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        DataObject* pDO = DataObject::cast( pObj );
        if( pDO == 0 ) continue;

        pDO->clearAll(); // 删除所有数据
    }
    actrTransactionManager->endTransaction();
}

bool FieldHelper::AddField( const CString& type, const CString& field )
{
    if( IsEmptyString( type ) || IsEmptyString( field ) ) return false;

    bool ret = AddField_Helper( PROPERTY_DATA_FIELD_DICT, type, field );
    if( ret )
    {
        AcDbObjectIdArray objIds;
        GetDataObjectsByType( type, objIds );
        if( !objIds.isEmpty() )
        {
            AddFieldToDataObject( objIds );
        }
    }
    return ret;
}

bool FieldHelper::RemoveField( const CString& type, const CString& field )
{
    if( IsEmptyString( type ) || IsEmptyString( field ) ) return false;

    int index = RemoveField_Helper( PROPERTY_DATA_FIELD_DICT, type, field );
    bool ret = ( INVALID_ENTRY != index );
    if( ret )
    {
        // 删除字段信息
        FieldInfoHelper::RemoveFieldInfo( type, field );

        // 删除图元中的字段
        AcDbObjectIdArray objIds;
        GetDataObjectsByType( type, objIds );
        if( !objIds.isEmpty() )
        {
            RemoveFieldFromDataObject( objIds, index );
        }
    }
    return ret;
}

// 实现有问题???
void FieldHelper::RemoveAllFields( const CString& type )
{
    AcStringArray fields;
    GetAllFields( type, fields );
    int len = fields.length();
    for( int i = 0; i < len; i++ )
    {
        RemoveField( type, fields[i].kACharPtr() );
    }
    RemoveKey_Helper( PROPERTY_DATA_FIELD_DICT, type );
}

int FieldHelper::CountFields( const CString& type )
{
    return CountFields_Helper( PROPERTY_DATA_FIELD_DICT, type );
}

int FieldHelper::FindField( const CString& type, const CString& field )
{
    return FindField_Helper( PROPERTY_DATA_FIELD_DICT, type, field );
}

void FieldHelper::GetAllFields( const CString& type, AcStringArray& fields )
{
    GetAllFields_Helper( PROPERTY_DATA_FIELD_DICT, type, fields );
}

void FieldHelper::GetAllRegTypes( AcStringArray& types )
{
    // 类型包括3部分：
    //   1) arx中派生于MineGE类型的图元
    ArxClassHelper::GetAllTopParentClass( _T( "MineGE" ), types );

    //   2) arx中派生与ModelGE类型的图元
    ArxClassHelper::GetAllTopParentClass( _T( "ModelGE" ), types );

    //   3.1) 在词典OBJECT_LIST_DICT中注册的类型
    GetAllKeys_Helper( OBJECT_LIST_DICT, types );

    //   3.2) 在词典GLOBAL_SINGLE_INFO_DICT中注册的类型
    GetAllKeys_Helper( GLOBAL_SINGLE_INFO_DICT, types );
}
