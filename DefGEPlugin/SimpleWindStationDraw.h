#pragma once

#include "SimpleWallDraw.h"

// 测风站的效果与临时挡风墙相似
// 因此直接从SimpleWallDraw派生
class SimpleWindStationDraw : public SimpleWallDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleWindStationDraw) ;

public:
	SimpleWindStationDraw () ;
	virtual ~SimpleWindStationDraw () ;

	virtual void setAllExtraParamsToDefault();

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void regPropertyDataNames(AcStringArray& names) const;
	virtual void readPropertyDataFromGE(const AcStringArray& values);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

public:
	double m_lableAngle;  // 标注角度
	double m_lableLength; // 标注长度
	double m_hBaseLength;     // 标注文字的水平基长度(托住文字的水平直线)
	double m_textHeight;   // 文字高度

	CString S;
	CString V;
	CString Q;
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleWindStationDraw)
#endif
