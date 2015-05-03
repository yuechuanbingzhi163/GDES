#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

class DEFGE_EXPORT_API WindLibrary : public DirectionTagGE
{
public:
	ACRX_DECLARE_MEMBERS(WindLibrary) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	WindLibrary();
	WindLibrary(const AcGePoint3d& insertPt, double angle);
};

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(WindLibrary)
#endif
