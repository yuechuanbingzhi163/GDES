#include "StdAfx.h"
#include "TailraceGE.h"

Adesk::UInt32 TailraceGE::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS ( TailraceGE, 
						 FacilityGE, AcDb::kDHL_CURRENT,
						 AcDb::kMReleaseCurrent,AcDbProxyEntity::kNoOperation,
						 ·ÅË®Æ÷, DEFGEAPP )

TailraceGE::TailraceGE ()
{
}

TailraceGE::TailraceGE( const AcGePoint3d& insertPt, double angle )
: FacilityGE( insertPt, angle )
{
}

TailraceGE::~TailraceGE ()
{
}
