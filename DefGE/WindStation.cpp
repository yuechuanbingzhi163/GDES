#include "StdAfx.h"
#include "WindStation.h"

Adesk::UInt32 WindStation::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    WindStation, DirectionTagGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ≤‚∑Á’æ, DEFGEAPP
)

WindStation::WindStation() : DirectionTagGE()
{

}

WindStation::WindStation( const AcGePoint3d& insertPt, double angle ): DirectionTagGE( insertPt, angle )
{

}
