#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// 流量传感器
class DEFGE_EXPORT_API DifferPressSensorGE : public DirectionTagGE {

public:
	ACRX_DECLARE_MEMBERS(DifferPressSensorGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	DifferPressSensorGE() ;
	DifferPressSensorGE(const AcGePoint3d& insertPt, double angle);
	virtual ~DifferPressSensorGE();

private:
	virtual void writePropertyDataToGEDraw( MineGEDraw* pGEDraw ) const;
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DifferPressSensorGE)
#endif