#include "StdAfx.h"
#include "ValveGE.h"

Adesk::UInt32 ValveGE::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS ( ValveGE, 
FacilityGE, AcDb::kDHL_CURRENT,
AcDb::kMReleaseCurrent,AcDbProxyEntity::kNoOperation,
∑ß√≈, DEFGEAPP )

ValveGE::ValveGE ()
{
}

ValveGE::ValveGE( const AcGePoint3d& insertPt, double angle )
: FacilityGE( insertPt, angle )
{
}

ValveGE::~ValveGE ()
{
}
