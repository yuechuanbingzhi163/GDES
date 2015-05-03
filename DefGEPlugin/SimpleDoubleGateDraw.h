#pragma once

#include "SimpleGateDraw.h"

// Ë«Ïò·çÃÅ
class SimpleDoubleGateDraw : public SimpleGateDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleDoubleGateDraw) ;

public:
	SimpleDoubleGateDraw () ;
	virtual ~SimpleDoubleGateDraw () ;

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();	
	virtual void updateExtraParams();

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

	virtual void caclBackGroundMinPolygon(AcGePoint3dArray& pts);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleDoubleGateDraw)
#endif
