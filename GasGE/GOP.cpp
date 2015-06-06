#include "StdAfx.h"
#include "GOP.h"

Adesk::UInt32 GOP::kCurrentVersionNumber = 1 ;
Adesk::UInt32 SmallGOP::kCurrentVersionNumber = 1 ;
Adesk::UInt32 MidGOP::kCurrentVersionNumber = 1 ;
Adesk::UInt32 LargeGOP::kCurrentVersionNumber = 1 ;
Adesk::UInt32 LargerGOP::kCurrentVersionNumber = 1 ;

ACRX_NO_CONS_DEFINE_MEMBERS( GOP, PointGE )

ACRX_DXF_DEFINE_MEMBERS (
    SmallGOP, GOP,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    小型突出点, DEFGEAPP
)

ACRX_DXF_DEFINE_MEMBERS (
    MidGOP, GOP,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    中型突出点, DEFGEAPP
)

ACRX_DXF_DEFINE_MEMBERS (
    LargeGOP, GOP,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    大型突出点, DEFGEAPP
)

ACRX_DXF_DEFINE_MEMBERS (
    LargerGOP, GOP,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    特大型突出点, DEFGEAPP
)

GOP::GOP() : PointGE()
{

}

GOP::GOP( const AcGePoint3d& insertPt ): PointGE( insertPt )
{

}

SmallGOP::SmallGOP () : GOP ()
{
}

SmallGOP::SmallGOP( const AcGePoint3d& insertPt ) : GOP( insertPt )
{

}

MidGOP::MidGOP () : GOP ()
{
}

MidGOP::MidGOP( const AcGePoint3d& insertPt ) : GOP( insertPt )
{

}

LargeGOP::LargeGOP () : GOP ()
{
}

LargeGOP::LargeGOP( const AcGePoint3d& insertPt ) : GOP( insertPt )
{

}

LargerGOP::LargerGOP () : GOP ()
{
}

LargerGOP::LargerGOP( const AcGePoint3d& insertPt ) : GOP( insertPt )
{

}
