#pragma once

#include "PolyLineTunnelDraw.h"

class PolyLineWorkSurfaceDraw : public PolyLineTunnelDraw
{
public:
	ACRX_DECLARE_MEMBERS(PolyLineWorkSurfaceDraw);

public:
	PolyLineWorkSurfaceDraw(void);

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

	bool m_clockWise;
	double m_trunkWidth;   // 主干半宽
	double m_trunkLength;  // 主干长度
	double m_arrowWidth;   // 箭头半宽多出的部分
	double m_arrowLength;  // 箭头长度
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PolyLineWorkSurfaceDraw)
#endif