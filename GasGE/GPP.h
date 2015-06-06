#pragma once

#include "PointGE.h"

// ÍßË¹Ñ¹Á¦µã
// GPP ==> Gas Pressure Point
class GASGE_EXPORT_API GPP : public PointGE
{
public:
	ACRX_DECLARE_MEMBERS(GPP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	GPP();
	GPP(const AcGePoint3d& insertPt);
};

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GPP)
#endif
