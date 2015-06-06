#include "StdAfx.h"
#include "GCP.h"

Adesk::UInt32 GCP::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    GCP, PointGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ÍßË¹º¬Á¿µã, DEFGEAPP
)

GCP::GCP() : PointGE()
{

}

GCP::GCP( const AcGePoint3d& insertPt ) : PointGE( insertPt )
{

}
