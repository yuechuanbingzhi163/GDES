#pragma once

#include "dlimexp.h"

// 图层管理辅助类
class ARXHELPER_DLLIMPEXP LayerHelper
{
public:
	static void SetCurrentLayer(const CString& layerName);  // 设置当前图层
	static CString GetCurrentLayerName();                   // 获取当前图层的名称
	static void AddLayer(const CString& layerName);         // 增加图层
	static void RemoveLayer(const CString& layerName);      // 删除图层
	static bool IsLayerExist(const CString& layerName);     // 判断图层是否存在
	static bool IsCurrentLayer(const CString& layerName);   // 判断图层是否当前图层
	static void RenameLayer(const CString& layerName, const CString& newLayerName); // 修改图层名称

	// 慎用!!!
	static bool FrozenLayer(const CString& layerName, bool isFrozen);      // 是否冻结图层
	static bool HideLayer(const CString& layerName, bool isOn);            // 是否隐藏图层(特殊)
	static bool LockLayer(const CString& layerName, bool locked);          // 是否锁定图层
	static bool OffLayer(const CString& layerName, bool off);              // 是否关闭图层

	// 获取图层列表
	static void GetAllLayers(AcStringArray& layers);

	// 获取图层上的所有实体
	static bool GetAllEntitiesOnLayer(const CString& layerName, AcDbObjectIdArray &ents);

	// 删除图层上的所有实体
	static void DeleteAllEntitiesOnLayer(const CString& layerName);
};

// 一个简单的图层切换类
// 当对象析构之后，自动将图层切换回来
// 如果createNewLayer参数为真，图层不存在则自动建立图层
class ARXHELPER_DLLIMPEXP LayerSwitch
{
public:
	LayerSwitch(const CString& layerName, bool createNewLayer=true);
	~LayerSwitch();

private:
	CString m_curLayer; // 记录当前图层
};