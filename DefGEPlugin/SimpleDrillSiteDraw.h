#pragma once

#include "../MineGEDraw/MineGEDraw.h"

//简单钻孔绘制效果
class SimpleDrillSiteDraw : public MineGEDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleDrillSiteDraw) ;

public:
	SimpleDrillSiteDraw () ;
	virtual ~SimpleDrillSiteDraw () ;

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
	virtual Acad::ErrorStatus subTransformBy( const AcGeMatrix3d & xform );
	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

public:
	//key param
	AcGePoint3d m_insertPt;     // 插入点坐标
	AcGePoint3d m_linkPt;       // 连接点坐标
	//data
	double m_width, m_height;   // 钻场宽度和高度
	CString m_name;				// 钻场编号
	double m_dist;              // 与迎头的距离
	int m_pos;                  // 位置
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleDrillSiteDraw)
#endif
