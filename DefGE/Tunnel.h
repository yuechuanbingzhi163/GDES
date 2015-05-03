#pragma once

#include "../MineGE/LinkedGE.h"
#include "dlimexp.h"

// ÏïµÀÍ¼Ôª
class DEFGE_EXPORT_API Tunnel : public LinkedGE 
{
public:
	ACRX_DECLARE_MEMBERS(Tunnel) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	Tunnel();
	Tunnel(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
	virtual ~Tunnel();
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Tunnel)
#endif
