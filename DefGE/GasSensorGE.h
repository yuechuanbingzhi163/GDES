#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// ÍßË¹´«¸ÐÆ÷
class DEFGE_EXPORT_API GasSensorGE : public DirectionTagGE {

public:
	ACRX_DECLARE_MEMBERS(GasSensorGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	GasSensorGE() ;
	GasSensorGE(const AcGePoint3d& insertPt, double angle);
	virtual ~GasSensorGE();

private:
	virtual void writePropertyDataToGEDraw( MineGEDraw* pGEDraw ) const;
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GasSensorGE)
#endif