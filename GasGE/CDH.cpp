#include "StdAfx.h"
#include "CDH.h"

Adesk::UInt32 CDH::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    CDH, PointGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ¼ûÃº×ê¿×, DEFGEAPP
)

CDH::CDH() : PointGE()
{

}

CDH::CDH( const AcGePoint3d& insertPt ) : PointGE( insertPt )
{

}
