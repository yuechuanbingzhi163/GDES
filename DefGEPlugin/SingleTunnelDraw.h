#pragma once

#include "../MineGEDraw/LinkedGEDraw.h"

class SingleTunnelDraw : public LinkedGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(SingleTunnelDraw);

public:
	SingleTunnelDraw();
	~SingleTunnelDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();
	
	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void extendByLength(double length);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	// 变换操作(移动、选择、镜像)--目前暂不考虑"镜像"操作
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	//- Osnap points protocol
	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

	// 包围盒重载(用于确定缩放的范围)
	// 一个"紧凑"的3d包围盒(立方体)
	virtual Acad::ErrorStatus subGetGeomExtents(AcDbExtents& extents) const;

};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SingleTunnelDraw)
#endif