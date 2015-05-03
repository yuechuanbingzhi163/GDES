#pragma once

#include "../MineGEDraw/LinkedGEDraw.h"

// 双弧线巷道效果
class DoubleArcTunnelDraw : public LinkedGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(DoubleArcTunnelDraw);

public:
	DoubleArcTunnelDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts);

	virtual AcGeVector3d getStartPointInExtendAngle() const;
	virtual AcGeVector3d getEndPointInExtendAngle() const;

	virtual void dealWithStartPointBoundary(const AcGeRay3d& boundaryLine);
	virtual void dealWithEndPointBoundary(const AcGeRay3d& boundaryLine);

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

public:
	void caclInPoint(AcGePoint3d& startPt, AcGePoint3d& endPt);
	void caclOutPoint(AcGePoint3d& startPt, AcGePoint3d& endPt);
	void drawArc(AcGiWorldDraw *mode, AcGePoint3d& startPt, AcGePoint3d& endPt, AcGePoint3d& thirdPt);

	void update();

	// key param
	AcGePoint3d m_thirdPt;

	// extra param
	double m_width; // 默认为30

	AcGePoint3d m_inStartPt, m_inEndPt;
	AcGePoint3d m_outStartPt, m_outEndPt;

};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleArcTunnelDraw)
#endif