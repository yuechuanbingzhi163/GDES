#include "StdAfx.h"
#include "BlockDraw.h"
#include "BlockDraw_ConfigDlg.h"

// 根据块名称获取块定义id
static AcDbObjectId GetBlockDefinitionByName( const CString& blockName )
{
    AcDbBlockTable* pBlockTable;
    acdbHostApplicationServices()->workingDatabase()->getSymbolTable( pBlockTable, AcDb::kForRead );

    AcDbBlockTableRecord* pBlockTableRecord;
    Acad::ErrorStatus es = pBlockTable->getAt( blockName, pBlockTableRecord, AcDb::kForRead );
    pBlockTable->close();

    AcDbObjectId objId;
    if( es == Acad::eOk )
    {
        objId = pBlockTableRecord->objectId();
        pBlockTableRecord->close();
    }
    return objId;
}

static AcGeMatrix3d GetBlockTransformMatrix( const AcDbObjectId& blockId, const AcGePoint3d& insertPt, double angle, double scale )
{
    AcDbBlockReference bRef( insertPt, blockId );
    bRef.setRotation( angle );
    bRef.setScaleFactors( scale );
    return bRef.blockTransform();
}

static void BlockToEntity( const AcDbObjectId& blkId, const AcGeMatrix3d& blkXform,
                           const AcStringArray& names, const AcStringArray& attValues,
                           AcGeVoidPointerArray& ents )
{
    // 将块定义分解成独立的图元
    // 将属性替换成多行文字
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, blkId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    AcDbBlockTableRecord* pBTR = AcDbBlockTableRecord::cast( pObj );
    // BUG：不能调用hasAttributeDefinitions()方法
    // 调用之后，如果没有在块编辑器中对块进行修改，
    // 那么进行移动、夹点编辑等操作，没有动态显示效果
    //if(!pBTR->hasAttributeDefinitions())
    //{
    //	// 没有属性定义
    //	acutPrintf(_T("\n没有属性定义"));
    //	actrTransactionManager->abortTransaction();
    //	return;
    //}

    AcDbBlockTableRecordIterator* pIterator;
    if( Acad::eOk != pBTR->newIterator( pIterator ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    // 遍历块中的图元，查找AcDbAttributeDefinition
    for( pIterator->start( true ); !pIterator->done(); pIterator->step( true ) )
    {
        AcDbObjectId objId;
        if( Acad::eOk != pIterator->getEntityId( objId ) ) continue;
        if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) ) continue;

        AcDbEntity* pEnt = AcDbEntity::cast( pObj );
        if( !pEnt->isKindOf( AcDbAttributeDefinition::desc() ) )
        {
            AcDbEntity* pClone = AcDbEntity::cast( pEnt->clone() );
            pClone->transformBy( blkXform );
            // 添加到实体集合
            ents.append( pClone );
        }
        else
        {
            AcDbAttributeDefinition* pAttDef = AcDbAttributeDefinition::cast( pEnt );
            pAttDef->convertIntoMTextAttributeDefinition( Adesk::kTrue );

            // 获取标签名称
            ACHAR* pTag = pAttDef->tag();
            int pos = names.find( pTag );
            if( pos != -1 )
            {
                // 获取多行文本对象
                AcDbMText* pMText = pAttDef->getMTextAttributeDefinition();
                pMText->transformBy( blkXform );
                pMText->setContents( attValues[pos].kACharPtr() );
                // 添加到实体集合
                ents.append( pMText );
            }
            acutDelString( pTag );
        }
    }
    delete pIterator;
    actrTransactionManager->endTransaction();
}

static void DeleteEntities( AcGeVoidPointerArray& ents )
{
    int n = ents.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbEntity* pEnt = ( AcDbEntity* )ents[i];
        delete pEnt;
    }
}

static void DrawBlockEnts( AcGiWorldDraw* mode, AcGeVoidPointerArray& ents )
{
    int n = ents.length();
    for( int i = 0; i < n; i++ )
    {
        AcDbEntity* pEnt = ( AcDbEntity* )ents[i];
        pEnt->worldDraw( mode );
    }
}

ACRX_NO_CONS_DEFINE_MEMBERS( BlockDraw, MineGEDraw )

BlockDraw::BlockDraw () : MineGEDraw ()
{
}

BlockDraw::~BlockDraw ()
{
}

void BlockDraw::setAllExtraParamsToDefault()
{
    m_scale = 1.0;
}

void BlockDraw::configExtraParams()
{
    // 切换资源
    CAcModuleResourceOverride myResources;

    // 显示已有的块定义列表, 让用户选择块名称
    // 从而修改可视化效果
    BlockDraw_ConfigDlg dlg;
    dlg.m_blockName = m_blockName;
    if( IDOK == dlg.DoModal() )
    {
        m_blockName = dlg.m_blockName;
    }
}

void BlockDraw::updateExtraParams()
{

}

void BlockDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

void BlockDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void BlockDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readString( m_blockName );
    reader.readDouble( m_scale );
}

void BlockDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeString( m_blockName );
    writer.writeDouble( m_scale );
}

void BlockDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    m_attValues.removeAll();
    m_attValues.append( values );
}

void BlockDraw::explodeBlock( AcGeVoidPointerArray& ents )
{
    // 填充属性数据
    AcStringArray names;
    regPropertyDataNames( names );
    if( names.isEmpty() )
    {
        acutPrintf( _T( "\n没有注册要提取的字段..." ) );
        return;
    }

    if( names.length() != m_attValues.length() )
    {
        acutPrintf( _T( "\n注册的属性数据与读取的数据个数不相等!" ) );
        return;
    }

    AcDbObjectId blkId = GetBlockDefinitionByName( m_blockName );
    if( blkId.isNull() )
    {
        acutPrintf( _T( "\n未知的块定义:%s" ), m_blockName );
        return;
    }

    // 计算变换矩阵
    AcGeMatrix3d blkXform = GetBlockTransformMatrix( blkId, m_insertPt, m_angle, m_scale );

    // 分解块定义
    BlockToEntity( blkId, blkXform, names, m_attValues, ents );
}

Adesk::Boolean BlockDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    // 分解块并填充数据
    AcGeVoidPointerArray ents;
    explodeBlock( ents );
    // 绘制图形
    DrawBlockEnts( mode, ents );

    // 删除图形实体指针
    DeleteEntities( ents );

    return Acad::eOk;
}

Acad::ErrorStatus BlockDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_insertPt.transformBy( xform );
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis );
    v.transformBy( xform );

    m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
    m_scale = v.length();

    return Acad::eOk;
}

Acad::ErrorStatus BlockDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    Adesk::GsMarker gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled();

    snapPoints.append( m_insertPt );

    return Acad::eOk;
}

Acad::ErrorStatus BlockDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // 从测试结果来看，块只有一个夹点：插入点
    gripPoints.append( m_insertPt );

    return Acad::eOk;
}

Acad::ErrorStatus BlockDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    m_insertPt += offset;

    return Acad::eOk;
}

void BlockDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    return MineGEDraw::caclBackGroundMinPolygon( pts );
}
