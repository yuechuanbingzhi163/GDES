#pragma once

#include "../MineGEDraw/MineGEDraw.h"

class SimpleDirectionDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(SimpleDirectionDraw);

public:
	SimpleDirectionDraw();
	~SimpleDirectionDraw(void);

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
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subGetGripPoints( AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds ) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

private:
	AcGePoint3d caclArrowTailPt() const; // 计算箭头末端的点坐标
	void drawPollutionFlag(AcGiWorldDraw *mode); // 绘制污风标记(2个半圆)

private:
	// key param
	AcGePoint3d m_insertPt;   // 插入点坐标
	double m_angle;           // 方向角度（逆时针，单位：度）

	// extra param
	double m_length;          // 箭头主干长度(默认为180)
	double m_arrow_theta;     // 箭头夹角(默认为20度)
	double m_arrow_length;    // 箭头分支长度(默认为50)

	// 绘制两个半圆
	double m_radius;          // 污风标记正弦曲线(2个半圆)的半径

	/* 
	 * 数据
	 * enum VENT_TYPE
	 * {
	 * 	 VT_IN       = 0,   // 进风
	 *	 VT_OUT      = 1,   // 回风
	 * };
	 */
	int m_vt;                 // 通风类型
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleDirectionDraw)
#endif