#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// 风流方向
class DEFGE_EXPORT_API FlowmeterGE : public FacilityGE {

public:
	ACRX_DECLARE_MEMBERS(FlowmeterGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	FlowmeterGE() ;
	FlowmeterGE(const AcGePoint3d& insertPt, double angle);
	virtual ~FlowmeterGE();
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(FlowmeterGE)
#endif