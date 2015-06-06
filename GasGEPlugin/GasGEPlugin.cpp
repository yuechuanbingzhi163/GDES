#include "StdAfx.h"
#include "GasGEPlugin.h"

// 瓦斯突出点
#include "GOPBlockDraw.h"
#include "GCPBlockDraw.h"
#include "GPPBlockDraw.h"
#include "PPPBlockDraw.h"
#include "DCIBlockDraw.h"
#include "GFPBlockDraw.h"
#include "CDHBlockDraw.h"

void GasGEPlugin::initialize( MineGEDrawManager* pGEDrawManager )
{
    BEGIN_REGISTER_MINEGE_DRAW( MineGEDrawManager, pGEDrawManager )

    // 瓦斯突出点
    REGISTER_MINEGE_DRAW( SmallGOP, SmallGOPBlockDraw )
    REGISTER_MINEGE_DRAW( MidGOP, MidGOPBlockDraw )
    REGISTER_MINEGE_DRAW( LargeGOP, LargeGOPBlockDraw )
    REGISTER_MINEGE_DRAW( LargerGOP, LargerGOPBlockDraw )

    // 瓦斯含量点
    REGISTER_MINEGE_DRAW( GCP, GCPBlockDraw )

    // 瓦斯压力点
    REGISTER_MINEGE_DRAW( GPP, GPPBlockDraw )

    // 动力现象点
    REGISTER_MINEGE_DRAW( PPP, PPPBlockDraw )

    // 突出危险性预测指标
    REGISTER_MINEGE_DRAW( CSDCI, CSDCIBlockDraw )
    REGISTER_MINEGE_DRAW( WDCI1, WDCI1BlockDraw )
    REGISTER_MINEGE_DRAW( WDCI2, WDCI2BlockDraw )

    // 瓦斯涌出点
    REGISTER_MINEGE_DRAW( WGFP, WGFPBlockDraw )
    REGISTER_MINEGE_DRAW( TGFP, TGFPBlockDraw )

    // 见煤钻孔
    REGISTER_MINEGE_DRAW( CDH, CDHBlockDraw )

    END_REGISTER_MINEGE_DRAW
}

void GasGEPlugin::unInitialize( MineGEDrawManager* pGEDrawManager )
{
    BEGIN_UNREGISTER_MINEGE_DRAW( MineGEDrawManager, pGEDrawManager )

    // 瓦斯突出点
    UNREGISTER_MINEGE_DRAW( SmallGOP, SmallGOPBlockDraw )
    UNREGISTER_MINEGE_DRAW( MidGOP, MidGOPBlockDraw )
    UNREGISTER_MINEGE_DRAW( LargeGOP, LargeGOPBlockDraw )
    UNREGISTER_MINEGE_DRAW( LargerGOP, LargerGOPBlockDraw )

    // 瓦斯含量点
    UNREGISTER_MINEGE_DRAW( GCP, GCPBlockDraw )

    // 瓦斯压力点
    UNREGISTER_MINEGE_DRAW( GPP, GPPBlockDraw )

    // 动力现象点
    UNREGISTER_MINEGE_DRAW( PPP, PPPBlockDraw )

    // 突出危险性预测指标
    UNREGISTER_MINEGE_DRAW( CSDCI, CSDCIBlockDraw )
    UNREGISTER_MINEGE_DRAW( WDCI1, WDCI1BlockDraw )
    UNREGISTER_MINEGE_DRAW( WDCI2, WDCI2BlockDraw )

    // 瓦斯涌出点
    UNREGISTER_MINEGE_DRAW( WGFP, WGFPBlockDraw )
    UNREGISTER_MINEGE_DRAW( TGFP, TGFPBlockDraw )

    // 见煤钻孔
    UNREGISTER_MINEGE_DRAW( CDH, CDHBlockDraw )

    END_UNREGISTER_MINEGE_DRAW
}

// 插件创建函数定义实现并导出
MINEGE_PLUGIN_CREATE_FUNC_IMPL( GasGEPlugin )

// draw创建函数实现宏(删除了，因为还不能确定draw的构造方式，目前的draw是通过"名称+ARX RTTI"来创建的)
