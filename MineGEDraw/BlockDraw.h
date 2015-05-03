#pragma once

#include "MineGEDraw.h"

class MINEGE_DRAW_DLLIMPEXP BlockDraw : public MineGEDraw {

public:
	ACRX_DECLARE_MEMBERS(BlockDraw) ;

public:
	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

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
		AcDbIntArray &geomIds) const;

	virtual Acad::ErrorStatus subGetGripPoints( 
		AcGePoint3dArray &gripPoints, 
		AcDbIntArray &osnapModes, 
		AcDbIntArray &geomIds ) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts); // 默认实现，什么也不做

protected:
	BlockDraw();
	virtual ~BlockDraw();

public:
	// key param
	AcGePoint3d m_insertPt; // 插入点坐标
	double m_angle;         // 角度

	// extra param
	double m_scale;

	// extra param
	CString m_blockName;    // 块名称

	// 从图元提取的属性数据
	// 用户也可根据实际需求修改值
	AcStringArray m_attValues; // 属性数据

	void explodeBlock(AcGeVoidPointerArray& ents);
};

#ifdef MINEGEDRAW_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(BlockDraw)
#endif
