#include "stdafx.h"
#include "DrawCmd.h"

#include "../DefGE/Tunnel.h"
#include "../DefGE/WindDirection.h"

#include "../ArxHelper/ArxUtilHelper.h"

/* 全局函数(实现在DWGHelper.cpp中) */
extern bool SelectDwg( const CString& msg, CString& dwgFilePath );
extern bool ReadAllLinesFromDwg( const CString& dwgFilePath, const CString& layerName, AcGePoint3dArray& sptArray, AcGePoint3dArray& eptArray );

// 分析始末点坐标，找出所有公共的节点坐标
static void GetJoints( const AcGePoint3dArray& sptArray, const AcGePoint3dArray& eptArray, AcGePoint3dArray& pts )
{
    int len = sptArray.length();
    for( int i = 0; i < len; i++ )
    {
        AcGePoint3d spt = sptArray[i];
        AcGePoint3d ept = eptArray[i];
        if( !pts.contains( spt ) ) pts.append( spt );
        if( !pts.contains( ept ) ) pts.append( ept );
    }
}

static void DrawTunnels( const AcGePoint3dArray& sptArray,
                         const AcGePoint3dArray& eptArray,
                         bool addDir = false )
{
    int len = sptArray.length();
    for( int i = 0; i < len; i++ )
    {
        AcGePoint3d spt = sptArray[i];
        AcGePoint3d ept = eptArray[i];
        Tunnel* pTunnel = new Tunnel( spt, ept );
        ArxUtilHelper::PostToModelSpace( pTunnel );

        // 绘制风流方向
        if( addDir )
        {
            AcGeVector3d v = ept - spt;
            double angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
            WindDirection* pDir = new WindDirection( spt + v * 0.5, angle ); // 巷道中心位置
            pDir->setRelatedGE( pTunnel->objectId() );
            ArxUtilHelper::PostToModelSpace( pDir );
        }
    }
}
//

static void PrintMsg( const CString& dwgFilePath, const CString& layerName, bool isOk )
{
    CString msg;
    msg.Format( _T( "读取文件\n\t%s\n【%s】图层上的所有直线数据：【%s】" ), dwgFilePath, layerName, ( isOk ? _T( "成功" ) : _T( "失败" ) ) );
    if( !isOk )
    {
        msg.AppendFormat( _T( "\n\n请确保导入的dwg文件没有被AutoCAD使用!!!" ) );
    }
    AfxMessageBox( msg );
}

void DrawCmd::AutoDraw()
{
    CString dwgFilePath;
    if( !SelectDwg( _T( "请选择一个dwg文件" ), dwgFilePath ) ) return;

    CString srcLayer = _T( "巷道" );
    // 读取dwg文件中的所有直线始末点坐标
    AcGePoint3dArray sptArray, eptArray;
    if( !ReadAllLinesFromDwg( dwgFilePath, srcLayer, sptArray, eptArray ) )
    {
        PrintMsg( dwgFilePath, srcLayer, false );
        return;
    }

    if( sptArray.length() != eptArray.length() )
    {
        PrintMsg( dwgFilePath, srcLayer, false );
        return ;
    }

    if( sptArray.length() == 0 )
    {
        AfxMessageBox( _T( "所选图形的【巷道】图层上没有直线" ) );
        return;
    }

    PrintMsg( dwgFilePath, srcLayer, true );

    // 分析始末点坐标，得到所有的节点坐标
    AcGePoint3dArray pts;
    GetJoints( sptArray, eptArray, pts );

    bool addDir = ( IDYES == AfxMessageBox( _T( "是否绘制风流方向" ), MB_YESNO ) );
    DrawTunnels( sptArray, eptArray, addDir );
    //DrawJoints(pts);
}