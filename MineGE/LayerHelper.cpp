#include "StdAfx.h"
#include "LayerHelper.h"
//#include "GETypeNameMapHelper.h"

const static int kNameLength = 260;

////////////////////////////////////////////////////////////////////
//////////////////////网上摘录代码--图层相关/////////////////////
////////////////////////////////////////////////////////////////////

static bool getAllLayers( AcDbObjectIdArray& arrLayers )
{

    AcDbLayerTable* layerTable ;
    Acad::ErrorStatus es ;

    //open the layertable in the current drawing
    if ( Acad::eOk !=  acdbHostApplicationServices()->workingDatabase()->getLayerTable( layerTable, AcDb::kForRead ) )
    {
        acutPrintf ( _T( "\nCouldn''t get the layer table" ) ) ;
        return false;
    }

    //create a new iterator
    AcDbLayerTableIterator* pLayerTableIterator ;
    if ( Acad::eOk != layerTable->newIterator ( pLayerTableIterator ) )
    {
        acutPrintf ( _T( "\nCouldn''t get a new layer table iterator" ) ) ;
        layerTable->close () ;
        return false ;
    }
    layerTable->close () ;
    AcDbObjectId layerId ;

    //iterate through the all layers and collect their ids
    for ( ; !pLayerTableIterator->done () ; pLayerTableIterator->step() )
    {
        if ( Acad::eOk != pLayerTableIterator->getRecordId( layerId ) )
        {
            acutPrintf( _T( "\nCannot get layers" ) );
            continue;
        }
        else
        {
            arrLayers.append( layerId );
        }
    }
    delete pLayerTableIterator;

    return true;

}

// Argument : AcDbDatabase* pDb
static Acad::ErrorStatus setCurLayer( const TCHAR* lpLayerName, AcDbDatabase* pDb )
{
    AcDbDatabase* pCurDb = pDb;
    if ( pCurDb == NULL )
        pCurDb = acdbHostApplicationServices() -> workingDatabase();
    // 使用了layertable记录指针，比较方便
    AcDbLayerTableRecordPointer spRecord( lpLayerName, pCurDb, AcDb::kForRead );
    Acad::ErrorStatus es = spRecord.openStatus();
    if ( es == Acad::eOk )
    {
        es = pCurDb -> setClayer( spRecord -> objectId() );
    }
    return es;
}


static AcDbObjectId createNewLayer( const CString& LayerName )
{
    // 获得当前图形数据库的符号表
    AcDbLayerTable* pLayerTable;
    acdbHostApplicationServices()->workingDatabase()->getSymbolTable( pLayerTable, AcDb::kForWrite );
    // 生成新的图层表记录
    AcDbLayerTableRecord* pLayerTableRecord = new AcDbLayerTableRecord;
    pLayerTableRecord->setName( LayerName ); // 设置图层名
    //pLayerTableRecord->setColor(LayerColor); // 设置图层颜色
    AcDbObjectId layerId;
    pLayerTable->add( layerId, pLayerTableRecord );
    // 关闭图层表和图层表记录
    pLayerTable->close();
    pLayerTableRecord->close();
    return layerId;
}

static AcDbObjectId addLayer( const TCHAR* LayerName,
                              const Adesk::Int16 LayerColor,
                              //const char* Linetype,
                              AcDbDatabase* pDb )
{
    if( pDb == NULL )
        pDb = acdbHostApplicationServices()->workingDatabase();

    // 装载线型
    /*pDb->loadLineTypeFile(Linetype,"acadiso.lin");
    AcDbObjectId LineTypeId=AddLineType(pDb,Linetype);*/

    AcDbLayerTable* pLayerTable = NULL;
    AcDbLayerTableRecord* pLayerRecord = NULL;
    AcDbObjectId LayerId = AcDbObjectId::kNull;
    if( Acad::eOk != pDb->getLayerTable( pLayerTable, AcDb::kForRead ) ) return false;

    if( pLayerTable->has( LayerName ) )
    {
        pLayerTable->getAt( LayerName, LayerId );
    }
    else
    {
        pLayerRecord = new AcDbLayerTableRecord;
        AcCmColor color;
        color.setColorIndex( LayerColor );
        pLayerRecord->setColor( color );
        //pLayerRecord->setLinetypeObjectId(LineTypeId);
        pLayerRecord->setName( LayerName );
        pLayerTable->add( LayerId, pLayerRecord );
        pLayerRecord->close();
    }
    pLayerTable->close();
    return LayerId;
}

static bool frozenLayer( const CString& layerName, bool isFrozen )
{
    AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
    if( pDb == 0 ) return false;

    AcDbLayerTable* pLayerTable = NULL;
    if( Acad::eOk != pDb->getLayerTable( pLayerTable, AcDb::kForRead ) ) return false;

    AcDbLayerTableRecord* pLayerRecord = NULL;
    if( Acad::eOk != pLayerTable->getAt( layerName, pLayerRecord, AcDb::kForWrite ) )
    {
        pLayerTable->close();
        return false;
    }
    pLayerTable->close();

    pLayerRecord->setIsFrozen( isFrozen ); // 冻结/解冻
    pLayerRecord->close();

    return true;
}

static bool hiddenLayer( const CString& layerName, bool isOn )
{
    AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
    if( pDb == 0 ) return false;

    AcDbLayerTable* pLayerTable = NULL;
    if( Acad::eOk != pDb->getLayerTable( pLayerTable, AcDb::kForRead ) ) return false;

    AcDbLayerTableRecord* pLayerRecord = NULL;
    if( Acad::eOk != pLayerTable->getAt( layerName, pLayerRecord, AcDb::kForWrite ) )
    {
        pLayerTable->close();
        return false;
    }
    pLayerTable->close();

    pLayerRecord->setIsHidden( isOn ); // 隐藏/显示
    pLayerRecord->close();

    return true;
}

static bool lockLayer( const CString& layerName, bool locked )
{
    AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
    if( pDb == 0 ) return false;

    AcDbLayerTable* pLayerTable = NULL;
    if( Acad::eOk != pDb->getLayerTable( pLayerTable, AcDb::kForRead ) ) return false;

    AcDbLayerTableRecord* pLayerRecord = NULL;
    if( Acad::eOk != pLayerTable->getAt( layerName, pLayerRecord, AcDb::kForWrite ) )
    {
        pLayerTable->close();
        return false;
    }
    pLayerTable->close();

    pLayerRecord->setIsLocked( locked ); // 锁定/不锁定
    pLayerRecord->close();

    return true;
}

static bool offLayer( const CString& layerName, bool off )
{
    AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
    if( pDb == 0 ) return false;

    AcDbLayerTable* pLayerTable = NULL;
    if( Acad::eOk != pDb->getLayerTable( pLayerTable, AcDb::kForRead ) ) return false;

    AcDbLayerTableRecord* pLayerRecord = NULL;
    if( Acad::eOk != pLayerTable->getAt( layerName, pLayerRecord, AcDb::kForWrite ) )
    {
        pLayerTable->close();
        return false;
    }
    pLayerTable->close();

    pLayerRecord->setIsOff( off ); // 关闭/打开
    pLayerRecord->close();

    return true;
}

static void renameLayerName( const CString& layerName, const CString& newLayerName )
{
    AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
    if( pDb == 0 ) return /*false*/;

    AcDbLayerTable* pLayerTable = NULL;
    if( Acad::eOk != pDb->getLayerTable( pLayerTable, AcDb::kForRead ) ) return /*false*/;

    AcDbLayerTableRecord* pLayerRecord = NULL;
    if( Acad::eOk != pLayerTable->getAt( layerName, pLayerRecord, AcDb::kForWrite ) )
    {
        pLayerTable->close();
        return /*false*/;
    }
    pLayerTable->close();

    pLayerRecord->setName( newLayerName ); // 修改名称
    pLayerRecord->close();

    //return true;
}

void LayerHelper::SetCurrentLayer( const CString& layerName )
{
    // 图层不存在，则什么也不做，当前图层保持不变
    //if(!IsLayerExist(layerName)) return;

    setCurLayer( layerName, NULL );
}

CString LayerHelper::GetCurrentLayerName()
{
    AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();

    AcDbObject* pLayer;

    // 打开失败，则什么也不做，返回空字符串
    if ( Acad::eOk != acdbOpenObject( pLayer, layerId, AcDb::kForRead ) ) return _T( "" );

    const TCHAR* name;
    AcDbLayerTableRecord::cast( pLayer )->getName( name );
    pLayer->close();

    return name;
}

void LayerHelper::AddLayer( const CString& layerName )
{
    // 按照我的设想，还应该处理一些特殊的情况，例如
    // 字符串为空串""，字符串中包含非法字符'\t','\n',' '
    // 目前暂时不考虑这个问题

    // 图层已经存在
    if( IsLayerExist( layerName ) ) return;

    createNewLayer( layerName );
}

void LayerHelper::RemoveLayer( const CString& layerName )
{
    // 图层不存在
    if( !IsLayerExist( layerName ) ) return;

    // 图层为当前图层，不能删除
    if( IsCurrentLayer( layerName ) ) return;

    AcDbLayerTable* layerTable ;

    // 打开层表
    if ( Acad::eOk !=  acdbHostApplicationServices()->workingDatabase()->getLayerTable( layerTable, AcDb::kForWrite ) )
        return;

    // 判断层是否存在
    AcDbLayerTableRecord* pLayerTblRecord;
    // 打开层表记录失败（打开图层layerName）
    if( layerTable->getAt( layerName, pLayerTblRecord, AcDb::kForWrite ) != Acad::eOk )
    {
        layerTable->close();
        return;
    }

    // 删除图层
    pLayerTblRecord->erase( true );
    pLayerTblRecord->close();

    // 关闭层表
    layerTable->close();
}

bool LayerHelper::IsLayerExist( const CString& layerName )
{
    AcDbLayerTable* layerTable ;

    // 打开层表
    if ( Acad::eOk !=  acdbHostApplicationServices()->workingDatabase()->getLayerTable( layerTable, AcDb::kForRead ) )
        return false;

    // 判断层是否存在
    bool isExist = layerTable->has( layerName );

    // 关闭层表
    layerTable->close();

    return isExist;
}


bool LayerHelper::IsCurrentLayer( const CString& layerName )
{
    AcDbObjectId layerId = acdbHostApplicationServices()->workingDatabase()->clayer();

    AcDbObject* pLayer;

    // 打开失败，则什么也不做，返回
    if ( Acad::eOk != acdbOpenObject( pLayer, layerId, AcDb::kForRead ) ) return false;

    const TCHAR* name;
    AcDbLayerTableRecord::cast( pLayer )->getName( name );
    // 关闭图层对象
    pLayer->close();

    return ( layerName == name ); // 效率较差
}

bool LayerHelper::FrozenLayer( const CString& layerName, bool isFrozen )
{
    // 图层不存在
    if( !IsLayerExist( layerName ) ) return false;

    // 当前图层，不允许冻结
    if( IsCurrentLayer( layerName ) ) return false;

    return frozenLayer( layerName, isFrozen );
}

bool LayerHelper::HideLayer( const CString& layerName, bool isOn )
{
    // 图层不存在
    if( !IsLayerExist( layerName ) ) return false;

    // 当前图层，不允许隐藏
    if( IsCurrentLayer( layerName ) ) return false;

    return hiddenLayer( layerName, isOn );
}

bool LayerHelper::LockLayer( const CString& layerName, bool locked )
{
    // 图层不存在
    if( !IsLayerExist( layerName ) ) return false;

    // 当前图层，不允许隐藏
    if( IsCurrentLayer( layerName ) ) return false;

    return lockLayer( layerName, locked );
}

bool LayerHelper::OffLayer( const CString& layerName, bool off )
{
    // 图层不存在
    if( !IsLayerExist( layerName ) ) return false;

    // 当前图层，不允许隐藏
    if( IsCurrentLayer( layerName ) ) return false;

    return offLayer( layerName, off );
}

void LayerHelper::RenameLayer( const CString& layerName, const CString& newLayerName )
{
    // 图层名称为空不合理
    if( newLayerName.GetLength() == 0 ) return /*false*/;

    // 图层不存在
    if( !IsLayerExist( layerName ) ) return /*false*/;

    // 当前图层，不允许隐藏
    //if(IsCurrentLayer(layerName)) return false;

    renameLayerName( layerName, newLayerName );
}

//put the Ids of all the entities on a certain layer in the array, "ents"
bool LayerHelper::GetAllEntitiesOnLayer( const CString& layerName, AcDbObjectIdArray& ents )
{

    Acad::ErrorStatus es;
    //construct a resbuffer to select all the entities on a layer
    struct resbuf eb1;
    TCHAR sbuf1[kNameLength]; // Buffers to hold string
    eb1.restype = 8;  // select based on layer name
    _tcscpy( sbuf1, layerName );
    eb1.resval.rstring = sbuf1;
    eb1.rbnext = NULL; // No other properties

    ads_name ss;
    if ( RTNORM != acedSSGet( _T( "X" ), NULL, NULL, &eb1, ss ) )
        return false;
    long nEnts;

    //Iterate through the selection set, get all the entities' object Ids and
    //append them to the array, "ents"
    acedSSLength( ss, &nEnts );
    ents.setPhysicalLength( nEnts );
    for( int i = 0; i < nEnts; i++ )
    {
        ads_name eName;
        acedSSName( ss, i, eName );
        AcDbObjectId id;
        acdbGetObjectId( id, eName );
        ents.append( id );
    }
    acedSSFree( ss );

    return true;
}

void LayerHelper::DeleteAllEntitiesOnLayer( const CString& layerName )
{
    AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
    AcDbBlockTable* pBlkTbl;
    pDB->getSymbolTable( pBlkTbl, AcDb::kForRead );

    AcDbBlockTableRecord* pBlkTblRcd;
    pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();

    AcDbBlockTableRecordIterator* pBlkTblRcdItr;
    pBlkTblRcd->newIterator( pBlkTblRcdItr );
    for ( pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step() )
    {
        AcDbEntity* pEnt;
        if( Acad::eOk != pBlkTblRcdItr->getEntity( pEnt, AcDb::kForWrite ) ) continue;

        if( layerName.CompareNoCase( pEnt->layer() ) == 0 )
        {
            pEnt->erase( true );
        }
        pEnt->close();
    }
    delete pBlkTblRcdItr;

    pBlkTblRcd->close();
}