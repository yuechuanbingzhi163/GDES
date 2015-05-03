#include "StdAfx.h"
#include "PolyLineTTunnelDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

ACRX_CONS_DEFINE_MEMBERS ( PolyLineTTunnelDraw, PolyLineTunnelDraw, 1 )

PolyLineTTunnelDraw::PolyLineTTunnelDraw( void )
{
}

void PolyLineTTunnelDraw::drawText( AcGiWorldDraw* mode )
{
    // 绘制文字
    AcGeVector3d v = m_endPt - m_startPt;
    AcGePoint3d pt = m_startPt + v * 0.5; // 中心点

    if( v.x < 0 ) v.negate();
    double angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    v.normalize();
    v.rotateBy( PI / 2, AcGeVector3d::kZAxis ); // 始终与文字反向

    pt += v * m_width * 0.5;

    DrawMText( mode, pt, angle, _T( "掘进工作面" ), m_width * 0.618, AcDbMText::kBottomCenter );
}

Adesk::Boolean PolyLineTTunnelDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    //PolyLineTunnelDraw::subWorldDraw(mode);
    DrawPolyLine( mode, m_startPt, m_endPt, m_width );
    DrawJoint( mode, m_startPt, m_width * 0.5, jdt ); // 绘制始节点
    DrawJoint( mode, m_endPt, m_width * 0.5, jdt ); // 绘制末节点

    // 绘制一个文字
    drawText( mode );

    return Adesk::kTrue;
}
