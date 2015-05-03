#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// 风流方向
class DEFGE_EXPORT_API BackFireGE : public FacilityGE {

public:
	ACRX_DECLARE_MEMBERS(BackFireGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	BackFireGE() ;
	BackFireGE(const AcGePoint3d& insertPt, double angle);
	virtual ~BackFireGE();
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(BackFireGE)
#endif