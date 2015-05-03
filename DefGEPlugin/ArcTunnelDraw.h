#pragma once

#include "../MineGE/ConnectedGEDraw.h"

// 单弧线巷道
class ArcTunnelDraw : public ConnectedGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(ArcTunnelDraw);

public:
	ArcTunnelDraw();
	~ArcTunnelDraw(void);

	virtual void setAllParamsToDefault();

	virtual void config();

	virtual void readExtraParam(GEDrawParamReader& reader);

	virtual void writeExtraParam(GEDrawParamWriter& writer);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);
	Acad::ErrorStatus subGetOsnapPoints( 
		AcDb::OsnapMode osnapMode, 
		int gsSelectionMark, 
		const AcGePoint3d &pickPoint, 
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds ) const;
	virtual Acad::ErrorStatus subGetGripPoints( AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds ) const;
	virtual Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );

private:
	AcGePoint3d caclArcCenterPt() const; // 计算弧线的中点坐标
	// 弧率有正负之分，正值表示圆弧在左侧；负值表示圆弧在右侧
	double m_arcRatio;         // 圆弧弧率(默认为0.5)
};

#ifdef DEFGE_PLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ArcTunnelDraw)
#endif