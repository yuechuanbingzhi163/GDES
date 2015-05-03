#include "StdAfx.h"
#include "DoubleTTunnelDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

ACRX_CONS_DEFINE_MEMBERS ( DoubleTTunnelDraw, DoubleTunnelDraw, 1 )

DoubleTTunnelDraw::DoubleTTunnelDraw( void )
{
}

void DoubleTTunnelDraw::drawText( AcGiWorldDraw* mode )
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

Adesk::Boolean DoubleTTunnelDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

    //DoubleTunnelDraw::subWorldDraw(mode);
    DrawLine( mode, m_leftStartPt, m_leftEndPt );   // 绘制左线
    DrawLine( mode, m_rightStartPt, m_rightEndPt );	// 绘制右线
	DrawLine( mode, m_leftEndPt, m_rightEndPt );	// 绘制封闭
    //DrawJoint( mode, m_startPt, m_width * 0.5, jdt ); // 绘制始节点
    //DrawJoint( mode, m_endPt, m_width * 0.5, jdt ); // 绘制末节点

    // 绘制文字
    drawText( mode );

    return Adesk::kTrue;
}
