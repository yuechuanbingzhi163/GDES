#include "StdAfx.h"
#include "Fan.h"

Adesk::UInt32 Fan::kCurrentVersionNumber = 1 ;
Adesk::UInt32 LocalFan::kCurrentVersionNumber = 1 ;

ACRX_NO_CONS_DEFINE_MEMBERS( Fan, DirectionTagGE )

ACRX_DXF_DEFINE_MEMBERS (
    LocalFan, Fan,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    局部扇风机, DEFGEAPP
)

Fan::Fan () : DirectionTagGE ()
{
}

Fan::Fan( const AcGePoint3d& insertPt, double angle ) : DirectionTagGE( insertPt, angle )
{

}

LocalFan::LocalFan () : Fan ()
{
}

LocalFan::LocalFan( const AcGePoint3d& insertPt, double angle ) : Fan( insertPt, angle )
{

}
