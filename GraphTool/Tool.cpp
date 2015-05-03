#include "stdafx.h"

#include "Tool.h"

#include "../MineGE/TagGE.h"
#include "../MineGE/ConstData.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

// 判断是否需风巷道
//bool IsNeedAir( const AcDbObjectId& objId )
//{
//    CString v1, v2;
//    DataHelper::GetPropertyData( objId, _T( "功能类型" ), v1 );
//    DataHelper::GetPropertyData( objId, _T( "是否单独配风" ), v2 );
//
//    int i1 = 0;
//    ArxUtilHelper::StringToInt( v1, i1 );
//    int ft = i1 % 6; // 0-5
//
//    int i2 = 0;
//    ArxUtilHelper::StringToInt( v2, i2 );
//    bool needAir = ( i2 != 0 );
//
//    return ( needAir || ft == FT_ELECTRIC || ft == FT_DIESEL );
//}

// 判断是否变电所
// 前提：先确定是机电硐室
//bool IsPowerSubstation( const AcDbObjectId& objId )
//{
//    CString v;
//    DataHelper::GetPropertyData( objId,  _T( "机电硐室类型" ), v );
//    int i = 0;
//    ArxUtilHelper::StringToInt( v, i );
//    i = i % 4; // 参见RT_TYPE
//    return ( i == RT_POWER_SUBSTATION );
//}

void GetHosts( const AcDbObjectIdArray& objIds, AcDbObjectIdArray& hosts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        TagGE* pTag = TagGE::cast( pObj );
        if( pTag == 0 ) continue;

        AcDbObjectId host = pTag->getRelatedGE();
        if( !host.isNull() ) hosts.append( host );
    }
    actrTransactionManager->endTransaction();
}

void FindTTunnels( AcDbObjectIdArray& objIds )
{
    DrawHelper::FindMineGEs( _T( "TTunnel" ), objIds );
}

void FindMainFans( AcDbObjectIdArray& objIds )
{
    DrawHelper::FindMineGEs( _T( "MainFan" ), objIds );
}

// 查找所有的风门和调节风窗
void FindGates( AcDbObjectIdArray& objIds )
{
    DrawHelper::FindMineGEs( _T( "Gate" ), objIds );
    DrawHelper::FindMineGEs( _T( "Casement" ), objIds );
}

// 如果掘进工作面没有关联局扇，则id默认为null
void FindLocalFans( const AcDbObjectIdArray& tObjIds, AcDbObjectIdArray& objIds )
{
    // 查找所有的局部通风机
    AcDbObjectIdArray fanObjIds;
    DrawHelper::FindMineGEs( _T( "LocalFan" ), fanObjIds );

    // 查找局部通风机对应的掘进工作面
    AcDbObjectIdArray tObjIds2;
    GetHosts( fanObjIds, tObjIds2 );

    // 输入的掘进工作面个数和查找得到的掘进工作面个数必须要相等
    //if(tObjIds2.length() != tObjIds.length()) return false;

    // 调整顺序
    int len = tObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        int pos = tObjIds2.find( tObjIds[i] );
        if( pos < 0 )
        {
            objIds.append( AcDbObjectId::kNull ); // 使用null id代替
        }
        else
        {
            objIds.append( fanObjIds[pos] );
        }
    }

    //return true;
}

void FindChimneys( const AcDbObjectIdArray& tObjIds, AcDbObjectIdArray& objIds )
{
    // 查找所有的风筒
    AcDbObjectIdArray cObjIds;
    DrawHelper::FindMineGEs( _T( "Chimney" ), cObjIds );

    // 查找风筒对应的掘进工作面
    AcDbObjectIdArray tObjIds2;
    GetHosts( cObjIds, tObjIds2 );

    // 输入的掘进工作面个数和查找得到的掘进工作面个数必须要相等
    //if(tObjIds2.length() != tObjIds.length()) return false;

    // 调整顺序
    int len = tObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        int pos = tObjIds2.find( tObjIds[i] );
        if( pos < 0 )
        {
            objIds.append( AcDbObjectId::kNull ); // 使用null id代替
        }
        else
        {
            objIds.append( cObjIds[pos] );
        }
    }

    //return true;
}

void GetInsertPts( const AcDbObjectIdArray& objIds, AcGePoint3dArray& pts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return ;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        DirectionTagGE* pDir = DirectionTagGE::cast( pObj );
        if( pDir == 0 ) continue;

        pts.append( pDir->getInsertPt() );
    }

    actrTransactionManager->endTransaction();
}

bool IsMainPowerSubtation( const AcDbObjectId& objId )
{
    //if(!ArxUtilHelper::IsEqualType(_T("MachineRoom"), objId)) return true;

    CString str;
    //DataHelper::GetPropertyData(objId, _T("机电硐室类型"), str);

    //// 是否变电所
    //int i = 0;
    //ArxUtilHelper::StringToInt(str, i);
    //if(i != 1) return true;

    // 分析变电所的名称是否包含"采区"或"盘区"等文字
    DataHelper::GetPropertyData( objId, _T( "名称" ), str );

    if( str.Find( _T( "采区" ) ) != -1 ) return false;
    if( str.Find( _T( "区段" ) ) != -1 ) return false;
    if( str.Find( _T( "盘区" ) ) != -1 ) return false;
    return true;
}