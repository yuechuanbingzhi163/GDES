#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// ∑Á«≈
class DEFGE_EXPORT_API WindBridge : public DirectionTagGE {

public:
	ACRX_DECLARE_MEMBERS(WindBridge) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	WindBridge();
	WindBridge(const AcGePoint3d& insertPt, double angle);
	virtual ~WindBridge();
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(WindBridge)
#endif
