#include "StdAfx.h"
#include "Joint.h"

Adesk::UInt32 Joint::kCurrentVersionNumber = 1 ;
Adesk::UInt32 Test::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
						 Joint, DirectionTagGE,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						 AcDbProxyEntity::kNoOperation,
						 ½Úµã, DEFGEAPP
						 )

Joint::Joint() : DirectionTagGE()
{

}

Joint::Joint( const AcGePoint3d& insertPt, double angle ): DirectionTagGE( insertPt, angle )
{

}

ACRX_DXF_DEFINE_MEMBERS (
						 Test, AcDbEntity,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						 AcDbProxyEntity::kNoOperation,
						 ²âÊÔ, DEFGEAPP
						 )

Test::Test()
{

}