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

static void FillAttribute( const AcStringArray& names, const AcStringArray& attValues, AcDbBlockReference& bRef )
{
    if( names.isEmpty() )
    {
        acutPrintf( _T( "\n没有注册要提取的字段..." ) );
        return;
    }

    if( names.length() != attValues.length() )
    {
        acutPrintf( _T( "\n注册的属性数据与读取的数据个数不相等!" ) );
        return;
    }

    // 模仿cad的块插入过程
    // 查找所有的属性定义，并以之为模板，创建属性对象
    // 并添加到AcDbBlockReference中
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, bRef.blockTableRecord(), AcDb::kForRead ) )
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

    AcGeMatrix3d blkXform = bRef.blockTransform();

    // 遍历块中的图元，查找AcDbAttributeDefinition
    for( pIterator->start( true ); !pIterator->done(); pIterator->step( true ) )
    {
        AcDbObjectId objId;
        if( Acad::eOk != pIterator->getEntityId( objId ) ) continue;
        if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) ) continue;
        AcDbAttributeDefinition* pAttDef = AcDbAttributeDefinition::cast( pObj );
        if( pAttDef == 0 ) continue;

        // 获取标签名称
        ACHAR* pTag = pAttDef->tag();
        int pos = names.find( pTag );
        if( pos != -1 )
        {
            // 创建属性
            AcDbAttribute* pAtt = new AcDbAttribute();
            pAtt->setAttributeFromBlock( pAttDef, blkXform );

            /*
             * 属性的图层特性与属性定义保持一致，使用 setPropertiesFrom函数实现
             * 属性的可见性与属性定义一致
             * 属性的高度和角度与属性定义一致
             * 属性的插入点：属性定义的插入点与块参照插入点的矢量和
             * 属性的标记文字、提示文字与属性定义保持一致
             */
            /*
            pAtt->setPropertiesFrom(pAttDef);
            pAtt->setInvisible(pAttDef->isInvisible());
            AcGePoint3d ptBase = pAttDef->position();
            ptBase += bRef.position().asVector();
            pAtt->setPosition(ptBase);
            pAtt->setHeight(pAttDef->height());
            pAtt->setRotation(pAttDef->rotation());
            ACHAR* pPrompt = pAttDef->prompt();
            acutDelString(pPrompt);
            */

            // 设置属性数据
            pAtt->setTextString( attValues[pos].kACharPtr() );
            acutPrintf( _T( "\n添加属性%s，值:%s" ), pTag, attValues[pos].kACharPtr() );

            // 添加属性到AcDbBlockReference中
            bRef.appendAttribute( pAtt );
        }
        acutDelString( pTag );
    }
    delete pIterator;
    actrTransactionManager->endTransaction();
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
}

void BlockDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeString( m_blockName );
}

void BlockDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    m_attValues.removeAll();
    m_attValues.append( values );
}

Adesk::Boolean BlockDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    AcDbObjectId objId = GetBlockDefinitionByName( m_blockName );
    if( objId.isNull() ) return Adesk::kTrue;

    //acutPrintf(_T("\nBlockDraw::subWorldDraw()..."));
    //acutPrintf(_T("\n(%.3f, %.3f, %.3f), %.3f"), m_insertPt.x, m_insertPt.y, m_insertPt.z, m_angle);
    AcDbBlockReference bRef( m_insertPt, objId );
    bRef.setRotation( m_angle );

    // 填充属性数据
    AcStringArray names;
    regPropertyDataNames( names );
    FillAttribute( names, m_attValues, bRef );

    return bRef.worldDraw( mode );
}

Acad::ErrorStatus BlockDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_insertPt.transformBy( xform );

    // 构造一个倾角向量
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis ); // 得到原有的倾角向量

    // 执行变换
    v.transformBy( xform );

    m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    return Acad::eOk;
}

Acad::ErrorStatus BlockDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled();

    AcDbObjectId objId = GetBlockDefinitionByName( m_blockName );
    if( objId.isNull() ) return Acad::eOk;

    AcDbBlockReference bRef( m_insertPt, objId );
    bRef.setRotation( m_angle );
    return bRef.getOsnapPoints( osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds );
}

Acad::ErrorStatus BlockDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // 目前测试结果来看，块只有一个夹点：插入点
    /*
    AcDbObjectId objId = GetBlockDefinitionByName(m_blockName);
    if(objId.isNull()) return Acad::eOk;

    AcDbBlockReference bRef(m_insertPt, objId);
    bRef.setRotation(m_angle);
    //fillAttribute(bRef);
    return bRef.getGripPoints(gripPoints, osnapModes, geomIds);
    */
    gripPoints.append( m_insertPt );
    return Acad::eOk;
}

Acad::ErrorStatus BlockDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    /*AcDbObjectId objId = GetBlockDefinitionByName(m_blockName);
    if(objId.isNull()) return Acad::eOk;*/

    // 块只有一个夹点：插入点坐标
    // 且只能进行移动操作
    // 下面的代码目前是无效的
    /*AcDbBlockReference bRef(m_insertPt, objId);
    bRef.setRotation(m_angle);
    fillAttribute(bRef);
    acutPrintf(_T("\n修改前(%.3f, %.3f, %.3f), %.3f"), m_insertPt.x, m_insertPt.y, m_insertPt.z, m_angle);
    Acad::ErrorStatus es = bRef.moveGripPointsAt(indices, offset);
    if(es == Acad::eOk)
    {
    	m_insertPt = bRef.position();
    	m_angle = bRef.rotation();
    	acutPrintf(_T("\n修改后(%.3f, %.3f, %.3f), %.3f"), m_insertPt.x, m_insertPt.y, m_insertPt.z, m_angle);
    }
    return es;*/
    m_insertPt += offset;
    return Acad::eOk;
}

void BlockDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    /*AcDbObjectId objId = GetBlockDefinitionByName(m_blockName);
    if(objId.isNull()) return MineGEDraw::caclBackGroundMinPolygon(pts);

    AcDbBlockReference bRef(m_insertPt, objId);
    bRef.setRotation(m_angle);
    AcDbExtents ext;
    bRef.getGeomExtents(ext);
    AcGePoint3d pt = ext.minPoint();
    pts.append(pt);
    AcGePoint3d maxPt = ext.maxPoint();*/
    return MineGEDraw::caclBackGroundMinPolygon( pts );
}
