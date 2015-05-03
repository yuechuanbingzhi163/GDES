#pragma once

#include "../MineGEDraw/MineGEDraw.h"

class FanTagGEDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(FanTagGEDraw);

public:
	FanTagGEDraw();
	~FanTagGEDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

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
	AcGePoint3d m_insertPt; // 插入点坐标
	CString m_name;	//风机型号
	double m_q;	//风机工作风量
	double m_h; //工作风压
	CString m_way; //风机工作方式

	// extra param
	double m_distance;
	AcGePoint3d m_pt1,m_pt2;//绘制文字的线始末节点

private:
	void drawText(AcGiWorldDraw *mode);
	void drawFanTag(AcGiWorldDraw* mode);
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(FanTagGEDraw)
#endif
