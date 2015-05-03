#include "StdAfx.h"
#include "DetermineHoleGE.h"

Adesk::UInt32 DetermineHoleGE::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS ( DetermineHoleGE, 
						 FacilityGE, AcDb::kDHL_CURRENT,
						 AcDb::kMReleaseCurrent,AcDbProxyEntity::kNoOperation,
						 ²â¶¨¿×, DEFGEAPP )

DetermineHoleGE::DetermineHoleGE ()
{
}

DetermineHoleGE::DetermineHoleGE( const AcGePoint3d& insertPt, double angle )
: FacilityGE( insertPt, angle )
{
}

DetermineHoleGE::~DetermineHoleGE ()
{
}
