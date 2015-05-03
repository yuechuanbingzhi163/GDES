#pragma once

#include "../MineGEDraw/MineGEDraw.h"

// 双线风桥可视化效果
class DoubleWindBridgeDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(DoubleWindBridgeDraw);

public:
	DoubleWindBridgeDraw();

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

public:
	// key param
	AcGePoint3d m_insertPt;   // 插入点坐标
	double m_angle;           // 方向角度（逆时针，单位：度）

	// 除了双线巷道的附加参数之外
	// 额外增加绘制风桥需要的其它参数
	double m_width;
	double m_sideLength;  // 侧边长度
	double m_sideLength2; // 带倾角的侧边长度
	double m_sideAngle;   // 侧边倾角(以侧边为基准方向)
	double m_sideOffset;  // 侧边偏移(以巷道的轮廓为基准)
	double m_linewidth; // 线宽

public:
	AcGePoint3d caclSidePt(const AcGePoint3d& pt, const AcGeVector3d& baseV, bool clockwise) const;
	void drawSide(AcGiWorldDraw *mode);
	void drawOneSide(AcGiWorldDraw *mode, const AcGeVector3d& v, const AcGePoint3d& pt, bool clockwise);
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleWindBridgeDraw)
#endif