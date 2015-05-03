#include "StdAfx.h"
#include "WindBridge.h"

Adesk::UInt32 WindBridge::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    WindBridge, DirectionTagGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ∑Á«≈, DEFGEAPP
)

WindBridge::WindBridge () : DirectionTagGE ()
{
}

WindBridge::WindBridge( const AcGePoint3d& insertPt, double angle ) : DirectionTagGE( insertPt, angle )
{
}

WindBridge::~WindBridge ()
{
}