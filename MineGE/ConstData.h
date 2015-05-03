#pragma once

// 预定义一些图元的属性数据常量(枚举)

/*******************************/
/****** 巷道(Tunnel)相关*******/
/*******************************/

// 材质类型
enum MATERIAL_TYPE
{
	MT_RIGID     = 0,   // 刚性风筒(默认)
	MT_FLEXIBLE  = 1,   // 柔性风筒
};

// 通风类型
// 如果没有指定，默认为进风
enum VENT_TYPE
{
	VT_IN       = 0,   // 进风
	VT_OUT      = 1,   // 回风
};

//通风方法
enum VENT_METHOD
{
	VP_PUSH    = 0,        // 压入式(默认)
	VP_PULL    = 1		  // 抽出式
};

// 通风方式
enum VENT_WAY
{
	VENT_AUXI_RAM		= 0,	//辅助风筒
	VENT_DIST_RAM		= 1,	//间断风筒
	VENT_SIG_DOUB		= 2,	//单机双列
	VENT_LOCAL_SERIER	= 3,	//串联风筒
	VENT_HYBRID_VNT		= 4,	//混合通风
	VENT_WIND_CABT		= 5,	//风柜通风
	VENT_WIND_LIB		= 6,	//风库通风
	VENT_DRILL_VENT		= 7		//钻孔通风
};
