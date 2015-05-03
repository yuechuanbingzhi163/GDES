#include "StdAfx.h"
#include "ComGE.h"

#include "ArxDataTool.h"
#include "ArxEntityHelper.h"

#include <set>


static bool MergeExtents( const AcDbObjectIdArray& objIds, AcDbExtents& extents )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    int count = 0;
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        AcDbEntity* pEnt = AcDbEntity::cast( pObj );
        if( pEnt == 0 ) continue;

        AcDbExtents ext;
        if( Acad::eOk == pEnt->getGeomExtents( ext ) )
        {
            //extents.addExt(ext); // 合并
            extents.addPoint( ext.minPoint() );
            extents.addPoint( ext.maxPoint() );
            count++; // 记录正确的次数
        }
    }
    actrTransactionManager->endTransaction();
    return ( count != 0 );
}

static void FindAllSections( const AcDbObjectId& thisObjId, AcDbObjectIdArray& allSections )
{
    // 查找所有的ComGE
    AcDbObjectIdArray allObjIds;
    ArxDataTool::GetEntsByType( _T( "ComGE" ), allObjIds );

    // 记录所有ComGE中包含的section
    // 排除当前正在操作的ComGE图元
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = allObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        if( allObjIds[i] == thisObjId ) continue;

        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, allObjIds[i], AcDb::kForRead ) ) continue;

        ComGE* pCom = ComGE::cast( pObj );
        if( pCom == 0 ) continue;

        AcDbObjectIdArray tempObjIds;
        pCom->getAllEnts( tempObjIds );
        allSections.append( tempObjIds );
    }
    actrTransactionManager->endTransaction();
}

Adesk::UInt32 ComGE::kCurrentVersionNumber = 1 ;

// 有修改，使得ComGE成为抽象类
ACRX_NO_CONS_DEFINE_MEMBERS ( ComGE, MineGE )

ComGE::ComGE( void )
{
}

void ComGE::showEnt( const AcDbObjectId& objId, bool isVisible )
{
    assertWriteEnabled();

    ArxEntityHelper::ShowEntity( objId, isVisible );
}

bool ComGE::appendEnt( const AcDbObjectId& objId )
{
    assertWriteEnabled();

    // objId必须有效
    if( objId.isNull() ) return false;
    // 必须从AcDbEntity派生(可视化图元)
    if( !ArxDataTool::IsEqualType( _T( "AcDbEntity" ), objId ) ) return false;

    showEnt( objId, false );         // 隐藏图元
    simpleAppendEnt( objId );

    return true;
}

void ComGE::getAllEnts( AcDbObjectIdArray& objIds ) const
{
    assertReadEnabled();

    objIds.append( m_geIds );
}

void ComGE::simpleAppendEnt( const AcDbObjectId& objId )
{
    assertWriteEnabled();
    m_geIds.append( objId );
}

void ComGE::removeAllEnts()
{
    assertWriteEnabled();
    m_geIds.removeAll();
}

void ComGE::pushKeyParamToWriter( DrawParamWriter& writer ) const
{

}

void ComGE::pullKeyParamFromReader( DrawParamReader& reader )
{

}

Acad::ErrorStatus ComGE::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;

    acutPrintf( _T( "\nCompositeGE::dwgOutFields" ) );

    Acad::ErrorStatus es = MineGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    if ( ( es = pFiler->writeUInt32 ( ComGE::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    // 写入section信息
    int len = m_geIds.length();
    pFiler->writeInt32( len ); // int->long
    for( int i = 0; i < len; i++ )
    {
        pFiler->writeHardPointerId( m_geIds[i] );
    }

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus ComGE::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;

    acutPrintf( _T( "\nCompositeGE::dwgInFields" ) );

    Acad::ErrorStatus es = MineGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > ComGE::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    m_geIds.setLogicalLength( 0 );

    int len;
    AcDbHardPointerId objId;
    pFiler->readInt32( ( long* )&len );
    for( int i = 0; i < len; i++ )
    {
        pFiler->readItem( &objId );
        m_geIds.append( objId );
    }

    return ( pFiler->filerStatus () ) ;
}

Adesk::Boolean ComGE::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    //acutPrintf(_T("\n绘制ComGE,个数:%d"), m_geIds.length());
    ArxEntityHelper::DrawEntities( m_geIds, mode );

    // 必须要调用基类的subWorldDraw方法
    // 初始化数据对象
    return MineGE::subWorldDraw( mode );
}

Acad::ErrorStatus ComGE::subTransformBy( const AcGeMatrix3d& xform )
{
    ArxEntityHelper::TransformEntities( m_geIds, xform );

    return MineGE::subTransformBy( xform );
}

Acad::ErrorStatus ComGE::subErase( Adesk::Boolean erasing )
{
    Acad::ErrorStatus retCode = MineGE::subErase( erasing );
    if( Acad::eOk != retCode ) return retCode;

    AcDbObjectIdArray objIds;
    findEntsNotInOtherComGEs( m_geIds, objIds );

    ArxEntityHelper::EraseObjects2( objIds, erasing );

    return Acad::eOk;
}

void ComGE::decomposeAllEnts()
{
    assertWriteEnabled();

    AcDbObjectIdArray objIds;
    findEntsNotInOtherComGEs( m_geIds, objIds );

    // 显示所有的子图元
    ArxEntityHelper::ShowEntities( objIds, true );

    // 清空
    m_geIds.removeAll();
}

Acad::ErrorStatus ComGE::subGetGeomExtents( AcDbExtents& extents ) const
{
    assertReadEnabled () ;

    //Acad::ErrorStatus es = MineGE::subGetGeomExtents(extents);
    Acad::ErrorStatus es = Acad::eOk;
    if( es == Acad::eOk )
    {
        AcDbExtents ext;
        if( MergeExtents( m_geIds, ext ) )
        {
            //extents.addExt(ext); // 貌似无效???
            extents.addPoint( ext.minPoint() );
            extents.addPoint( ext.maxPoint() );
        }
    }
    return es;
}

void ComGE::findEntsNotInOtherComGEs( const AcDbObjectIdArray& geIds, AcDbObjectIdArray& objIds )
{
    AcDbObjectIdArray allSections;
    FindAllSections( this->objectId(), allSections );
    if( allSections.isEmpty() )
    {
        objIds.append( geIds );
        return;
    }

    // 遍历查询当前ComGE中的所有section
    // 查找不重合的部分
    int len = geIds.length();
    for( int i = 0; i < len; i++ )
    {
        if( !allSections.contains( geIds[i] ) )
        {
            objIds.append( geIds[i] );
        }
    }
}
