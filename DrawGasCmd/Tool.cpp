#include "stdafx.h"

#include "Tool.h"
#include "../GasGE/PointGE.h"

bool GetInsertPoint( const AcDbObjectId& objId, AcGePoint3d& pt )
{
    AcDbEntity* pEnt;
    if( Acad::eOk != acdbOpenObject( pEnt, objId, AcDb::kForRead ) ) return false;

    PointGE* pGE = PointGE::cast( pEnt );
    bool ret = ( pGE != 0 );
    if( ret )
    {
        pt = pGE->getInsertPt();
    }
    pEnt->close();

    return ret;
}

bool SetInsertPoint( const AcDbObjectId& objId, const AcGePoint3d& pt )
{
    AcDbEntity* pEnt;
    if( Acad::eOk != acdbOpenObject( pEnt, objId, AcDb::kForWrite ) ) return false;

    PointGE* pGE = PointGE::cast( pEnt );
    bool ret = ( pGE != 0 );
    if( ret )
    {
        pGE->setInsertPt( pt );;
    }
    pEnt->close();

    return ret;
}