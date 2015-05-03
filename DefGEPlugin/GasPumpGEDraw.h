//#pragma once
//
//#include "../MineGEDraw/LinkedGEDraw.h"
//
//class GasPumpGEDraw : public LinkedGEDraw
//{
//public:
//	ACRX_DECLARE_MEMBERS(GasPumpGEDraw);
//
//public:
//	GasPumpGEDraw();
//	~GasPumpGEDraw(void);
//
//	virtual void setAllExtraParamsToDefault();
//	virtual void configExtraParams();
//	virtual void updateExtraParams();
//
//	virtual void readKeyParam(DrawParamReader& reader);
//	virtual void writeKeyParam(DrawParamWriter& writer);
//
//	virtual void reverse();
//	virtual void extendByLength(double length);
//
//	virtual void regPropertyDataNames(AcStringArray& names) const;
//	virtual void readPropertyDataFromGE(const AcStringArray& values);
//
//	virtual void readExtraParam(DrawParamReader& reader);
//	virtual void writeExtraParam(DrawParamWriter& writer);
//
//	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts);
//
//
//protected:
//	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);
//
//	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);
//
//	virtual Acad::ErrorStatus subGetOsnapPoints (
//		AcDb::OsnapMode osnapMode,
//		Adesk::GsMarker gsSelectionMark,
//		const AcGePoint3d &pickPoint,
//		const AcGePoint3d &lastPoint,
//		const AcGeMatrix3d &viewXform,
//		AcGePoint3dArray &snapPoints,
//		AcDbIntArray &geomIds) const ;
//
//	virtual Acad::ErrorStatus subGetGripPoints( AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds ) const;
//
//	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );
//
//public:
//	void SetAngle(double angle);
//
//private:
//	// key param
//	//AcGePoint3d m_insertPt;   // 插入点坐标
//
//	// extra param
//	double m_radius;          // 瓦斯泵圆的半径
//	double m_angle;           // 方向角度（逆时针，单位：度）
//};
//
//#ifdef DEFGEPLUGIN_MODULE
//ACDB_REGISTER_OBJECT_ENTRY_AUTO(GasPumpGEDraw)
//#endif

#pragma once

#include "../MineGEDraw/LinkedGEDraw.h"
#include "JointDrawType.h"

class GasPumpGEDraw : public LinkedGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(GasPumpGEDraw);

public:
	GasPumpGEDraw();

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();	

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void regPropertyDataNames(AcStringArray& names) const;
	virtual void readPropertyDataFromGE(const AcStringArray& values);

	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts);

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

	// extra param
	//double m_width; // 默认为30
	//AcGePoint3d m_leftStartPt, m_leftEndPt;
	//AcGePoint3d m_rightStartPt, m_rightEndPt;
	JointDrawType jdt;
	double m_radius;
	double m_angle;
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GasPumpGEDraw)
#endif