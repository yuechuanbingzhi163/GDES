#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"
#include "../DefGEPlugin/GasFlowTagGEDraw.h"

class DEFGE_EXPORT_API GasFlowTagGE : public FacilityGE
{
public:
	ACRX_DECLARE_MEMBERS(GasFlowTagGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	GasFlowTagGE();
	GasFlowTagGE(const AcGePoint3d& insertPt, double angle);

private:
	virtual void writePropertyDataToGEDraw( MineGEDraw* pGEDraw ) const;
};

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GasFlowTagGE)
#endif
