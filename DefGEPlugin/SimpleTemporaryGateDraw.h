#pragma once

#include "SimpleGateDraw.h"

// ¡Ÿ ±∑Á√≈
class SimpleTemporaryGateDraw : public SimpleGateDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleTemporaryGateDraw) ;

public:
	SimpleTemporaryGateDraw () ;
	virtual ~SimpleTemporaryGateDraw () ;

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();	
	virtual void updateExtraParams();

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleTemporaryGateDraw)
#endif
