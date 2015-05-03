#include "stdafx.h"

// 选择dwg图形文件
bool SelectDwg( const CString& msg, CString& dwgFilePath )
{
    struct resbuf* rb = acutNewRb( RTSTR );
    bool ret = ( RTNORM == acedGetFileD( msg, NULL, _T( "dwg" ), 0, rb ) );
    if( ret )
    {
        dwgFilePath = rb->resval.rstring;
    }
    acutRelRb( rb ); // 必须要释放占用的内存
    return ret;
}

// 从dwg文件中的"巷道"图层读取所有的直线始末坐标
bool ReadAllLinesFromDwg( const CString& dwgFilePath, const CString& layerName, AcGePoint3dArray& sptArray, AcGePoint3dArray& eptArray )
{
    AcDbDatabase db( false );
    if( Acad::eOk != db.readDwgFile( dwgFilePath ) ) return false;

    // 获得名称为Blk的块表记录
    AcDbBlockTable* pBlkTbl;
    if ( Acad::eOk != db.getBlockTable( pBlkTbl, AcDb::kForRead ) )
    {
        acedAlert( _T( "获得块表失败!" ) );
        return false;
    }
    AcDbBlockTableRecord* pBlkTblRcd;
    Acad::ErrorStatus es = pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();
    if ( Acad::eOk != es )
    {
        acedAlert( _T( "打开模型空间块表记录失败!" ) );
        return false;
    }

    // 创建块表记录遍历器
    AcDbBlockTableRecordIterator* pItr;
    if ( Acad::eOk != pBlkTblRcd->newIterator( pItr ) )
    {
        acedAlert( _T( "创建遍历器失败!" ) );
        pBlkTblRcd->close();
        return false;
    }

    for ( pItr->start(); !pItr->done(); pItr->step() )
    {
        AcDbEntity* pEnt;
        if( Acad::eOk != pItr->getEntity( pEnt, AcDb::kForRead ) ) continue;

        if( layerName.CompareNoCase( pEnt->layer() ) == 0 ) // 只选择指定图层
        {
            AcDbLine* pLine = AcDbLine::cast( pEnt );   // 只选择直线
            if( pLine != 0 )
            {
                // 获取直线的始末点坐标
                AcGePoint3d spt, ept;
                pLine->getStartPoint( spt );
                pLine->getEndPoint( ept );

                // !!![修正]!!!
                //		目前不考虑3维情况，强制所有坐标z=0
                spt.z = 0;
                ept.z = 0;

                if( spt != ept ) // 排除始末点相同的直线
                {
                    sptArray.append( spt );
                    eptArray.append( ept );
                }
            }
        }
        pEnt->close();
    }
    delete pItr;
    pBlkTblRcd->close();

    //acutPrintf(_T("\n坐标个数:%d"), sptArray.length());
    return true;
}