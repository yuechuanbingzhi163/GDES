#include "StdAfx.h"
#include "FlowSensorGE.h"
#include "../MineGE/DataHelper.h"
#include "../MineGE/DataListHelper.h"

Adesk::UInt32 FlowSensorGE::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS ( FlowSensorGE, 
						 DirectionTagGE, AcDb::kDHL_CURRENT,
						 AcDb::kMReleaseCurrent,AcDbProxyEntity::kNoOperation,
						 流量传感器, DEFGEAPP )

						 FlowSensorGE::FlowSensorGE ()
{
}

FlowSensorGE::FlowSensorGE( const AcGePoint3d& insertPt, double angle )
: DirectionTagGE( insertPt, angle )
{
}

FlowSensorGE::~FlowSensorGE ()
{
}

static void ReadWriteData( const AcDbObjectId& sObjId, const CString& sField, const AcDbObjectId& tObjId, const CString& tField )
{
	CString v;
	DataHelper::GetPropertyData( sObjId, sField, v );
	DataHelper::SetPropertyData( tObjId, tField, v );
}

void FlowSensorGE::writePropertyDataToGEDraw( MineGEDraw* pGEDraw ) const
{
	AcDbObjectId gID = getRelatedGE();
	AcDbObjectId objID = objectId();
	ReadWriteData(gID,_T("瓦斯管中的瓦斯流量"),objID,_T("流量"));
	MineGE::writePropertyDataToGEDraw(pGEDraw);
}