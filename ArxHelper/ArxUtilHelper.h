#pragma once

#include "dlimexp.h"

// ARX工具类
class ARXHELPER_DLLIMPEXP ArxUtilHelper
{
public:
	// 将新建的图元添加到CAD图形数据库
	static bool PostToModelSpace(AcDbEntity* pEnt);

	// 命令交互选择一个实体
	static AcDbObjectId SelectObject(const CString& msg);

	// 获取当前已选择的图元(PickSet)
	static void GetPickSetEntity(AcDbObjectIdArray& objIds);
	static void GetPickSetEntity2(AcDbObjectIdArray& objIds);

	// 交互选择一个点坐标
	static bool PromptPt(const CString& msg, AcGePoint3d& pt);
	static bool PromptPt2(const CString& msg, const AcGePoint3d& basePt, AcGePoint3d& pt);

	// 计算向量v与x轴逆时针旋转的角度
	static double AngleToXAxis(const AcGeVector3d& v);

	// 暂停
	static void Pause(const CString& msg=_T("\n请按回车或空格键结束..."));

	// 是否指定类型的图元
	static bool IsEqualType(const CString& type, const AcDbObjectId& objId, bool isDerivedFromParent=true);
	
	// 临时用一种颜色显示一个图元
	// 会话结束后，恢复原有颜色
	static void ShowEntityWithColor(const AcDbObjectId& objId, unsigned short colorIndex);

	// 临时用一种颜色显示多个图元
	// 会话结束后，恢复原有颜色
	static void ShowEntitiesWithColor(AcDbObjectIdArray& objIds, unsigned short colorIndex);

	// 临时用多种颜色显示多个图元
	// 会话结束后，恢复原有颜色
	static void ShowEntitiesWithColor2(AcDbObjectIdArray& objIds, const AcArray<Adesk::UInt16>& colors);

	// 命令行交互遍历多个图元
	static void BrowserEntities(const AcDbObjectIdArray& objIds);

	// 调整角度(正值，[0, 2*PI])
	// 可以输入任意大小的角度(不论正负，输入值可以超出[0, 2*PI]区间范围)
	static double AdjustAngle(double angle);

	// string-->int, double之间的转换
	static bool StringToDouble(const CString& value, double& v);
	static bool StringToInt(const CString& value, int& v);
	static void DoubleToString(double v, CString& value);
	static void IntToString(int v, CString& value);
	
	// 判断字符串是否是一个合法的整数串
	// 格式[+/-][0-9]
	static bool IsInteger(LPCTSTR pSrc);
	
	// 判断字符串是否是一个合法的浮点数串
	// 格式[+/-][0-9][.][0-9]	
	static bool IsNumeric(LPCTSTR pSrc);

	// 生成GUID字符串
	static bool NewGUID(CString& strGUID);

	//获取模块运行路径
	static CString GetAppPathDir(HINSTANCE hInstance);

	//构造路径
	static CString BuildPath( const CString& dir, const CString& fileName );

	//cstring <--> acgepoint3d之间的转换
	static CString Point3dToString(const AcGePoint3d& pt);
	static bool StringToPoint3d(const CString& value, AcGePoint3d& pt);

	static CString Vector3dToString(const AcGeVector3d& v);
	static bool StringToVector3d(const CString& value, AcGeVector3d& v);

	//分隔字符串
	// tokens表示分隔符(可以有多个分隔符)
	static void SplitCString(const CString& str, const CString& tokens, CStringArray& values);
};
