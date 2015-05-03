#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// ÎÂ¶È´«¸ÐÆ÷
class DEFGE_EXPORT_API TempeSensorGE : public DirectionTagGE {

public:
	ACRX_DECLARE_MEMBERS(TempeSensorGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	TempeSensorGE() ;
	TempeSensorGE(const AcGePoint3d& insertPt, double angle);
	virtual ~TempeSensorGE();

private:
	virtual void writePropertyDataToGEDraw( MineGEDraw* pGEDraw ) const;
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TempeSensorGE)
#endif