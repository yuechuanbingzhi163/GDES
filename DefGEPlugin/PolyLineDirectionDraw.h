#pragma once

#include "../MineGEDraw/MineGEDraw.h"

class PolyLineDirectionDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(PolyLineDirectionDraw);

public:
	PolyLineDirectionDraw();
	~PolyLineDirectionDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);
	
	virtual void regPropertyDataNames(AcStringArray& names) const;
	virtual void readPropertyDataFromGE(const AcStringArray& values);

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

	virtual Acad::ErrorStatus subGetGripPoints( AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds ) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

private:
	// key param
	AcGePoint3d m_insertPt;   // 插入点坐标
	double m_angle;           // 方向角度（逆时针，单位：度）

	// extra param
	double m_length;          // 箭头主干一半长度(默认为60)
	double m_arrow_length;    // 箭头分支长度(默认为30)
	double m_arrow_width;     // 箭头宽度(默认10)
	double m_radius;          // 绘制回风标记的正弦曲线(2个半圆)的半径

	int m_vt;                 // 通风类型
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PolyLineDirectionDraw)
#endif