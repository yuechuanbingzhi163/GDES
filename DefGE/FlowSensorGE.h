#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// 流量传感器
class DEFGE_EXPORT_API FlowSensorGE : public DirectionTagGE {

public:
	ACRX_DECLARE_MEMBERS(FlowSensorGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	FlowSensorGE() ;
	FlowSensorGE(const AcGePoint3d& insertPt, double angle);
	virtual ~FlowSensorGE();

private:
	virtual void writePropertyDataToGEDraw( MineGEDraw* pGEDraw ) const;
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(FlowSensorGE)
#endif