#pragma once

#include "../MineGEDraw/LinkedGEDraw.h"
#include "JointDrawType.h"

class DoubleTunnelDraw : public LinkedGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(DoubleTunnelDraw);

public:
	DoubleTunnelDraw();

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();	

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts);

	virtual void dealWithStartPointBoundary(const AcGeRay3d& boundaryLine);
	virtual void dealWithEndPointBoundary(const AcGeRay3d& boundaryLine);

	virtual void reverse();
	virtual void extendByLength(double length);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);
	
	// 变换操作(移动、选择、镜像)--目前暂不考虑"镜像"操作
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

public:
	void update();
	void caclStartPoint(AcGePoint3d& startPt, AcGePoint3d& endPt);
	void caclEndPoint(AcGePoint3d& startPt, AcGePoint3d& endPt);

	//double getLenth();
	//void setLenth(double lenth);

	// extra param
	double m_width; // 默认为30
	AcGePoint3d m_leftStartPt, m_leftEndPt;
	AcGePoint3d m_rightStartPt, m_rightEndPt;
	JointDrawType jdt;
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleTunnelDraw)
#endif