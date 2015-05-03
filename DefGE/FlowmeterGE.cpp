#include "StdAfx.h"
#include "FlowmeterGE.h"

Adesk::UInt32 FlowmeterGE::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS ( FlowmeterGE, 
						 FacilityGE, AcDb::kDHL_CURRENT,
						 AcDb::kMReleaseCurrent,AcDbProxyEntity::kNoOperation,
						 Á÷Á¿¼Æ, DEFGEAPP )
FlowmeterGE::FlowmeterGE ()
{
}

FlowmeterGE::FlowmeterGE( const AcGePoint3d& insertPt, double angle )
: FacilityGE( insertPt, angle )
{
}

FlowmeterGE::~FlowmeterGE ()
{
}
