#pragma once

// 数据类型枚举
enum DATA_TYPE
{
	DT_STRING   = 0, // 字符串
	DT_INT      = 1, // 整数
	DT_NUMERIC  = 2, // 浮点数
	DT_BOOL     = 3, // 布尔类型
	DT_DATE     = 4, // 日期类型
	DT_LIST     = 5  // 列表类型(特殊的离散类型)
};

// 列表数据类型
enum LIST_TYPE
{
	LT_STRING = 0, // 字符串列表
	LT_INT    = 1, // 整数列表
	LT_OBJECT = 2  // AcDbObject列表
};