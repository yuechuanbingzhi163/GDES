#include "StdAfx.h"
#include "WindDirection.h"

Adesk::UInt32 WindDirection::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    WindDirection, DirectionTagGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    流动方向, DEFGEAPP
)

WindDirection::WindDirection ()
{
}

WindDirection::WindDirection( const AcGePoint3d& insertPt, double angle )
    : DirectionTagGE( insertPt, angle )
{
}

WindDirection::~WindDirection ()
{
}
