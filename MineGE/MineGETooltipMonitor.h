#pragma once

#include "acedinpt.h"

/*
 * 注：所有的Reactor可以不使用arx宏
 * 用于修改鼠标悬停在图元上方的时候弹出的tooltip
 * 由于它是和AcDocument关联的，因此需要在On_kLoadDwgMsg 
 * 和 On_kUnloadDwgMsg消息中的初始化和卸载
 */
class MineGETooltipMonitor : public AcEdInputPointMonitor 
{
protected:
	AcApDocument *mpDocument ;

public:
	MineGETooltipMonitor (AcApDocument *pDoc =NULL) ;
	virtual ~MineGETooltipMonitor () ;

	virtual void Attach (AcApDocument *pDoc) ;
	virtual void Detach () ;
	virtual AcApDocument *Subject () const ;
	virtual bool IsAttached () const ;

	virtual bool excludeFromOsnapCalculation (
		const AcArray<AcDbObjectId> &nestedEntity,
		Adesk::GsMarker gsSelectionMark
	) ;

	virtual Acad::ErrorStatus monitorInputPoint (
		bool &appendToTooltipStr,
		ACHAR *&additionalTooltipString,
		AcGiViewportDraw *drawContext,
		AcApDocument *document,
		bool pointComputed,
		int history,
		const AcGePoint3d &lastPoint,
		const AcGePoint3d &rawPoint,
		const AcGePoint3d &grippedPoint,
		const AcGePoint3d &cartesianSnappedPoint,
		const AcGePoint3d &osnappedPoint,
		AcDb::OsnapMask osnapMask,
		const AcArray<AcDbCustomOsnapMode *> &customOsnapModes,
		AcDb::OsnapMask osnapOverrides,
		const AcArray<AcDbCustomOsnapMode *> &customOsnapOverrides,
		const AcArray<AcDbObjectId> &apertureEntities,
		const AcArray<AcDbObjectIdArray, AcArrayObjectCopyReallocator<AcDbObjectIdArray> > &nestedApertureEntities,
		const AcArray<Adesk::GsMarker> &gsSelectionMark,
		const AcArray<AcDbObjectId> &keyPointEntities,
		const AcArray<AcDbObjectIdArray, AcArrayObjectCopyReallocator<AcDbObjectIdArray> > &nestedKeyPointEntities,
		const AcArray<Adesk::GsMarker> &keyPointGsSelectionMark,
		const AcArray<AcGeCurve3d *> &alignmentPaths,
		const AcGePoint3d &computedPoint,
		const ACHAR *tooltipString
	) ;
} ;
