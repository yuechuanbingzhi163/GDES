#pragma once

#include "../MineGEDraw/MineGEDraw.h"

class GasFlowTagGEDraw : public MineGEDraw {

public:
	ACRX_DECLARE_MEMBERS(GasFlowTagGEDraw) ;

public:
	GasFlowTagGEDraw();
	~GasFlowTagGEDraw();
	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void regPropertyDataNames(AcStringArray& names) const;
	virtual void readPropertyDataFromGE(const AcStringArray& values);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

private:
	// key param
	AcGePoint3d m_insertPt;   // 插入点坐标
	double m_angle;           // 方向角度（逆时针，单位：度）

	// extra param
	double m_lenth;

	CString Q;
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GasFlowTagGEDraw)
#endif
