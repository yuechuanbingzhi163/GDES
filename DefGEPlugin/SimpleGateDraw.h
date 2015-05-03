#pragma once

#include "../MineGEDraw/MineGEDraw.h"

// 风门可视化默认效果
class SimpleGateDraw : public MineGEDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleGateDraw) ;

public:
	SimpleGateDraw () ;
	virtual ~SimpleGateDraw () ;

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();	
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;

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

protected:
	void drawArc(AcGiWorldDraw *mode, const AcGePoint3d& insertPt, double angle, double gap, double radius, bool moreAngle = true);
	void drawLine(AcGiWorldDraw *mode, const AcGePoint3d& insertPt, double angle,  double gap, double offset, double length);
	
protected:
	// key param
	AcGePoint3d m_insertPt;  // 插入点坐标
	double m_angle;         // 角度(逆时针）

	// extra param
	double m_radius;        // 风门的半径
	double m_length;        // 风门直线的一半长度（从中轴向外的距离）

	// 派生类根据需要添加到附加参数中
	double m_offset;
	double m_gap;

	double m_dTWidth;  // 所在双线巷道的宽度

} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleGateDraw)
#endif
