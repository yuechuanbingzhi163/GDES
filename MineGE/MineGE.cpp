#include "StdAfx.h"
#include "MineGE.h"

#include "config.h"
#include "DataHelperImpl.h"
#include "CurDrawTool.h"

#include "../MineGEDraw/MineGEDraw_Jig.h"
#include "../MineGEDraw/MineGEDrawSystem.h"
#include "../ArxDbgXdata/ArxDbgXdata.h"

static bool HasDataObject( const AcDbObjectId& dictId )
{
    AcDbDictionary* pDict;
    if( Acad::eOk != acdbOpenObject( pDict, dictId, AcDb::kForWrite ) ) return false;

    AcDbObjectId objId;
    bool ret = pDict->has( DATA_OBJECT_EXT_DICT_KEY );
    pDict->close();

    return ret;
}

static AcDbObjectId CreateDataObject( const AcDbObjectId& dictId,
                                      const CString& type,
                                      const AcDbObjectId& objId )
{
    AcDbDictionary* pDict;
    if( Acad::eOk != acdbOpenObject( pDict, dictId, AcDb::kForWrite ) ) return AcDbObjectId::kNull;

    // 创建数据对象
    DataObject* pDO = new DataObject();
    // 设置图元id
    pDO->setGE( objId );
    // 记录类型名称
    pDO->setType( type );
    // 初始化数据
    pDO->initData();

    AcDbObjectId dbObjId;
    if( Acad::eOk != pDict->setAt( DATA_OBJECT_EXT_DICT_KEY, pDO, dbObjId ) )
    {
        delete pDO;
    }
    else
    {
        pDO->close();
    }
    pDict->close();

    return dbObjId;
}

static AcDbObjectId GetDataObject( const AcDbObjectId& dictId )
{
    if( dictId.isNull() ) return AcDbObjectId::kNull;

    AcDbDictionary* pDict;
    if( Acad::eOk != acdbOpenObject( pDict, dictId, AcDb::kForRead ) ) return AcDbObjectId::kNull;

    AcDbObjectId objId;
    pDict->getAt( DATA_OBJECT_EXT_DICT_KEY, objId );

    pDict->close();

    return objId;
}

// 对图元关联的所有标签图元进行变换(可见图元，不包括已删除掉的图元)
//static void TransformAllTagGE( const AcDbObjectId& objId, const AcGeMatrix3d & xform )
//{
//	AcDbObjectIdArray objIds;
//	DrawHelper::GetAllTagGEById(objId, objIds);
//	ArxEntityHelper::TransformEntities(objIds, xform);
//}

Adesk::UInt32 MineGE::kCurrentVersionNumber = 1 ;

// 有修改，使得MineGE成为抽象类
ACRX_NO_CONS_DEFINE_MEMBERS ( MineGE, AcDbEntity )

MineGE::MineGE() : m_pCurrentGEDraw( 0 )
{
    //acutPrintf(_T("\nMineGE::MineGE()..."));
}

MineGE::~MineGE ()
{
    //acutPrintf(_T("\nMineGE::~MineGE()..."));
    m_pCurrentGEDraw = 0;
}

CString MineGE::getTypeName() const
{
    // 返回类型名称
    // 使用了虚函数方法isA()
    return this->isA()->name();
}

void MineGE::initPropertyData()
{
    //assertWriteEnabled();
    if( !m_dataObjectId.isNull() ) return;

    Acad::ErrorStatus es = upgradeOpen();
    if( es == Acad::eOk || es == Acad::eWasOpenForWrite )
    {
        createExtensionDictionary();
        AcDbObjectId dictId = extensionDictionary();
        m_dataObjectId = CreateDataObject( dictId, getTypeName(), objectId() );
    }
    if( es == Acad::eOk )
    {
        downgradeOpen();
    }
}

AcDbObjectId MineGE::getDataObject() const
{
    assertReadEnabled();

    return m_dataObjectId;
}

void MineGE::initDraw()
{
    if( m_pCurrentGEDraw == 0 )
    {
        Acad::ErrorStatus es = upgradeOpen();
        if( es == Acad::eOk || es == Acad::eWasOpenForWrite )
        {
            updateCurrentDraw();
        }
        if( es == Acad::eOk )
        {
            downgradeOpen();
        }
    }
}

void MineGE::updateDrawParams( MineGEDraw* pGEDraw )
{
    if( pGEDraw != 0 )
    {
        writeKeyParamToGEDraw( pGEDraw );
        pGEDraw->updateExtraParams();
        readExtraParamFromGEDraw( pGEDraw );
    }
}

void MineGE::configDraw( const CString& drawName )
{
    MineGEDraw* pGEDraw = MineGEDrawSystem::GetInstance()->getGEDraw( getTypeName(), drawName );
    updateDrawParams( pGEDraw );
}

void MineGE::extractExistedDraw( AcStringArray& existedDraw )
{
    ArxDbgAppXdata xdata( DRAW_PARAMS_XDATA_GROUP, acdbHostApplicationServices()->workingDatabase() );
    xdata.getXdata( this );          // 提取扩展数据到xdata对象中

    if( xdata.isEmpty() ) return;    // 没有数据

    int len = 0;
    xdata.getInteger( 1, len );      // 绘制效果个数
    for( int i = 1; i <= len; i++ )
    {
        CString drawName;
        xdata.getString( 2 * i, drawName ); // 绘制效果名称
        existedDraw.append( drawName );
    }
}

// 这里的情况比较复杂
// 必须要保证xdata里的draw与全局的draw是同步的
// 可能存在如下几种情况：
// 1、xdata里的draw 等于 全局的draw
// 2、xdata里的draw 大于 全局的draw(增加了draw插件)
// 3、xdata里的draw 小于 全局的draw(删除了draw插件)
// 4、xata里的部分draw中在全局的draw中没有；全局的部分draw在xdata中没有
// 目前采用最简单的办法，不论draw存在与否，全部都合并到xdata中
// 也就是说xdata中可能存在冗余的、不再使用的draw数据
// 后期可以考虑专门编写一个清理draw的程序
void MineGE::initAllExtraParamsToXData()
{
    // 通过assertWriteEnabled限定只能在write状态下操作
    assertWriteEnabled();

    // 提取已存在的draw
    AcStringArray existedDraw;
    extractExistedDraw( existedDraw );

    MineGEDrawSystem* pGEService = MineGEDrawSystem::GetInstance();
    AcStringArray drawList;
    pGEService->getAllGEDrawsByGEType( getTypeName(), drawList );

    ArxDbgAppXdata xdata( DRAW_PARAMS_XDATA_GROUP, acdbHostApplicationServices()->workingDatabase() );
    xdata.setString( 0, _T( "" ) );                           // 当前绘制效果名称
    xdata.setInteger( 1, 0 );                                 // 绘制效果个数(临时赋给0)

    bool foundNewDraw = false;                                // 是否发现新的draw
    int i = existedDraw.length() + 1;
    int len = drawList.length();
    for( int j = 0; j < len; j++ )
    {
        if( existedDraw.contains( drawList.at( j ) ) ) continue;

        foundNewDraw = true;                                   // 发现了新的draw

        MineGEDraw* pGEDraw = pGEService->getGEDraw( getTypeName(), drawList.at( j ).kACharPtr() );
        writeKeyParamToGEDraw( pGEDraw );                          // 写入主要参数到draw中
        // 有些extra param是需要计算的，
        // 且可能与key param有关
        //pGEDraw->setAllExtraParamsToDefault();                   // 参数置为默认值
        pGEDraw->updateExtraParams();                              // 计算并更新参数
        xdata.setString( 2 * i, pGEDraw->isA()->name() );          // 绘制效果名称

        ArxDbgXdataList dataList;
        DrawParamWriter writer( &dataList );
        pGEDraw->writeExtraParam( writer );
        xdata.setList( 2 * i + 1, dataList );                      // 写入附加参数
        i++;
    }
    xdata.setInteger( 1, i - 1 );                                  // 修正draw的实际个数

    if( foundNewDraw ) xdata.setXdata( this );                     // 只有发现新的draw才会更新xdata
}

static MineGEDraw* GetCurrentDrawPointer( const CString& type )
{
    MineGEDraw* pDraw = 0;

    CString draw;
    if( GetCurDraw( type, draw ) )
    {
        MineGEDrawSystem* pDrawSystem = MineGEDrawSystem::GetInstance();
        if( pDrawSystem != 0 )
        {
            pDraw = pDrawSystem->getGEDraw( type, draw );
        }
    }
    return pDraw;
}

void MineGE::updateDraw()
{
    assertWriteEnabled();

    //m_pCurrentGEDraw = GetCurrentDrawPointer(getTypeName());
    if( m_pCurrentGEDraw != 0 )
    {
        updateDrawParams( m_pCurrentGEDraw );
    }
}

void MineGE::updateCurrentDraw()
{
    assertWriteEnabled();

    m_pCurrentGEDraw = GetCurrentDrawPointer( getTypeName() );
    if( !isNewObject() )
    {
        //acutPrintf(_T("\nnot new object"));
        // clone操作(例如动态编辑效果显示、复制等)
        // 得到的图元没有提交到数据库
        // 此时可视化参数尚未初始化，不需要更新可视化参数
        //updateDrawParams(m_pCurrentGEDraw);
    }
    else
    {
        //acutPrintf(_T("\nnew object"));
        initAllExtraParamsToXData();
    }
}

MineGEDraw* MineGE::getCurrentDraw() const
{
    return m_pCurrentGEDraw;
}

void MineGE::writeExtraParamToGEDraw( MineGEDraw* pGEDraw ) const
{
    assertReadEnabled();

    ArxDbgAppXdata xdata( DRAW_PARAMS_XDATA_GROUP, acdbHostApplicationServices()->workingDatabase() );
    xdata.getXdata( ( MineGE* )this ); // 提取扩展数据到xdata对象中(强制去掉const属性)

    int len = 0;
    xdata.getInteger( 1, len );    // 绘制效果个数
    for( int i = 1; i <= len; i++ )
    {
        CString drawName;
        xdata.getString( 2 * i, drawName ); // 绘制效果名称

        if( drawName.CompareNoCase( pGEDraw->isA()->name() ) == 0 )
        {
            ArxDbgXdataList dataList;
            xdata.getList( 2 * i + 1, dataList ); // 绘制效果的参数

            ArxDbgXdataListIterator paramList( dataList );
            DrawParamReader reader( &paramList );
            pGEDraw->readExtraParam( reader ); // 从扩展数据中读取参数
        }
    }
}

// 必须在write状态下操作
void MineGE::readExtraParamFromGEDraw( MineGEDraw* pGEDraw )
{
    assertWriteEnabled();

    ArxDbgAppXdata xdata( DRAW_PARAMS_XDATA_GROUP, acdbHostApplicationServices()->workingDatabase() );
    xdata.getXdata( this ); // 提取扩展数据到xdata对象中

    // 进行替换的扩展数据
    // 思路：将原有的扩展数据复制一份，对于修改的draw的数据先进行修改，然后再保存到新的扩展数据中
    ArxDbgAppXdata xdata2( DRAW_PARAMS_XDATA_GROUP, acdbHostApplicationServices()->workingDatabase() );
    xdata2.setString( 0, _T( "" ) ); // 当前绘制效果名称

    int len = 0;
    xdata.getInteger( 1, len );     // 绘制效果个数
    xdata2.setInteger( 1, len );

    for( int i = 1; i <= len; i++ )
    {
        CString drawName;
        xdata.getString( 2 * i, drawName ); // 绘制效果名称
        xdata2.setString( 2 * i, drawName );

        if( drawName.CompareNoCase( pGEDraw->isA()->name() ) == 0 )
        {
            ArxDbgXdataList dataList;
            DrawParamWriter writer( &dataList );
            pGEDraw->writeExtraParam( writer );

            xdata2.setList( 2 * i + 1, dataList ); // 修改并保存绘制效果的参数
        }
        else
        {
            ArxDbgXdataList dataList;
            xdata.getList( 2 * i + 1, dataList );
            xdata2.setList( 2 * i + 1, dataList );
        }
    }
    xdata2.setXdata( this );
}

void MineGE::readKeyParamFromGEDraw( MineGEDraw* pGEDraw )
{
    assertWriteEnabled();

    // 从MineGEDraw中读取更新之后的关键参数
    ArxDbgXdataList dataList;
    DrawParamWriter writer( &dataList );
    pGEDraw->writeKeyParam( writer );

    ArxDbgXdataListIterator dataListIter( dataList );
    DrawParamReader reader( &dataListIter );
    readKeyParam( reader );
}

void MineGE::writeKeyParamToGEDraw( MineGEDraw* pGEDraw ) const
{
    assertReadEnabled();

    // 将关键参数更新到MineGEDraw中
    ArxDbgXdataList dataList;
    DrawParamWriter writer( &dataList );
    writeKeyParam( writer );

    ArxDbgXdataListIterator dataListIter( dataList );
    DrawParamReader reader( &dataListIter );
    pGEDraw->readKeyParam( reader );
}

static bool GetPropertyDataFromDataObject( const AcDbObjectId& objId, const AcStringArray& names, AcStringArray& values )
{
    DataObject* pDO;
    if( Acad::eOk != acdbOpenObject( pDO, objId, AcDb::kForRead ) ) return false;

    DataHelperImpl dh( pDO );
    int len = names.length();
    for( int i = 0; i < len; i++ )
    {
        CString value;
        bool ret = dh.getPropertyData( names[i].kACharPtr(), value );
        values.append( value );
    }
    pDO->close();

    return true;
}

void MineGE::writePropertyDataToGEDraw( MineGEDraw* pGEDraw ) const
{
    assertReadEnabled();

    // 读取要查询的字段名称集合
    AcStringArray names;
    pGEDraw->regPropertyDataNames( names );
    if( names.isEmpty() ) return;

    // 查询数据，并写入到values中
    AcStringArray values;
    if( !GetPropertyDataFromDataObject( getDataObject(), names, values ) )
    {
        int len = names.length();
        for( int i = 0; i < len; i++ )
        {
            // 如果获取数据失败，添加与names等长的空字符串
            values.append( _T( "" ) );
        }
    }
	//int len = names.length();
	//for( int i = 0; i < len; i++ )
	//{
	//	acutPrintf(_T("\n字段名:%s\t值:%s\n"),names[i].kACharPtr(),values[i].kACharPtr());
	//}

    // 将查询到的数据返回到pGEDraw
    pGEDraw->readPropertyDataFromGE( values );
}

void MineGE::writeParamToGEDraw( MineGEDraw* pGEDraw ) const
{
    assertReadEnabled();

    // 1、将关键参数更新到MineGEDraw中
    writeKeyParamToGEDraw( pGEDraw );

    // 2、从扩展数据中提取参数
    writeExtraParamToGEDraw( pGEDraw );

    // 3、读取属性数据，并传递给MineGEDraw
    writePropertyDataToGEDraw( pGEDraw );
}

void MineGE::readParamFromGEDraw( MineGEDraw* pGEDraw )
{
    assertWriteEnabled();

    // 1、从MineGEDraw中读取更新后的关键参数
    readKeyParamFromGEDraw( pGEDraw );

    // 2、将draw的参数保存到扩展数据中
    readExtraParamFromGEDraw( pGEDraw );
}

void MineGE::updateDrawParam( bool readOrWrite ) const
{
    if( readOrWrite )
    {
        ( ( MineGE* )this )->readParamFromGEDraw( m_pCurrentGEDraw ); // 强制去掉const修饰
    }
    else
    {
        writeParamToGEDraw( m_pCurrentGEDraw );
    }
}

Acad::ErrorStatus MineGE::dwgOutFields( AcDbDwgFiler* pFiler ) const
{
    //acutPrintf(_T("\nid:%d call MineGE::dwgOutFields()..."), objectId());

    assertReadEnabled () ;
    Acad::ErrorStatus es = AcDbEntity::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;

    if ( ( es = pFiler->writeUInt32 ( MineGE::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeSoftPointerId( m_dataObjectId );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus MineGE::dwgInFields( AcDbDwgFiler* pFiler )
{
    //acutPrintf(_T("\nid:%d call MineGE::dwgInFields()..."), objectId());

    assertWriteEnabled () ;
    Acad::ErrorStatus es = AcDbEntity::dwgInFields ( pFiler );
    if ( es != Acad::eOk )
        return ( es ) ;

    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > MineGE::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    /* 199之前(包括199)的版本使用 */
    //AcString str;
    //pFiler->readString(str);
    //pFiler->readString(str);

    /* 199之后(不包括199)的版本使用 */
    AcDbSoftPointerId id;
    pFiler->readSoftPointerId( &id );
    m_dataObjectId = id;

    initDraw();

    return ( pFiler->filerStatus () ) ;
}

// DWORD <==> Adesk::UInt32 <==> unsigned long
// Adesk::UInt8 <==> unsigned char
static void LONG2RGB( Adesk::UInt32 rgbColor, Adesk::UInt8& r, Adesk::UInt8& g, Adesk::UInt8& b )
{
    // 下面2段代码是等价的

    /*r = ( rgbColor & 0xffL );
    g = ( rgbColor & 0xff00L ) >> 8;
    b = rgbColor >> 16;*/

    r = GetRValue( rgbColor );
    g = GetGValue( rgbColor );
    b = GetBValue( rgbColor );
}

// 获取cad绘图窗口背景色
static bool GetBackgroundColor( Adesk::UInt8& r, Adesk::UInt8& g, Adesk::UInt8& b )
{
    // 获取cad当前的所有颜色设置
    // 右键"选项"->"显示"->"颜色"
    AcColorSettings cs;
    if( !acedGetCurrentColors( &cs ) ) return false;

    // 读取背景颜色
    DWORD rgbColor = cs.dwGfxModelBkColor;
    LONG2RGB( rgbColor, r, g, b );
    return true;
}

// 废弃代码
/*
Adesk::UInt32 rgbColor = AcCmEntityColor::lookUpRGB(cl.colorIndex()); // 转换成rgb颜色

AcCmEntityColor bgColor(255-r, 255-g, 255-b); // RGB颜色

AcCmEntityColor bgColor;
bgColor.setRGB(0, 0, 0);
if(layerColor.colorIndex() == 7) bgColor.setRGB(r, g, b); // RGB颜色
else bgColor.setRGB(255-r, 255-g, 255-b);
 */

/*
 * 在绘制闭合图形时，AcGiFillType默认为kAcGiFillAlways (始终填充)
 * 闭合图形包括：圆、多边形、网格等
 * 参见：AcGiSubEntityTraits::fillType()方法说明
 * 例如，绘制一个圆，当前颜色是黑底白色，那么采用自定义实体绘制的圆有2种情况:
 *	    1) arx程序加载的情况下-- 白边+黑底填充(正常效果，和cad的圆是一样的)
 *		2) arx程序卸载，cad采用代理实体模式显示图元 -- 白边+白底填充
 * 具体参见：绘制填充圆的一些说明.doc
 */

// 慎用AcCmColor::colorIndex()方法，因为color index总共只有256种，且白/黑都使用7表示，无法区分
// 如果要使用rgb颜色，应使用AcCmEntityColor或AcCmColor对象
void MineGE::drawBackground( MineGEDraw* pGEDraw, AcGiWorldDraw* mode )
{
    if( isNewObject() ) return;

    AcGePoint3dArray pts;
    m_pCurrentGEDraw->caclBackGroundMinPolygon( pts );

    // 用户没有定义边界
    if( pts.isEmpty() ) return;

    // 获取cad绘图窗口背景色
    Adesk::UInt8 r, g, b;
    if( !GetBackgroundColor( r, g, b ) ) return;

    AcGiSubEntityTraits& traits = mode->subEntityTraits();
    // 保存原有的属性
    Adesk::UInt16 cl = traits.color();;
    AcGiFillType ft = traits.fillType();

    AcCmEntityColor bgColor( r, g, b );
    traits.setTrueColor( bgColor );
    traits.setFillType( kAcGiFillAlways ); // 填充
    //acutPrintf(_T("\n颜色索引：%d"), bgColor.colorIndex());
    mode->geometry().polygon( pts.length(), pts.asArrayPtr() );

    // 恢复属性
    traits.setFillType( ft );
    traits.setColor( cl );
}

Adesk::Boolean MineGE::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    if( m_pCurrentGEDraw == 0 ) return Adesk::kTrue;

    //acutPrintf(_T("\ncall id:%d MineGE::subWorldDraw()..."), objectId());
   // acutPrintf(_T("\ncall drawname:%s..."), m_pCurrentGEDraw->isA()->name());

    // 1、更新参数到MineGEDraw中
    updateDrawParam( false );
    // 2、绘制背景块
    // 该方法在动态效果中可能会有一些问题,特别是jig
    // 猜测原因:绘制填充的多边形可能比较慢
    drawBackground( m_pCurrentGEDraw, mode );

    // 3、draw对象负责绘制实际图形
    m_pCurrentGEDraw->worldDraw( mode );

    return Adesk::kTrue;
}

Acad::ErrorStatus MineGE::subTransformBy( const AcGeMatrix3d& xform )
{
    if( m_pCurrentGEDraw == 0 )
    {
        //acutPrintf(_T("\nsubTransformBy中draw=null"));
        return Acad::eOk;
    }

    //acutPrintf(_T("\ncall id:%d MineGE::subTransformBy()..."), objectId());

    // 1、更新参数到MineGEDraw中
    updateDrawParam( false );

    // 2、执行变换
    m_pCurrentGEDraw->transformBy( xform );

    // 3、从MineGEDraw中读取更新后的参数
    updateDrawParam( true );

    // 4、将附带所有的TagGE
    //TransformAllTagGE(objectId(), xform);

    return Acad::eOk;
}

Acad::ErrorStatus MineGE::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    Adesk::GsMarker gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    if( m_pCurrentGEDraw == 0 ) return Acad::eOk;

    // 1、更新参数到MineGEDraw中
    updateDrawParam( false );

    // 2、draw对象负责绘制实际图形
    m_pCurrentGEDraw->getOsnapPoints( osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds );

    return Acad::eOk;
}

Acad::ErrorStatus MineGE::subGetGripPoints(
    AcGePoint3dArray& gripPoints,
    AcDbIntArray& osnapModes,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    if( m_pCurrentGEDraw == 0 ) return Acad::eOk;

    // 1、更新参数到MineGEDraw中
    updateDrawParam( false );

    // 2、draw对象负责绘制实际图形
    m_pCurrentGEDraw->getGripPoints( gripPoints, osnapModes, geomIds );

    return Acad::eOk;
}

Acad::ErrorStatus MineGE::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    if( m_pCurrentGEDraw == 0 ) return Acad::eOk;

    //acutPrintf(_T("\ncall id:%d MineGE::subMoveGripPointsAt()..."), objectId());

    // 1、更新参数到MineGEDraw中
    updateDrawParam( false );

    // 2、执行变换
    m_pCurrentGEDraw->moveGripPointsAt( indices, offset );

    // 3、从MineGEDraw中读取更新后的参数
    updateDrawParam( true );

    // 所有的标签图元也进行变换
    // 变换结果偏差较大，不建议在程序中使用
    //TransformAllTagGE(objectId(), AcGeMatrix3d::translation(offset));

    return Acad::eOk;
}

Acad::ErrorStatus MineGE::subGetGeomExtents( AcDbExtents& extents ) const
{
    assertReadEnabled () ;

    if( m_pCurrentGEDraw == 0 ) return AcDbEntity::subGetGeomExtents( extents );

    // 1、更新参数到MineGEDraw中
    updateDrawParam( false );

    Acad::ErrorStatus es = m_pCurrentGEDraw->getGeomExtents( extents );
    // Draw没有重载实现subGetGeomExtents
    if( Acad::eOk != es )
    {
        //acutPrintf(_T("\n使用背景消隐多边形计算缩放区域...\n"));
        // 使用caclBackGroundMinPolygon()方法计算的多边形代替
        AcGePoint3dArray pts;
        m_pCurrentGEDraw->caclBackGroundMinPolygon( pts );
        if( pts.isEmpty() )
        {
            es = Acad::eInvalidExtents;
        }
        else
        {
            int len = pts.length();
            for( int i = 0; i < len; i++ )
            {
                extents.addPoint( pts[i] );
            }
            es = Acad::eOk;
        }
    }

    return es;
}

// cad会频繁的调用subClose
Acad::ErrorStatus MineGE::subClose( void )
{
    //acutPrintf(_T("\nid:%d call MineGE::subClose()...\n"), objectId());

    Acad::ErrorStatus es = AcDbEntity::subClose () ;

    // new对象并成功提交到数据库之后
    // 初始化可视化效果参数(扩展数据)
    // 构造数据对象(扩展词典)
    if( es == Acad::eOk )
    {
        initDraw();
        initPropertyData();
    }
    return es;
}