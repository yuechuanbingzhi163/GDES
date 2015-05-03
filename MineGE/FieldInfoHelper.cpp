#include "StdAfx.h"
#include "FieldInfoHelper.h"

#include "config.h"
#include "FieldInfoObject.h"

#include "../ArxHelper/HelperClass.h"

static CString MakeFieldInfoKey( const CString& type, const CString& field )
{
    CString key;
    key.Format( _T( "%s_%s" ), type, field );
    return key;
}

void FieldInfoHelper::ReadFieldInfo( const CString& type, const CString& field, FieldInfo& info )
{
    // 读取字段信息数据
    ArxDictTool2* pDictTool = ArxDictTool2::GetDictTool( PROPERTY_DATA_FIELD_INFO_DICT );
    AcDbObjectId objId;
    bool ret = pDictTool->findEntry( MakeFieldInfoKey( type, field ), objId );
    delete pDictTool;

    if( !ret ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk == pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        FieldInfoObject* pInfoObject = FieldInfoObject::cast( pObj );
        info.copyFrom( pInfoObject->m_info );
    }
    actrTransactionManager->endTransaction();
}

void FieldInfoHelper::WriteFieldInfo( const CString& type, const CString& field, const FieldInfo& info )
{
    // 读取字段信息数据
    ArxDictTool2* pDictTool = ArxDictTool2::GetDictTool( PROPERTY_DATA_FIELD_INFO_DICT );
    AcDbObjectId objId;
    bool ret = pDictTool->findEntry( MakeFieldInfoKey( type, field ), objId );
    delete pDictTool;

    if( !ret ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk == pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        FieldInfoObject* pInfoObject = FieldInfoObject::cast( pObj );
        pInfoObject->m_info.copyFrom( info );
    }
    actrTransactionManager->endTransaction();
}

void FieldInfoHelper::RemoveFieldInfo( const CString& type, const CString& field )
{
    ArxDictTool2* pDictTool2 = ArxDictTool2::GetDictTool( PROPERTY_DATA_FIELD_INFO_DICT );
    pDictTool2->removeEntry( MakeFieldInfoKey( type, field ) );
    delete pDictTool2;
}

bool FieldInfoHelper::FindFieldInfo( const CString& type, const CString& field )
{
    ArxDictTool2* pDictTool = ArxDictTool2::GetDictTool( PROPERTY_DATA_FIELD_INFO_DICT );
    AcDbObjectId objId;
    bool ret = pDictTool->findEntry( MakeFieldInfoKey( type, field ), objId );
    delete pDictTool;
    return ret;
}

void FieldInfoHelper::AddFieldInfo( const CString& type, const CString& field, const FieldInfo& info )
{
    ArxDictTool2* pDictTool = ArxDictTool2::GetDictTool( PROPERTY_DATA_FIELD_INFO_DICT );
    FieldInfoObject* pInfoObj = new FieldInfoObject();
    pInfoObj->m_info.copyFrom( info );
    if( !pDictTool->addEntry( MakeFieldInfoKey( type, field ), pInfoObj ) ) delete pInfoObj;
    delete pDictTool;
}