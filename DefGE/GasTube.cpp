#include "StdAfx.h"
#include "GasTube.h"
#include "../MineGE/DataHelper.h"
#include "../MineGE/DrawHelper.h"
#include "../MineGE/DataListHelper.h"
Adesk::UInt32 GasTube::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (GasTube, LinkedGE,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						 AcDbProxyEntity::kNoOperation,瓦斯抽采管路, DEFGEAPP)

GasTube::GasTube () : LinkedGE ()
{
}

GasTube::GasTube( const AcGePoint3d& startPt, const AcGePoint3d& endPt ) : LinkedGE( startPt, endPt )
{
}

GasTube::~GasTube ()
{
}
