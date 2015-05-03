#include "StdAfx.h"
#include "GasPumpGE.h"

Adesk::UInt32 GasPumpGE::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS ( GasPumpGE, 
LinkedGE, AcDb::kDHL_CURRENT,
AcDb::kMReleaseCurrent,AcDbProxyEntity::kNoOperation,
ÍßË¹±Ã, DEFGEAPP )

GasPumpGE::GasPumpGE () : LinkedGE ()
{
}

GasPumpGE::GasPumpGE(const AcGePoint3d& startPt, const AcGePoint3d& endPt) : LinkedGE( startPt, endPt )
{
}

GasPumpGE::~GasPumpGE ()
{
}