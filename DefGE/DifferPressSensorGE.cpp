#include "StdAfx.h"
#include "DifferPressSensorGE.h"
#include "../MineGE/DataHelper.h"
#include "../MineGE/DataListHelper.h"

Adesk::UInt32 DifferPressSensorGE::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS ( DifferPressSensorGE, 
						 DirectionTagGE, AcDb::kDHL_CURRENT,
						 AcDb::kMReleaseCurrent,AcDbProxyEntity::kNoOperation,
						 Ñ¹²î´«¸ÐÆ÷, DEFGEAPP )

						 DifferPressSensorGE::DifferPressSensorGE ()
{
}

DifferPressSensorGE::DifferPressSensorGE( const AcGePoint3d& insertPt, double angle )
: DirectionTagGE( insertPt, angle )
{
}

DifferPressSensorGE::~DifferPressSensorGE ()
{
}

static void ReadWriteData( const AcDbObjectId& sObjId, const CString& sField, const AcDbObjectId& tObjId, const CString& tField )
{
	CString v;
	DataHelper::GetPropertyData( sObjId, sField, v );
	DataHelper::SetPropertyData( tObjId, tField, v );
}

void DifferPressSensorGE::writePropertyDataToGEDraw( MineGEDraw* pGEDraw ) const
{
	AcDbObjectId gID = getRelatedGE();
	AcDbObjectId objID = objectId();
	ReadWriteData(gID,_T("¹ÜÂ·Ñ¹²î"),objID,_T("Ñ¹²î"));
	MineGE::writePropertyDataToGEDraw(pGEDraw);
}