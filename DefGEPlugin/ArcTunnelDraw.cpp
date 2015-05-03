#include "StdAfx.h"
#include "ArcTunnelDraw.h"
#include <cmath>

// 使用ACRX_CONS_DEFINE_MEMBERS即可满足需要
// 不需要dxf、dwg类的宏
// 版本号随便写一个即可
ACRX_CONS_DEFINE_MEMBERS ( ArcTunnelDraw, ConnectedGEDraw, 1 )

ArcTunnelDraw::ArcTunnelDraw()
{
}

ArcTunnelDraw::~ArcTunnelDraw( void )
{
}

void ArcTunnelDraw::setAllParamsToDefault()
{
    m_arcRatio = 0.15;
}

void ArcTunnelDraw::config()
{

}

void ArcTunnelDraw::readExtraParam( GEDrawParamReader& reader )
{
    reader.readDouble( m_arcRatio );
}

void ArcTunnelDraw::writeExtraParam( GEDrawParamWriter& writer )
{
    writer.writeDouble( m_arcRatio );
}

AcGePoint3d ArcTunnelDraw::caclArcCenterPt() const
{
    AcGeVector3d v = m_endPt - m_startPt;

    // 弦的中点坐标
    AcGePoint3d pt = m_startPt + v / 2;

    // 计算弦的中点到弧的中点的距离
    // 公式: d = 弦长/2 * ( 平方根( 4*弧率的平方 + 1 ) - 2*弧率 )
    double d = v.length() * 0.5 * ( sqrt( 4 * m_arcRatio * m_arcRatio + 1 ) - 2 * m_arcRatio );

    v.rotateBy( PI / 2, AcGeVector3d::kZAxis ); // 旋转90度(垂直向量)
    v.normalize(); // 将v转换成单位向量
    v *= d;

    // 计算弦中点坐标
    return pt + v;
}

Adesk::Boolean ArcTunnelDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    mode->geometry().circularArc( m_startPt, caclArcCenterPt(), m_endPt );

    return Adesk::kTrue;
}

Acad::ErrorStatus ArcTunnelDraw::subGetOsnapPoints( AcDb::OsnapMode osnapMode, int gsSelectionMark, const AcGePoint3d& pickPoint, const AcGePoint3d& lastPoint, const AcGeMatrix3d& viewXform, AcGePoint3dArray& snapPoints, AcDbIntArray& geomIds ) const
{
    assertReadEnabled();

    // 执行父类的捕捉行为
    Acad::ErrorStatus es =
        ConnectedGEDraw::subGetOsnapPoints( osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds );

    if( es != Acad::eOk ) return es;

    // 增加捕捉1种类型的点：中点
    if( osnapMode != AcDb::kOsMaskMid ) return Acad::eOk;

    if( osnapMode ==  AcDb::kOsModeMid )
    {
        snapPoints.append( caclArcCenterPt() ); // 将弧中点也作为捕捉点
    }

    return Acad::eOk;
}

Acad::ErrorStatus ArcTunnelDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    Acad::ErrorStatus es =
        ConnectedGEDraw::subGetGripPoints( gripPoints, osnapModes, geomIds );

    if( es != Acad::eOk ) return es;

    gripPoints.append( caclArcCenterPt() ); // 将弧中点也作为夹点

    return Acad::eOk;
}

Acad::ErrorStatus ArcTunnelDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    Acad::ErrorStatus es =
        ConnectedGEDraw::subMoveGripPointsAt( indices, offset );

    if( es != Acad::eOk ) return es;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        // 弧的中点
        if ( idx == 2 )
        {
            AcGePoint3d centerPt = caclArcCenterPt();
            centerPt += offset;

            AcGeCircArc3d arc( m_startPt, centerPt, m_endPt );
            AcGePoint3d pt = arc.center(); // 圆心坐标
            AcGeLine3d line( m_startPt, m_endPt );

            // 计算圆心到弦的距离
            double d = line.distanceTo( pt );

            // 计算弦的长度
            double l = m_startPt.distanceTo( m_endPt );

            // 更新弧率
            m_arcRatio = d / l;
        }
    }
    return ( Acad::eOk );
}