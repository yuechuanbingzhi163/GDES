#include "StdAfx.h"
#include "BackFireGE.h"

Adesk::UInt32 BackFireGE::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS ( BackFireGE, 
						 FacilityGE, AcDb::kDHL_CURRENT,
						 AcDb::kMReleaseCurrent,AcDbProxyEntity::kNoOperation,
						 ×è»ðÆ÷, DEFGEAPP )
BackFireGE::BackFireGE ()
{
}

BackFireGE::BackFireGE( const AcGePoint3d& insertPt, double angle )
: FacilityGE( insertPt, angle )
{
}

BackFireGE::~BackFireGE ()
{
}
