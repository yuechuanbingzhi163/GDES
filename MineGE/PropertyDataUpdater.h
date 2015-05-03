#pragma once

#include "dlimexp.h"

class CMFCPropertyGridCtrl;

// 在MFCPropertyGridCtrl<-->objId之间显示、编辑、更新属性数据
class MINEGE_DLLIMPEXP PropertyDataUpdater
{
public:
	// 根据类型名称以及字段初始化CMFCPropertyGridCtrl
	// 注：pPropDataList指向的内存必须有效!!!
	static bool BuildPropGridCtrl(CMFCPropertyGridCtrl* pPropDataList, const CString& type,const AcStringArray& funcFieldsInfo );

	// 读取数据到CMFCPropertyGridCtrl中
	// 前提：使用BuildPropGridCtrl()方法成功创建CMFCPropertyGridCtrl
	static bool ReadDataFromGE(CMFCPropertyGridCtrl* pPropDataList, const AcDbObjectId& objId);

	// 将CMFCPropertyGridCtrl中的数据更新到图元中
	// 前提：使用BuildPropGridCtrl()方法成功创建CMFCPropertyGridCtrl
	static bool WriteDataToGE(CMFCPropertyGridCtrl* pPropDataList, const AcDbObjectId& objId);
	
	//static void UpdateAllGasFlow();
};
