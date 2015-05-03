#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// 风流方向
class DEFGE_EXPORT_API DetermineHoleGE : public FacilityGE {

public:
	ACRX_DECLARE_MEMBERS(DetermineHoleGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	DetermineHoleGE() ;
	DetermineHoleGE(const AcGePoint3d& insertPt, double angle);
	virtual ~DetermineHoleGE();
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DetermineHoleGE)
#endif