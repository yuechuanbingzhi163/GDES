#include "StdAfx.h"
#include "DefGEPlugin.h"

// 双线巷道
#include "DoubleTunnelDraw.h"

// 单线巷道
#include "SingleTunnelDraw.h"

// 多段线巷道
#include "PolyLineTunnelDraw.h"

// 单箭头
#include "SimpelDirectionDraw.h"

// 多段线箭头
#include "PolyLineDirectionDraw.h"

// 简单风机
#include "SimpleLocalFanDraw.h"
#include "FanTagGEDraw.h"

// 简单风筒
#include "SimpleChimneyDraw.h"


// 掘进工作面
#include "DoubleTTunnelDraw.h"
#include "PolyLineTTunnelDraw.h"

//节点
#include "SimpleJointDraw.h"

// 简单风库
#include "SimpleWindLibraryDraw.h"

//风筒的出入口风量
#include "QTagGEDraw.h"

//有效射程
#include "EffectRanTagGEDraw.h"

//瓦斯抽采管路
#include "GasTubeGEDraw.h"

//瓦斯泵
#include "GasPumpGEDraw.h"

//阀
#include "ValveGEDraw.h"

//放水器
#include "TailraceDraw.h"

//流量计
#include "FlowmeterGEDraw.h"

//阻火器
#include "BackFireGEDraw.h"

//测定孔
#include "DetermineHoleGEDraw.h"

//瓦斯流量
#include "GasFlowTagGEDraw.h"

//钻孔
#include "DrillGEDraw.h"

//温度传感器
#include "TempeSensorGEDraw.h"

//流量传感器
#include "FlowSensorGEDraw.h"

//压差传感器
#include "DifferPressSensorGEDraw.h"

//瓦斯传感器
#include "GasSensorGEDraw.h"

//钻孔
#include "SimplePoreDraw.h"
//钻场
#include "SimpleDrillSiteDraw.h"
//煤层面
#include "SimpleCoalSurfaceDraw.h"
//石门
#include "SimpleRockGateDraw.h"

// 井筒
//#include "DoubleShaftDraw.h"

// 指北针
//#include "CompassBlockDraw.h"

// CircularJointDraw绘制效果的Jig实现
//#include "CircularJointDraw_Jig.h"

// SingleTunnelDraw绘制效果的Jig实现
//#include "SingleTunnelDraw_Jig.h"

// DoubleTunnelDraw绘制效果的Jig实现
//#include "DoubleTunnelDraw_Jig.h"

// DoubleArcTunnelDraw绘制效果的Jig实现
//#include "SingleArcTunnelDraw_Jig.h"

// DoubleArcTunnelDraw绘制效果的jig实现
//#include "DoubleArcTunnelDraw_Jig.h"

// DoubleWorkSurfaceDraw绘制效果的jig实现
//#include "DoubleWorkSurfaceDraw_Jig.h"

void DefGEPlugin::initialize( MineGEDrawManager* pGEDrawManager )
{
    BEGIN_REGISTER_MINEGE_DRAW( MineGEDrawManager, pGEDrawManager )
    // 巷道
    REGISTER_MINEGE_DRAW( Tunnel, DoubleTunnelDraw )
    REGISTER_MINEGE_DRAW( Tunnel, PolyLineTunnelDraw )
    REGISTER_MINEGE_DRAW( Tunnel, SingleTunnelDraw )
    //REGISTER_MINEGE_DRAW_JIG(Tunnel, DoubleTunnelDraw, DoubleTunnelDraw_Jig)
    //REGISTER_MINEGE_DRAW_JIG(Tunnel, SingleTunnelDraw, SingleTunnelDraw_Jig)

	// 弧线巷道
	REGISTER_MINEGE_DRAW( ArcTunnel, DoubleArcTunnelDraw )

    // 掘进工作面
    REGISTER_MINEGE_DRAW( TTunnel, DoubleTTunnelDraw )
    REGISTER_MINEGE_DRAW( TTunnel, PolyLineTTunnelDraw )

	//风流方向
    REGISTER_MINEGE_DRAW( WindDirection, PolyLineDirectionDraw )
    REGISTER_MINEGE_DRAW( WindDirection, SimpelDirectionDraw )

    // 风机
    REGISTER_MINEGE_DRAW( LocalFan, SimpleLocalFanDraw )
	REGISTER_MINEGE_DRAW( FanTagGE, FanTagGEDraw )

	//节点
	REGISTER_MINEGE_DRAW( Joint, SimpleJointDraw )


    // 风筒
    REGISTER_MINEGE_DRAW( Chimney, SimpleChimneyDraw )

	// 风库
	REGISTER_MINEGE_DRAW( WindLibrary, SimpleWindLibraryDraw )
	
	//风筒出入口风量
	REGISTER_MINEGE_DRAW( QTagGE, QTagGEDraw )

	REGISTER_MINEGE_DRAW( EffectRanTagGE, EffectRanTagGEDraw )

    //REGISTER_MINEGE_DRAW(Shaft, DoubleShaftDraw)

    // 指北针
    //REGISTER_MINEGE_DRAW(Compass, CompassBlockDraw)

	//瓦斯抽采管路
	REGISTER_MINEGE_DRAW( GasTube, GasTubeGEDraw )

	//瓦斯泵
	REGISTER_MINEGE_DRAW( GasPumpGE, GasPumpGEDraw )

	//阀
	REGISTER_MINEGE_DRAW( ValveGE, ValveGEDraw )

	//放水器
	REGISTER_MINEGE_DRAW( TailraceGE, TailraceGEDraw )

	//流量计
	REGISTER_MINEGE_DRAW( FlowmeterGE, FlowmeterGEDraw )

	//阻火器
	REGISTER_MINEGE_DRAW( BackFireGE, BackFireGEDraw )

	//测定孔
	REGISTER_MINEGE_DRAW( DetermineHoleGE, DetermineHoleGEDraw )

	//瓦斯流量
	REGISTER_MINEGE_DRAW( GasFlowTagGE , GasFlowTagGEDraw )

	//钻孔
	REGISTER_MINEGE_DRAW( DrillGE, DrillGEDraw );

	//温度传感器
	REGISTER_MINEGE_DRAW( TempeSensorGE, TempeSensorGEDraw );

	//流量传感器
	REGISTER_MINEGE_DRAW( FlowSensorGE, FlowSensorGEDraw );

	//压差传感器
	REGISTER_MINEGE_DRAW( DifferPressSensorGE, DifferPressSensorGEDraw );

	//瓦斯传感器
	REGISTER_MINEGE_DRAW( GasSensorGE, GasSensorGEDraw );

	//钻孔
	REGISTER_MINEGE_DRAW( Pore, SimplePoreDraw );
	//钻场
	REGISTER_MINEGE_DRAW( DrillSite, SimpleDrillSiteDraw );
	//煤层面
	REGISTER_MINEGE_DRAW( CoalSurface, SimpleCoalSurfaceDraw );
	//石门
	REGISTER_MINEGE_DRAW( RockGate, SimpleRockGateDraw );

    END_REGISTER_MINEGE_DRAW
}

void DefGEPlugin::unInitialize( MineGEDrawManager* pGEDrawManager )
{
    BEGIN_UNREGISTER_MINEGE_DRAW( MineGEDrawManager, pGEDrawManager )
    // 巷道
    UNREGISTER_MINEGE_DRAW( Tunnel, DoubleTunnelDraw )
    UNREGISTER_MINEGE_DRAW( Tunnel, PolyLineTunnelDraw )
    UNREGISTER_MINEGE_DRAW( Tunnel, SingleTunnelDraw )

	// 弧线巷道
	UNREGISTER_MINEGE_DRAW( ArcTunnel, DoubleArcTunnelDraw )

    // 掘进工作面
    UNREGISTER_MINEGE_DRAW( TTunnel, DoubleTTunnelDraw )
    UNREGISTER_MINEGE_DRAW( TTunnel, PolyLineTTunnelDraw )

    // 风流方向
    UNREGISTER_MINEGE_DRAW( WindDirection, PolyLineDirectionDraw )
    UNREGISTER_MINEGE_DRAW( WindDirection, SimpelDirectionDraw )

    // 风机
    UNREGISTER_MINEGE_DRAW( LocalFan, SimpleLocalFanDraw )
	UNREGISTER_MINEGE_DRAW( FanTagGE, FanTagGEDraw )

	// 节点
	UNREGISTER_MINEGE_DRAW( Joint, SimpleJointDraw )

    // 风筒
    UNREGISTER_MINEGE_DRAW( Chimney, SimpleChimneyDraw )

	// 风库
	UNREGISTER_MINEGE_DRAW( WindLibrary, SimpleWindLibraryDraw )

	//风筒出入口风量
	UNREGISTER_MINEGE_DRAW( QTagGE, QTagGEDraw )

	UNREGISTER_MINEGE_DRAW( EffectRanTagGE, EffectRanTagGEDraw )

    //UNREGISTER_MINEGE_DRAW(Shaft, DoubleShaftDraw)

    // 指北针
    //UNREGISTER_MINEGE_DRAW(Compass, CompassBlockDraw)

	//瓦斯抽采管路
	UNREGISTER_MINEGE_DRAW( GasTube, GasTubeGEDraw )

	//瓦斯泵
	UNREGISTER_MINEGE_DRAW( GasPumpGE, GasPumpGEDraw )

	//阀
	UNREGISTER_MINEGE_DRAW( ValveGE, ValveGEDraw )

	//放水器
	UNREGISTER_MINEGE_DRAW( TailraceGE, TailraceGEDraw )

	//流量计
	UNREGISTER_MINEGE_DRAW( FlowmeterGE, FlowmeterGEDraw )

	//阻火器
	UNREGISTER_MINEGE_DRAW( BackFireGE, BackFireGEDraw )

	//测定孔
	UNREGISTER_MINEGE_DRAW( DetermineHoleGE, DetermineHoleGEDraw )

	//测定孔
	UNREGISTER_MINEGE_DRAW( GasFlowTagGE, GasFlowTagGEDraw )

	//钻孔
	UNREGISTER_MINEGE_DRAW( DrillGE, DrillGEDraw );

	//温度传感器
	UNREGISTER_MINEGE_DRAW( TempeSensorGE, TempeSensorGEDraw );

	//流量传感器
	UNREGISTER_MINEGE_DRAW( FlowSensorGE, FlowSensorGEDraw );

	//压差传感器
	UNREGISTER_MINEGE_DRAW( DifferPressSensorGE, DifferPressSensorGEDraw );

	//瓦斯传感器
	UNREGISTER_MINEGE_DRAW( GasSensorGE, GasSensorGEDraw );

	//钻孔
	UNREGISTER_MINEGE_DRAW( Pore, SimplePoreDraw );
	//钻场
	UNREGISTER_MINEGE_DRAW( DrillSite, SimpleDrillSiteDraw );
	//煤层面
	UNREGISTER_MINEGE_DRAW( CoalSurface, SimpleCoalSurfaceDraw );
	//石门
	UNREGISTER_MINEGE_DRAW( RockGate, SimpleRockGateDraw );
	
    END_UNREGISTER_MINEGE_DRAW
}

// 插件创建函数定义实现并导出
MINEGE_PLUGIN_CREATE_FUNC_IMPL( DefGEPlugin )

//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(Joint, CircularJointDraw, CircularJointDraw_Jig)

//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(Tunnel, SingleTunnelDraw, SingleTunnelDraw_Jig)
//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(Tunnel, DoubleTunnelDraw, DoubleTunnelDraw_Jig)
//
//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(ArcTunnel, DoubleArcTunnelDraw, SingleArcTunnelDraw_Jig)
//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(ArcTunnel, DoubleArcTunnelDraw, DoubleArcTunnelDraw_Jig)
//
//MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(WorkSurface, DoubleWorkSurfaceDraw, DoubleWorkSurfaceDraw_Jig)

// draw创建函数实现宏(删除了，因为还不能确定draw的构造方式，目前的draw是通过"名称+ARX RTTI"来创建的)
