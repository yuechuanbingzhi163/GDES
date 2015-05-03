#include "StdAfx.h"
#include "GasFlowTagGE.h"

Adesk::UInt32 GasFlowTagGE::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
						 GasFlowTagGE, FacilityGE,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						 AcDbProxyEntity::kNoOperation,
						 瓦斯流量, DEFGEAPP
						 )

GasFlowTagGE::GasFlowTagGE() : FacilityGE()
{

}

GasFlowTagGE::GasFlowTagGE( const AcGePoint3d& insertPt, double angle ): FacilityGE( insertPt, angle )
{

}


#include "../MineGE/DataHelper.h"
static void ReadWriteData( const AcDbObjectId& sObjId, const CString& sField, const AcDbObjectId& tObjId, const CString& tField )
{
	CString v;
	DataHelper::GetPropertyData( sObjId, sField, v );
	DataHelper::SetPropertyData( tObjId, tField, v );
}

void GasFlowTagGE::writePropertyDataToGEDraw( MineGEDraw* pGEDraw ) const
{
	//acutPrintf(_T("\n些数据"));
	AcDbObjectId gID = getRelatedGE();
	AcDbObjectId objID = objectId();
	ReadWriteData(gID,_T("瓦斯管中的瓦斯流量"),objID,_T("瓦斯流量"));
	MineGE::writePropertyDataToGEDraw(pGEDraw);
}