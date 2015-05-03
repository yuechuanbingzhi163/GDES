#pragma once

// 将符号转变成字符串
#define SYMBOL_STRINGIZE(symbol) DO_SYMBOL_STRINGIZE(symbol)
#define DO_SYMBOL_STRINGIZE(symbol) _T(#symbol)

// 将2个符号连接起来
#define SYMBOL_JOIN2(symbol1, symbol2) DO_SYMBOL_JOIN2(symbol1, symbol2)
#define DO_SYMBOL_JOIN2(symbol1, symbol2) DO_SYMBOL_JOIN2_IMPL(symbol1, symbol2)
#define DO_SYMBOL_JOIN2_IMPL(symbol1, symbol2) symbol1##symbol2

// 将2个符号用分隔符连接起来
#define SYMBOL_JOIN2_WITH_DELIMITER(symbol1, symbol2, delimiter) SYMBOL_JOIN2(SYMBOL_JOIN2(symbol1, delimiter), symbol2)

// 将3个符号用分隔符连接起来
#define SYMBOL_JOIN3_WITH_DELIMITER(symbol1, symbol2, symbol3, delimiter) SYMBOL_JOIN2_WITH_DELIMITER(SYMBOL_JOIN2_WITH_DELIMITER(symbol1, symbol2, delimiter), symbol3, delimiter)

// 定义下划线作为分隔符
#define DELIMITER _

#define FUNCTION_EXPORT_PREFIX extern "C" _declspec(dllexport)

// 插件创建函数名称
#define MINEGE_PLUGIN_CREATE_FUNCTION_NAME CreateMineGEPlugin

//#define MINEGE_DRAW_CREATE_FUNCTION_MAIN_NAME Create

// 插件创建函数实现宏
#define MINEGE_PLUGIN_CREATE_FUNC_IMPL(MineGEPluginImpl) \
	FUNCTION_EXPORT_PREFIX MineGEPlugin* MINEGE_PLUGIN_CREATE_FUNCTION_NAME(void) \
{ \
	static MineGEPluginImpl plugin; \
	return &plugin; \
}

// Jig创建函数主要前缀名称
#define JIG_CREATE_FUNCTION_NAME CreateJig

// Jig创建函数实现宏
#define MINEGE_DRAW_JIG_CREATE_FUNC_IMPL(GEType, MineGEDrawImpl, MineGEDraw_Jig_Impl) \
	MineGEDraw_Jig* SYMBOL_JOIN2_WITH_DELIMITER(JIG_CREATE_FUNCTION_NAME, MineGEDraw_Jig_Impl, DELIMITER)(void) \
{ \
	static MineGEDraw_Jig_Impl jig(SYMBOL_STRINGIZE(GEType), SYMBOL_STRINGIZE(MineGEDrawImpl)); \
	return &jig; \
}

// 绘制效果类注册宏--开始
#define BEGIN_REGISTER_MINEGE_DRAW(MineGEDrawManagerType, MineGEDrawManager_Instance) \
{ \
	MineGEDrawManagerType* pTempMineGEDrawManager = MineGEDrawManager_Instance; \

// 绘制效果类注册宏--注册
// 只是验证是否MineGEDraw类型(及其子类)
// 并未验证MineGEDraw的有效性(是否与MineGE匹配)
// *****有待修复****!!!!
#define REGISTER_MINEGE_DRAW(GEType, MineGEDrawImpl) \
	{ \
	AcRxObject* pObj = acrxClassDictionary->at(SYMBOL_STRINGIZE(MineGEDrawImpl)); \
	if(pObj != 0) \
		{ \
		AcRxClass* pClass = AcRxClass::cast(pObj); \
		MineGEDraw* pGEDraw = MineGEDraw::cast(pClass->create()); \
		if(pGEDraw != 0) \
			{ \
			pTempMineGEDrawManager->regGEDraw(SYMBOL_STRINGIZE(GEType), pGEDraw, 0); \
			} \
		} \
	}

// 绘制效果类注册宏--注册（包含jig功能）
#define REGISTER_MINEGE_DRAW_JIG(GEType, MineGEDrawImpl, MineGEDraw_Jig_Impl) \
	{ \
	    AcRxObject* pObj = acrxClassDictionary->at(SYMBOL_STRINGIZE(MineGEDrawImpl)); \
	    if(pObj != 0) \
	    { \
	        AcRxClass* pClass = AcRxClass::cast(pObj); \
			MineGEDraw* pGEDraw = MineGEDraw::cast(pClass->create()); \
			if(pGEDraw != 0) \
	        { \
			extern MineGEDraw_Jig* SYMBOL_JOIN2_WITH_DELIMITER(JIG_CREATE_FUNCTION_NAME, MineGEDraw_Jig_Impl, DELIMITER)(void); \
			MineGEDraw_Jig* pJig = SYMBOL_JOIN2_WITH_DELIMITER(JIG_CREATE_FUNCTION_NAME, MineGEDraw_Jig_Impl, DELIMITER)(); \
			pTempMineGEDrawManager->regGEDraw(SYMBOL_STRINGIZE(GEType), pGEDraw, pJig); \
            } \
     	} \
	}

// 绘制效果类注册宏--结束
#define END_REGISTER_MINEGE_DRAW }

// 绘制效果类反注册宏--开始
#define BEGIN_UNREGISTER_MINEGE_DRAW(MineGEDrawManagerType, MineGEDrawManager_Instance) \
{ \
	MineGEDrawManagerType* pTempMineGEDrawManager = MineGEDrawManager_Instance; \

// 绘制效果类反注册宏--反注册
#define UNREGISTER_MINEGE_DRAW(GEType, MineGEDrawImpl) \
	pTempMineGEDrawManager->unRegGEDraw(SYMBOL_STRINGIZE(GEType), SYMBOL_STRINGIZE(MineGEDrawImpl));

// 绘制效果类反注册宏--结束
#define END_UNREGISTER_MINEGE_DRAW }