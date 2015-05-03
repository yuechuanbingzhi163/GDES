#pragma once

#include "SimpleGateDraw.h"

// 平衡风门
class SimpleBalanceGateDraw : public SimpleGateDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleBalanceGateDraw) ;

public:
	SimpleBalanceGateDraw () ;
	virtual ~SimpleBalanceGateDraw () ;

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();	
	virtual void updateExtraParams();

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;

private:
	// 绘制抛物线(两个半圆)
	void drawParabola(AcGiWorldDraw *mode, const AcGePoint3d& insertPt, double angle, double gap, double radius, bool clockWise = true);
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleBalanceGateDraw)
#endif
