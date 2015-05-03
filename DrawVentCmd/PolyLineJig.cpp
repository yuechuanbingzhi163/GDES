#include "StdAfx.h"
#include "PolyLineJig.h"

#include "../ArxHelper/HelperClass.h"

static AcGePoint2d Point3D_To_2D( const AcGePoint3d& pt )
{
    return AcGePoint2d( pt.x, pt.y );
}

static AcGePoint3d Point2D_To_3D( const AcGePoint2d& pt )
{
    return AcGePoint3d( pt.x, pt.y, 0 );
}

PolyLineJig::PolyLineJig () : AcEdJig ()
{
}

PolyLineJig::~PolyLineJig ()
{
}

Adesk::Boolean PolyLineJig::doJig( AcGePoint3dArray& polygon )
{
    setUserInputControls( ( UserInputControls )( kNullResponseAccepted ) );

    setDispPrompt( _T( "\n请添加边界点: " ) );
    AcGePoint3d pt;
    if( kNormal != acquirePoint( pt ) ) return Adesk::kFalse;

    // 获取第1个点
    polygon.append( pt );

    Adesk::Boolean ret = Adesk::kTrue;
    AcDbObjectId objId;
    while( ret )
    {
        setUserInputControls( ( UserInputControls )( kNullResponseAccepted ) );

        setDispPrompt( _T( "\n请添加边界点[或按空格/ESC退出]: " ) );
        AcGePoint3d next_pt;
        if( kNormal != acquirePoint( next_pt, pt ) ) break;

        if( polygon.contains( next_pt ) )
        {
            setDispPrompt( _T( "\n不允许添加重复点" ) );
            continue;
        }

        polygon.append( next_pt );
        pt = next_pt;
        if( polygon.length() == 2 )
        {
            AcDbPolyline* pPolyLine = new AcDbPolyline( 2 );
            pPolyLine->addVertexAt( 0, Point3D_To_2D( polygon[0] ) );
            pPolyLine->addVertexAt( 1, Point3D_To_2D( polygon[1] ) );
            if( !ArxUtilHelper::PostToModelSpace( pPolyLine ) )
            {
                delete pPolyLine;
                ret = Adesk::kFalse;
            }
            else
            {
                objId = pPolyLine->objectId();
            }
        }
        else
        {
            AcDbEntity* pEnt;
            if( Acad::eOk != acdbOpenAcDbEntity( pEnt, objId, AcDb::kForWrite ) )
            {
                ret = Adesk::kFalse;
            }
            else
            {
                AcDbPolyline* pPolyLine = AcDbPolyline::cast( pEnt );
                pPolyLine->addVertexAt( pPolyLine->numVerts(), Point3D_To_2D( next_pt ) );
                pEnt->close();
            }
        }
    }

    if( !objId.isNull() ) ArxEntityHelper::EraseObject2( objId, true );
    if( !ret ) polygon.removeAll();
    if( polygon.length() < 3 ) ret = false;

    return ret;
}
