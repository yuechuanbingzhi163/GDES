#pragma once

#include "../MineGE/LinkedGE.h"
#include "dlimexp.h"

// ÍßË¹±Ã
class DEFGE_EXPORT_API GasPumpGE : public LinkedGE {

public:
	ACRX_DECLARE_MEMBERS(GasPumpGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	GasPumpGE() ;
	GasPumpGE(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
	virtual ~GasPumpGE();
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GasPumpGE)
#endif