#pragma once

#include "../MineGE/LinkedGE.h"
#include "dlimexp.h"

// 瓦斯抽采管路图元
class DEFGE_EXPORT_API GasTube : public LinkedGE 
{
public:
	ACRX_DECLARE_MEMBERS(GasTube) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	GasTube();
	GasTube(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
	virtual ~GasTube();

} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GasTube)
#endif
