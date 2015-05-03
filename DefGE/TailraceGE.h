#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// 风流方向
class DEFGE_EXPORT_API TailraceGE : public FacilityGE {

public:
	ACRX_DECLARE_MEMBERS(TailraceGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	TailraceGE() ;
	TailraceGE(const AcGePoint3d& insertPt, double angle);
	virtual ~TailraceGE();
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TailraceGE)
#endif