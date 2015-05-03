#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// 风流方向
class DEFGE_EXPORT_API WindDirection : public DirectionTagGE {

public:
	ACRX_DECLARE_MEMBERS(WindDirection) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	WindDirection() ;
	WindDirection(const AcGePoint3d& insertPt, double angle);
	virtual ~WindDirection();
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(WindDirection)
#endif