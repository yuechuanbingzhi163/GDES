#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// 风流方向
class DEFGE_EXPORT_API ValveGE : public FacilityGE {

public:
	ACRX_DECLARE_MEMBERS(ValveGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	ValveGE() ;
	ValveGE(const AcGePoint3d& insertPt, double angle);
	virtual ~ValveGE();
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ValveGE)
#endif