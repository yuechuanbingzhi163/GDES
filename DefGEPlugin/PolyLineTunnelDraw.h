#pragma once

#include "../MineGEDraw/LinkedGEDraw.h"
#include "JointDrawType.h"

class PolyLineTunnelDraw : public LinkedGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(PolyLineTunnelDraw);

public:
	PolyLineTunnelDraw();

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();	

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void extendByLength(double length);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);
	
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

	// 包围盒重载(用于确定缩放的范围)
	// 一个"紧凑"的3d包围盒(立方体)
	virtual Acad::ErrorStatus subGetGeomExtents(AcDbExtents& extents) const;

public:
	// extra param
	double m_width; // 默认为40
	JointDrawType jdt;
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PolyLineTunnelDraw)
#endif