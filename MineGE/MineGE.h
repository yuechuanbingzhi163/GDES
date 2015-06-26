#pragma once

#include "dlimexp.h"

#include "../MineGEDraw/MineGEDraw.h"
#include "../ArxDbgXdata/XDataStream.h"

// 矿井系统图元基类(抽象类)
class MINEGE_DLLIMPEXP MineGE : public AcDbEntity 
{
public:
	ACRX_DECLARE_MEMBERS(MineGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

	// 公共接口
public:
	virtual ~MineGE ();	

	// 获取类型名称
	CString getTypeName() const;

	// 当全局配置extra param的参数后，更新图元的参数
	void configDraw(const CString& drawName);

	// 更新图元可视化效果
	void updateDraw();

	// 获取数据对象id
	AcDbObjectId getDataObject() const;

	//启用/关闭跟随效果
	void enableFollow(bool bFollow);

	// 必须要重载实现的虚函数
protected:
	// 将关键参数写入到writer中
	virtual void writeKeyParam(DrawParamWriter& writer) const = 0;

	// 从reader中读取关键参数
	virtual void readKeyParam(DrawParamReader& reader) = 0;

	// AcDbObject重载函数
public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

	// AcDbEntity的重载函数
protected:
	// 图元绘制
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	// 变换操作(移动、选择、镜像)--目前暂不考虑"镜像"操作
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	// 获取捕捉点
	virtual Acad::ErrorStatus subGetOsnapPoints(
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	// 获取夹点
	virtual Acad::ErrorStatus subGetGripPoints(
		AcGePoint3dArray &gripPoints, 
		AcDbIntArray &osnapModes, 
		AcDbIntArray &geomIds) const ;

	// 夹点编辑行为
	virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray &indices, const AcGeVector3d &offset) ;

	// 包围盒重载(用于确定缩放的范围)
	// 一个"紧凑"的3d包围盒(立方体)
	virtual Acad::ErrorStatus subGetGeomExtents(AcDbExtents& extents) const;

	virtual Acad::ErrorStatus subClose(void);

protected:
	// 构造函数
	MineGE();

	// 更新参数(辅助方法)
	// true  -- 将参数写入到draw中
	// false -- 将参数读取回来到ge中
	// 与writeParamToGEDraw和readParamFromGEDraw方法在功能上重复(***)
	void updateDrawParam(bool readOrWrite = true) const;

	// 获取当前draw对象(方便子类使用，避免直接操纵私有数据)
	MineGEDraw* getCurrentDraw() const;

	//变换所有的关联图元
	void transformAllTagGE(const AcGeMatrix3d& xform);

private:
	// 绘制背景消隐
	void drawBackground(MineGEDraw* pGEDraw, AcGiWorldDraw *mode);

	// 读写关键参数
	void readKeyParamFromGEDraw(MineGEDraw* pGEDraw);
	void writeKeyParamToGEDraw(MineGEDraw* pGEDraw) const;

	// 读写绘制图元附加参数
	void readExtraParamFromGEDraw(MineGEDraw* pGEDraw);
	void writeExtraParamToGEDraw(MineGEDraw* pGEDraw) const;	

	// 读写参数(关键参数和附加参数)-- 辅助方法
	void readParamFromGEDraw(MineGEDraw* pGEDraw);
	void writeParamToGEDraw(MineGEDraw* pGEDraw) const;

	// 初始化所有的附加参数到xdata中
	void initAllExtraParamsToXData();

	// 提取已存在的draw
	void extractExistedDraw(AcStringArray& existedDraw);

	// 初始化可视化效果
	void initDraw();

	// 初始化属性数据
	void initPropertyData();

	void updateDrawParams(MineGEDraw* pGEDraw);

	void updateCurrentDraw();

protected:
	// 写入属性数据到MineGEDraw中
	virtual void writePropertyDataToGEDraw(MineGEDraw* pGEDraw) const;

private:
	// 当前绘制效果指针
	// 图元的图形效果都是通过该指针委托实现
	MineGEDraw* m_pCurrentGEDraw;
	// 图元关联的数据对象
	AcDbObjectId m_dataObjectId;
	//是否实现跟随效果(关联图元TagGE跟随移动,默认是禁用的!)
	bool m_bFollow;
};

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MineGE)
#endif