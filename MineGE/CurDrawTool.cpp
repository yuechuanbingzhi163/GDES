#include "stdafx.h"

#include "CurDrawTool.h"
#include "config.h"
#include "DrawHelper.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGEDraw/MineGEDrawSystem.h"

static void BuildCurDrawsDict( const AcStringArray& types )
{
    MineGEDrawSystem* pDrawSystem = MineGEDrawSystem::GetInstance();
    if( pDrawSystem == 0 ) return;

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( CUR_DRAW_DICT );
    int len = types.length();

    for( int i = 0; i < len; i++ )
    {
        CString type = types[i].kACharPtr();
        if( !pDictTool->findKey( type ) )
        {
            pDictTool->addEntry( type, pDrawSystem->getDefaultGEDrawName( type ) );
        }
    }

    delete pDictTool;
}

// 设置当前可视化效果
void InitAllCurDraws()
{
    AcStringArray types;
    DrawHelper::GetAllRegGETypesForDraw( types );
    if( types.isEmpty() ) return;

    BuildCurDrawsDict( types );
}

bool SetCurDraw( const CString& type, const CString& draw )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( CUR_DRAW_DICT );
    bool ret = pDictTool->modifyEntry( type, 1, draw );
    delete pDictTool;

    return ret;
}

bool GetCurDraw( const CString& type, CString& draw )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( CUR_DRAW_DICT );
    bool ret = pDictTool->getEntry( type, 1, draw );
    delete pDictTool;

    return ret;
}