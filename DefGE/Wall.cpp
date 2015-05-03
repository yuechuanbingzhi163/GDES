#include "StdAfx.h"
#include "Wall.h"

Adesk::UInt32 Wall::kCurrentVersionNumber = 1 ;
Adesk::UInt32 PermanentWall::kCurrentVersionNumber = 1 ;
Adesk::UInt32 TemporaryWall::kCurrentVersionNumber = 1 ;

ACRX_NO_CONS_DEFINE_MEMBERS( Wall, DirectionTagGE )

ACRX_DXF_DEFINE_MEMBERS (
    PermanentWall, Wall,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ”¿æ√√‹±’, DEFGEAPP
)

ACRX_DXF_DEFINE_MEMBERS (
    TemporaryWall, Wall,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ¡Ÿ ±√‹±’, DEFGEAPP
)

Wall::Wall() : DirectionTagGE()
{

}

Wall::Wall( const AcGePoint3d& insertPt, double angle ): DirectionTagGE( insertPt, angle )
{

}

PermanentWall::PermanentWall () : Wall ()
{
}

PermanentWall::PermanentWall( const AcGePoint3d& insertPt, double angle ) : Wall( insertPt, angle )
{

}

TemporaryWall::TemporaryWall () : Wall ()
{
}

TemporaryWall::TemporaryWall( const AcGePoint3d& insertPt, double angle ) : Wall( insertPt, angle )
{

}