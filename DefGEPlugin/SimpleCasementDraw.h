#pragma once

#include "../MineGEDraw/MineGEDraw.h"

// 调风窗可视化默认效果
class SimpleCasementDraw : public MineGEDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleCasementDraw) ;

public:
	SimpleCasementDraw () ;
	virtual ~SimpleCasementDraw () ;

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
	// key param
	AcGePoint3d m_insertPt;  // 插入点坐标
	double m_angle;         // 角度(逆时针）

	// extra param
	double m_radius;        // 风门的半径
	double m_length;        // 风门直线的一半长度（从中轴向外的距离）
	double m_width;         // 调节直线的一半宽度
	double m_linewidth;     // 调节直线的线宽(使用多段线绘制)
	double m_dTWidth;       // 所在双线巷道的宽度

	// 派生类根据需要添加到附加参数中
	double m_offset;
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleCasementDraw)
#endif
