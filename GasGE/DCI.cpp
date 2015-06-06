#include "StdAfx.h"
#include "DCI.h"

Adesk::UInt32 DCI::kCurrentVersionNumber = 1 ;
Adesk::UInt32 CSDCI::kCurrentVersionNumber = 1 ;
Adesk::UInt32 WDCI1::kCurrentVersionNumber = 1 ;
Adesk::UInt32 WDCI2::kCurrentVersionNumber = 1 ;

ACRX_NO_CONS_DEFINE_MEMBERS( DCI, PointGE )

ACRX_DXF_DEFINE_MEMBERS (
    CSDCI, DCI,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    煤层区域突出危险性预测指标值, DEFGEAPP
)

ACRX_DXF_DEFINE_MEMBERS (
    WDCI1, DCI,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    工作面突出危险性预测指标值1, DEFGEAPP
)

ACRX_DXF_DEFINE_MEMBERS (
    WDCI2, DCI,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    工作面突出危险性预测指标值2, DEFGEAPP
)

DCI::DCI() : PointGE()
{

}

DCI::DCI( const AcGePoint3d& insertPt ) : PointGE( insertPt )
{

}

CSDCI::CSDCI() : DCI()
{

}

CSDCI::CSDCI( const AcGePoint3d& insertPt ) : DCI( insertPt )
{

}

WDCI1::WDCI1() : DCI()
{

}

WDCI1::WDCI1( const AcGePoint3d& insertPt ) : DCI( insertPt )
{

}

WDCI2::WDCI2() : DCI()
{

}

WDCI2::WDCI2( const AcGePoint3d& insertPt ) : DCI( insertPt )
{

}
