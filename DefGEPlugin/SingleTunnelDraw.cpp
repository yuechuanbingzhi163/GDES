#include "StdAfx.h"
#include "SingleTunnelDraw.h"

ACRX_CONS_DEFINE_MEMBERS ( SingleTunnelDraw, LinkedGEDraw, 1 )

SingleTunnelDraw::SingleTunnelDraw()
{
}

SingleTunnelDraw::~SingleTunnelDraw( void )
{
}

void SingleTunnelDraw::setAllExtraParamsToDefault()
{

}

void SingleTunnelDraw::configExtraParams()
{

}

void SingleTunnelDraw::updateExtraParams()
{

}

void SingleTunnelDraw::readExtraParam( DrawParamReader& reader )
{
    // do nothing
}

void SingleTunnelDraw::writeExtraParam( DrawParamWriter& writer )
{
    // do nothing
}

void SingleTunnelDraw::extendByLength( double length )
{
    AcGeVector3d v = m_endPt - m_startPt;
    v.normalize();

    m_endPt = m_endPt + v * length; // 修改末点坐标
}

Adesk::Boolean SingleTunnelDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    AcGePoint3dArray pts;
    pts.append( m_startPt );
    pts.append( m_endPt );
    mode->geometry().worldLine( pts.asArrayPtr() );

    return Adesk::kTrue;
}

//----- AcDbEntity protocols
Acad::ErrorStatus SingleTunnelDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_startPt.transformBy( xform );
    m_endPt.transformBy( xform );

    return Acad::eOk;
}

//- Osnap points protocol
Acad::ErrorStatus SingleTunnelDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    Adesk::GsMarker gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // 只捕捉1种类型的点：端点
    if( osnapMode != AcDb::kOsModeEnd )
        return Acad::eOk;

    Acad::ErrorStatus es = Acad::eOk;

    if ( osnapMode == AcDb::kOsModeEnd )
    {
        snapPoints.append( m_startPt );
        snapPoints.append( m_endPt );
    }

    return es;
}

//- Grip points protocol
Acad::ErrorStatus SingleTunnelDraw::subGetGripPoints (
    AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;
    //----- This method is never called unless you return eNotImplemented
    //----- from the new getGripPoints() method below (which is the default implementation)

    gripPoints.append( m_startPt );
    gripPoints.append( m_endPt );
    return Acad::eOk;
}

Acad::ErrorStatus SingleTunnelDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;
    //----- This method is never called unless you return eNotImplemented
    //----- from the new moveGripPointsAt() method below (which is the default implementation)

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        // 始节点
        if ( idx == 0 ) m_startPt += offset;

        if ( idx == 1 ) m_endPt += offset;
    }
    return Acad::eOk;
}

Acad::ErrorStatus SingleTunnelDraw::subGetGeomExtents( AcDbExtents& extents ) const
{
    assertReadEnabled();

    extents.addPoint( m_startPt );
    extents.addPoint( m_endPt );

    return Acad::eOk;
}