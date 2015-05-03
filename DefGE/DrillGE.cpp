#include "StdAfx.h"
#include "DrillGE.h"

Adesk::UInt32 DrillGE::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (DrillGE, LinkedGE,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						 AcDbProxyEntity::kNoOperation,×ê¿×, DEFGEAPP)

						 DrillGE::DrillGE () : LinkedGE ()
{
}

DrillGE::DrillGE( const AcGePoint3d& startPt, const AcGePoint3d& endPt ) : LinkedGE( startPt, endPt )
{
}

DrillGE::~DrillGE ()
{

}