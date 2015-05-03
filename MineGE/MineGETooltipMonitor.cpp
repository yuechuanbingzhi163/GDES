#include "StdAfx.h"
#include "MineGETooltipMonitor.h"

#include "FieldHelper.h"
#include "DataHelper.h"

#include "FieldInfoHelper.h"
//#include "TypeNameMapHelper.h"

#include "../ArxHelper/HelperClass.h"

// 获取要在tooltip显示的字段
static void GetFieldsShowInToolTip( const CString& type, AcStringArray& fields )
{
    AcStringArray allFields;
    FieldHelper::GetAllFields( type, allFields );
    if( allFields.isEmpty() ) return;

    int len = allFields.length();
    for( int i = 0; i < len; i++ )
    {
        FieldInfo info;
        FieldInfoHelper::ReadFieldInfo( type, allFields[i].kACharPtr(), info );
        if( info.m_showInTooltip )
        {
            fields.append( allFields[i] );
        }
    }
}

// 格式化tooltip
static void FormatToolTip( const CString& type, const AcDbObjectId& objId, const AcStringArray& fields, AcString& toolTipStr )
{
    CString type2( type );
    //TypeNameMapHelper::GetTypeNameMap(type, type2); // 用户在词典中指定的名称
    ArxClassHelper::GetDxfName( type, type2 ); // 获取dxf名称(在类定义的时候，通过ARX宏指定)

    // 图元类型名称
    AcString str;
    str.format( _T( "\n图元类型: %s\n" ), type2 );

    toolTipStr.append( str );
    str.format( _T( "属性数据\t字段个数:%d\n" ), fields.length() );
    toolTipStr.append( str );

    if( fields.isEmpty() )
    {
        toolTipStr.format( _T( "\n没有可以显示的数据!" ) );
        //toolTipStr.append(_T("\n  没有可以显示的数据\n  请在\"字段管理对话框\"中设置是否显示"));
        return;
    }

    int len = fields.length();
    for( int i = 0; i < len; i++ )
    {
        CString name = fields[i].kACharPtr();
        CString value;
        DataHelper::GetPropertyData( objId, name, value );

        str.format( _T( "%s: \t%s\n" ), name, value );
        toolTipStr.append( str );
    }
}

MineGETooltipMonitor::MineGETooltipMonitor ( AcApDocument* pDoc ) : AcEdInputPointMonitor(), mpDocument( pDoc )
{
    if ( pDoc )
    {
        //acutPrintf(_T("\nMineGETooltipMonitor : %ld"), (long)pDoc);
        pDoc->inputPointManager ()->addPointMonitor ( this ) ;
    }
}

MineGETooltipMonitor::~MineGETooltipMonitor ()
{
    Detach () ;
}

void MineGETooltipMonitor::Attach ( AcApDocument* pDoc )
{
    Detach () ;
    if ( mpDocument == NULL )
    {
        if ( ( mpDocument = pDoc ) != NULL )
            pDoc->inputPointManager ()->addPointMonitor ( this ) ;
    }
}

void MineGETooltipMonitor::Detach ()
{
    if ( mpDocument )
    {
        mpDocument->inputPointManager ()->removePointMonitor ( this ) ;
        mpDocument = NULL ;
    }
}

AcApDocument* MineGETooltipMonitor::Subject () const
{
    return ( mpDocument ) ;
}

bool MineGETooltipMonitor::IsAttached () const
{
    return ( mpDocument != NULL ) ;
}

bool MineGETooltipMonitor::excludeFromOsnapCalculation (
    const AcArray<AcDbObjectId>& nestedEntity,
    Adesk::GsMarker gsSelectionMark )
{
    return ( AcEdInputPointMonitor::excludeFromOsnapCalculation ( nestedEntity, gsSelectionMark ) ) ;
}

Acad::ErrorStatus MineGETooltipMonitor::monitorInputPoint (
    bool& appendToTooltipStr,
    ACHAR*& additionalTooltipString,
    AcGiViewportDraw* drawContext,
    AcApDocument* document,
    bool pointComputed,
    int history,
    const AcGePoint3d& lastPoint,
    const AcGePoint3d& rawPoint,
    const AcGePoint3d& grippedPoint,
    const AcGePoint3d& cartesianSnappedPoint,
    const AcGePoint3d& osnappedPoint,
    AcDb::OsnapMask osnapMask,
    const AcArray<AcDbCustomOsnapMode*>& customOsnapModes,
    AcDb::OsnapMask osnapOverrides,
    const AcArray<AcDbCustomOsnapMode*>& customOsnapOverrides,
    const AcArray<AcDbObjectId>& apertureEntities,
    const AcArray<AcDbObjectIdArray, AcArrayObjectCopyReallocator<AcDbObjectIdArray> >& nestedApertureEntities,
    const AcArray<Adesk::GsMarker>& gsSelectionMark,
    const AcArray<AcDbObjectId>& keyPointEntities,
    const AcArray<AcDbObjectIdArray, AcArrayObjectCopyReallocator<AcDbObjectIdArray> >& nestedKeyPointEntities,
    const AcArray<Adesk::GsMarker>& keyPointGsSelectionMark,
    const AcArray<AcGeCurve3d*>& alignmentPaths,
    const AcGePoint3d& computedPoint,
    const ACHAR* tooltipString
)
{
    // !!!需要对文档进行锁定操作!!!
    // 否则所有与词典相关的操作都会返回错误的结果
    if( !apertureEntities.isEmpty() )
    {
        // 锁定文档(document参数表示文档对象)
        acDocManager->lockDocument( document );

        // 获取数据对象
        AcDbObjectId dObjId;
        if( !DataHelper::GetDataObject( apertureEntities[0], dObjId ) ) return Acad::eOk;

        // 获取类型名称
        CString type;
        if( !DataHelper::GetTypeName( apertureEntities[0], type ) ) return Acad::eOk;

        // 查找字段
        AcStringArray fields;
        GetFieldsShowInToolTip( type, fields );

        // 格式化tooltip
        AcString toolTipStr;
        FormatToolTip( type, dObjId, fields, toolTipStr );

        // 解锁当前文档
        acDocManager->unlockDocument( document );

        // 追加tooltip
        appendToTooltipStr = true;
        additionalTooltipString = _tcsdup( toolTipStr.kACharPtr() );
    }
    return ( Acad::eOk );
}
