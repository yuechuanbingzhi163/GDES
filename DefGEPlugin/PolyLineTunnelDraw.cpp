#include "StdAfx.h"
#include "PolyLineTunnelDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

#include <cmath>

ACRX_CONS_DEFINE_MEMBERS ( PolyLineTunnelDraw, LinkedGEDraw, 1 )

PolyLineTunnelDraw::PolyLineTunnelDraw()
{
}

void PolyLineTunnelDraw::setAllExtraParamsToDefault()
{
    m_width = 40;
    jdt = JDT_NULL;
}

void PolyLineTunnelDraw::configExtraParams()
{
    // 切换资源
    //CAcModuleResourceOverride myResources;
}

void PolyLineTunnelDraw::updateExtraParams()
{
}

void PolyLineTunnelDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_width );

    int i = 0;
    reader.readInt( i );
    jdt = ( JointDrawType )( i % 3 );
}

void PolyLineTunnelDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_width );
    writer.writeInt( ( int )jdt ); // 转换成整数
}

void PolyLineTunnelDraw::extendByLength( double length )
{
    AcGeVector3d v = m_endPt - m_startPt;
    v.normalize();

    m_endPt = m_endPt + v * length; // 更新末点坐标
}

Adesk::Boolean PolyLineTunnelDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    //if(m_startPt == m_endPt) // 始末点重合
    //{
    //	// 绘制立井符号
    //	DrawShaft(mode, m_startPt, m_width);
    //}
    //else
    //{
    double c = 0.618;
    DrawPolyLine( mode, m_startPt, m_endPt, m_width );
    DrawJoint( mode, m_startPt, m_width * 0.5 * c, jdt ); // 绘制始节点
    DrawJoint( mode, m_endPt, m_width * 0.5 * c, jdt ); // 绘制末节点
    //}

    return Adesk::kTrue;
}

Acad::ErrorStatus PolyLineTunnelDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_startPt.transformBy( xform );
    m_endPt.transformBy( xform );
    updateExtraParams();

    return Acad::eOk;
}

Acad::ErrorStatus PolyLineTunnelDraw::subGetOsnapPoints (
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

    // 如果需要设定捕捉点位置，可能还需要增加一些捕捉点
    // 例如中点、2/3处点等等
    if ( osnapMode == AcDb::kOsModeEnd )
    {
        snapPoints.append( m_startPt );
        snapPoints.append( m_endPt );
    }

    return es;
}

//- Grip points protocol
Acad::ErrorStatus PolyLineTunnelDraw::subGetGripPoints ( AcGePoint3dArray& gripPoints,
        AcDbIntArray& osnapModes,
        AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    gripPoints.append( m_startPt );

    if( m_startPt == m_endPt )
    {
        AcGePoint3d pt( m_startPt );
        pt.x = pt.x + m_width * 0.3;
        gripPoints.append( pt );
    }
    else
    {
        gripPoints.append( m_endPt );
    }

    return Acad::eOk;
}

Acad::ErrorStatus PolyLineTunnelDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );

        if ( idx == 0 )
        {
            /*if(m_startPt == m_endPt)
            {
            	m_startPt += offset;
            	m_endPt += offset;
            }
            else
            {*/
            m_startPt += offset;
            //}
        }

        if ( idx == 1 )
        {
            //if(m_startPt == m_endPt) // 始末点重合
            //{
            //	AcGePoint3d pt(m_startPt);
            //	pt.x = pt.x + m_width*0.3; // 外环半径的60%
            //	pt += offset;
            //	m_endPt = pt;
            //}
            //else
            //{
            m_endPt += offset;
            //}
        }
    }
    return Acad::eOk;
}

Acad::ErrorStatus PolyLineTunnelDraw::subGetGeomExtents( AcDbExtents& extents ) const
{
    assertReadEnabled();

    extents.addPoint( m_startPt );
    extents.addPoint( m_endPt );

    return Acad::eOk;
}