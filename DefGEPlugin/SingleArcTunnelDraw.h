#pragma once

#include "../MineGEDraw/LinkedGEDraw.h"

// 多段线弧线巷道(效果不是很好)
class DoubleArcTunnelDraw : public LinkedGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(DoubleArcTunnelDraw);

public:
	DoubleArcTunnelDraw();
	~DoubleArcTunnelDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual AcGeVector3d getStartPointInExtendAngle() const;
	virtual AcGeVector3d getEndPointInExtendAngle() const;

	virtual void extendByLength(double length);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	Acad::ErrorStatus subGetOsnapPoints( 
		AcDb::OsnapMode osnapMode, 
		int gsSelectionMark, 
		const AcGePoint3d &pickPoint, 
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds ) const;
	virtual Acad::ErrorStatus subGetGripPoints( AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds ) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

public:
	// key param
	AcGePoint3d m_thirdPt;

	// 附加参数
	double m_width;
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleArcTunnelDraw)
#endif