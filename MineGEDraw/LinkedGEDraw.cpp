#include "StdAfx.h"
#include "LinkedGEDraw.h"

// 注意：有修改
ACRX_NO_CONS_DEFINE_MEMBERS ( LinkedGEDraw, MineGEDraw )

LinkedGEDraw::LinkedGEDraw()
{

}

void LinkedGEDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_startPt );
    reader.readPoint( m_endPt );
}

void LinkedGEDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_startPt );
    writer.writePoint( m_endPt );
}


void LinkedGEDraw::dealWithStartPointBoundary( const AcGeRay3d& boundaryLine )
{
    // do nothing
}

void LinkedGEDraw::dealWithEndPointBoundary( const AcGeRay3d& boundaryLine )
{
    // do nothing
}

AcGeVector3d LinkedGEDraw::getStartPointInExtendAngle() const
{
    return ( m_endPt - m_startPt );
}

AcGeVector3d LinkedGEDraw::getEndPointInExtendAngle() const
{
    AcGeVector3d v = m_endPt - m_startPt;
    v.rotateBy( PI, AcGeVector3d::kZAxis ); // 旋转180度

    return v;
}

static void SwapPoint( AcGePoint3d& pt1, AcGePoint3d& pt2 )
{
    AcGePoint3d tpt = pt1;
    pt1 = pt2;
    pt2 = tpt;
}

void LinkedGEDraw::reverse()
{
    SwapPoint( m_startPt, m_endPt );
}

void LinkedGEDraw::extendByLength( double length )
{

}