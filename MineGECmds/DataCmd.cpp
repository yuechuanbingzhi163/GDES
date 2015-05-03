#include "StdAfx.h"
#include "DataCmd.h"

#include "../MineGE/DataHelper.h"

#include "../ArxHelper/HelperClass.h"

void DataCmd::CopyData()
{
    AcDbObjectIdArray objIds;
    ArxUtilHelper::GetPickSetEntity( objIds );
    if( objIds.length() != 1 ) return; // 没有图元或者超过1个图元被选中

    AcDbObjectId targetObjId = ArxUtilHelper::SelectObject( _T( "选择目标图元(target):" ) );
    if( targetObjId.isNull() ) return;

    if( objIds[0] == targetObjId )
    {
        acutPrintf( _T( "\n选择的2个图元是都是指向一个图元..." ) );
        return;
    }

    DataHelper::CopyPropertyData( objIds[0], targetObjId );
}

void DataCmd::CopyData2( void )
{
    //acutPrintf( _T( "\n属性数据复制...\n" ) );

    AcDbObjectId objId1 = ArxUtilHelper::SelectObject( _T( "选择源图元(source):" ) );
    if( objId1.isNull() ) return;

    AcDbObjectId objId2 = ArxUtilHelper::SelectObject( _T( "选择目标图元(target):" ) );
    if( objId2.isNull() ) return;
	
    DataHelper::CopyPropertyData( objId1, objId2 );
}