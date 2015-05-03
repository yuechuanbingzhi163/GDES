#include "StdAfx.h"
#include "DataHelper.h"

#include "MineGE.h"
#include "ModelGE.h"

#include "DrawHelper.h"
#include "DataHelperImpl.h"
#include "FieldHelper.h"

#include "../ArxHelper/HelperClass.h"

static bool GetDataObject_Helper( const AcDbObjectId& objId, AcDbObjectId& dObjId )
{
    if( objId.isNull() ) return false;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    bool ret = true;
    if( pObj->isKindOf( MineGE::desc() ) )
    {
        MineGE* pGE = MineGE::cast( pObj );
        dObjId = pGE->getDataObject();
    }
    else if( pObj->isKindOf( ModelGE::desc() ) )
    {
        ModelGE* pM = ModelGE::cast( pObj );
        dObjId = pM->getDataObject();
    }
    else if( pObj->isKindOf( DataObject::desc() ) )
    {
        dObjId = objId;
    }
    else
    {
        ret = false;
    }
    actrTransactionManager->endTransaction();

    return ret;
}

static bool GetPropertyData_Helper( const AcDbObjectId& objId, const CString& fieldName, CString& value )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) ) // read状态
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    DataObject* pDO = DataObject::cast( pObj );
    if( pDO == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    DataHelperImpl dh( pDO );
    bool ret = dh.getPropertyData( fieldName, value );
    actrTransactionManager->endTransaction();

    return ret;
}

static bool SetPropertyData_Helper( const AcDbObjectId& objId, const CString& fieldName, const CString& value )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    DataObject* pDO = DataObject::cast( pObj );
    if( pDO == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    DataHelperImpl dh( pDO );
    bool ret = dh.setPropertyData( fieldName, value );
    actrTransactionManager->endTransaction();

    return ret;
}

bool DataHelper::GetDataObject( const AcDbObjectId& objId, AcDbObjectId& dObjId )
{
    // 获取数据对象
    return GetDataObject_Helper( objId, dObjId );
}

bool DataHelper::IsDataObject( const AcDbObjectId& objId )
{
    return ( !objId.isNull() &&
             ArxDataTool::IsEqualType( _T( "DataObject" ), objId, false ) );
}

bool DataHelper::GetTypeName( const AcDbObjectId& objId, CString& type )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    bool ret = true;
    if( pObj->isKindOf( MineGE::desc() ) )
    {
        MineGE* pGE = MineGE::cast( pObj );
        type = pGE->getTypeName();
    }
    else if( pObj->isKindOf( ModelGE::desc() ) )
    {
        ModelGE* pM = ModelGE::cast( pObj );
        type = pM->getTypeName();
    }
    else if( pObj->isKindOf( DataObject::desc() ) )
    {
        DataObject* pDO = DataObject::cast( pObj );
        type = pDO->getType();
    }
    else
    {
        ret = false;
    }
    actrTransactionManager->endTransaction();

    return ret;
}

bool DataHelper::GetPropertyData( const AcDbObjectId& objId, const CString& fieldName, CString& value )
{
    if( objId.isNull() ) return false;

    // 获取数据对象
    AcDbObjectId dObjId;
    if( !DataHelper::GetDataObject( objId, dObjId ) ) return false;
    return GetPropertyData_Helper( dObjId, fieldName, value );
}

bool DataHelper::SetPropertyData( const AcDbObjectId& objId, const CString& fieldName, const CString& value )
{
    if( objId.isNull() ) return false;

    // 获取数据对象
    AcDbObjectId dObjId;
    if( !DataHelper::GetDataObject( objId, dObjId ) ) return false;

    return SetPropertyData_Helper( dObjId, fieldName, value );
}

void DataHelper::CopyPropertyData( const AcDbObjectId& sourceObjId, const AcDbObjectId& targetObjId )
{
	AcDbObjectId dObjId1, dObjId2;
	if(!DataHelper::GetDataObject(sourceObjId, dObjId1)) return;
	if(!DataHelper::GetDataObject(targetObjId, dObjId2)) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj1;
    if( Acad::eOk != pTrans->getObject( pObj1, dObjId1, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }
    AcDbObject* pObj2;
    if( Acad::eOk != pTrans->getObject( pObj2, dObjId2, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

	//acutPrintf(_T("source:%s \t target:%s\n"), pObj1->isA()->name(), pObj2->isA()->name());

    DataObject* pDO1 = DataObject::cast( pObj1 );
    if( pDO1 == 0 )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    DataObject* pDO2 = DataObject::cast( pObj2 );
    if( pDO2 == 0 )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    // 类型名称是否匹配
    if( pDO1->getType() != pDO2->getType() )
    {
        actrTransactionManager->abortTransaction();
        return;
    }
     //测试用
    //acutPrintf(_T("source:%s \t target:%s\n"), pObj1->isA()->name(), pObj2->isA()->name());

    AcStringArray values;
    pDO1->getAllData( values ); // 获取所有数据
    pDO2->setAllData( values ); // 交换数据

	//AcStringArray fields;
	//FieldHelper::GetAllFields(_T("Chimney"),fields);
	//int len = fields.length();
	//for (int i = 0; i < len; i++)
	//{
	//	CString value;
	//	//CString &field = fields[i];
	//	DataHelper::GetPropertyData(sourceObjId,fields[i].kszPtr(),value);
	//	DataHelper::SetPropertyData(targetObjId,fields[i].kszPtr(),value);
	//}
    actrTransactionManager->endTransaction();
}
