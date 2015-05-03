#pragma once

#include "../MineGE/LinkedGE.h"
#include "dlimexp.h"

// 瓦斯抽采管路图元
class DEFGE_EXPORT_API DrillGE : public LinkedGE 
{
public:
	ACRX_DECLARE_MEMBERS(DrillGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	DrillGE();
	DrillGE(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
	virtual ~DrillGE();
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DrillGE)
#endif
