#include "StdAfx.h"
#include "WindLibrary.h"

Adesk::UInt32 WindLibrary::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
						 WindLibrary, DirectionTagGE,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						 AcDbProxyEntity::kNoOperation,
						 ·ç¿â, DEFGEAPP
						 )

WindLibrary::WindLibrary() : DirectionTagGE()
{

}

WindLibrary::WindLibrary( const AcGePoint3d& insertPt, double angle ): DirectionTagGE( insertPt, angle )
{

}
