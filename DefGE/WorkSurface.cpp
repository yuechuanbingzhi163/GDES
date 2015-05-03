#include "StdAfx.h"
#include "WorkSurface.h"

Adesk::UInt32 WorkSurface::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    WorkSurface, LinkedGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    回采工作面, DEFGEAPP
)

WorkSurface::WorkSurface () : LinkedGE ()
{
}

WorkSurface::WorkSurface( const AcGePoint3d& startPt, const AcGePoint3d& endPt ) : LinkedGE( startPt, endPt )
{

}

WorkSurface::~WorkSurface ()
{
}