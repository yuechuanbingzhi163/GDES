#include "StdAfx.h"
#include "Tunnel.h"

Adesk::UInt32 Tunnel::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    Tunnel, LinkedGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ÏïµÀ, DEFGEAPP
)

Tunnel::Tunnel () : LinkedGE ()
{
}

Tunnel::Tunnel( const AcGePoint3d& startPt, const AcGePoint3d& endPt ) : LinkedGE( startPt, endPt )
{
}

Tunnel::~Tunnel ()
{
}