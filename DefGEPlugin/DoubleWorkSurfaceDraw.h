#pragma once

#include "DoubleTunnelDraw.h"

// 简单的双线阴影工作面
class DoubleWorkSurfaceDraw : public DoubleTunnelDraw
{
public:
	ACRX_DECLARE_MEMBERS(DoubleWorkSurfaceDraw);

public:
	DoubleWorkSurfaceDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

public:
	void drawArrow(AcGiWorldDraw *mode, bool clockWise);
	void drawText(AcGiWorldDraw *mode);

	// 默认为AcDb::kLnWt060
	//AcDb::LineWeight m_oneSideLineWeight;
	bool m_clockWise;
	double m_trunkWidth;   // 主干半宽
	double m_trunkLength;  // 主干长度
	double m_arrowWidth;   // 箭头半宽多出的部分
	double m_arrowLength;  // 箭头长度
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleWorkSurfaceDraw)
#endif