#include "StdAfx.h"
#include "SimpleMainFanDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleMainFanDraw, MineGEDraw, 1 )

SimpleMainFanDraw::SimpleMainFanDraw () : MineGEDraw ()
{
}

SimpleMainFanDraw::~SimpleMainFanDraw ()
{
}

void SimpleMainFanDraw::setAllExtraParamsToDefault()
{
    m_radius = 30;
    m_distance = 110;
}

void SimpleMainFanDraw::configExtraParams()
{

}

void SimpleMainFanDraw::updateExtraParams()
{

}

void SimpleMainFanDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

void SimpleMainFanDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void SimpleMainFanDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_radius );
    reader.readDouble( m_distance );
}

void SimpleMainFanDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_radius );
    writer.writeDouble( m_distance );
}

Adesk::Boolean SimpleMainFanDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    DrawCircle( mode, m_insertPt, m_radius, false );

    // 计算点坐标
    AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    v1.rotateBy( m_angle + PI / 6, AcGeVector3d::kZAxis ); // 倾角：30度
    v2.rotateBy( m_angle - PI / 6, AcGeVector3d::kZAxis );

    double c = 1.1547005383792515290182975610039; // 2除以根号3
    double L = c * m_distance;
    AcGePoint3d firstPt = m_insertPt + v1 * L;
    AcGePoint3d secondPt = m_insertPt + v2 * L;

    // 绘制3条直线
    DrawLine( mode, m_insertPt, firstPt );
    DrawLine( mode, m_insertPt, secondPt );
    DrawLine( mode, firstPt, secondPt );

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleMainFanDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_insertPt.transformBy( xform );

    // 构造一个倾角向量
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis ); // 得到原有的倾角向量

    // 执行变换
    v.transformBy( xform );

    m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    return Acad::eOk;
}

Acad::ErrorStatus SimpleMainFanDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;
    // 只捕捉1种类型的点：插入点
    if( osnapMode != AcDb::kOsModeCen )
        return Acad::eOk;

    Acad::ErrorStatus es = Acad::eOk;

    if ( osnapMode == AcDb::kOsModeCen )
    {
        snapPoints.append( m_insertPt );

        AcGeVector3d v( AcGeVector3d::kXAxis );
        v.rotateBy( m_angle, AcGeVector3d::kZAxis );

        snapPoints.append( m_insertPt + v * m_distance );
    }

    return es;
}

Acad::ErrorStatus SimpleMainFanDraw::subGetGripPoints (
    AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds
) const
{
    assertReadEnabled () ;

    gripPoints.append( m_insertPt );

    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis );

    gripPoints.append( m_insertPt + v * m_distance );

    return Acad::eOk;
}

Acad::ErrorStatus SimpleMainFanDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        // 始节点
        if ( idx == 0 ) m_insertPt += offset;

        if ( idx == 1 )
        {
            AcGeVector3d v( AcGeVector3d::kXAxis );
            v.rotateBy( m_angle, AcGeVector3d::kZAxis );

            AcGePoint3d pt = m_insertPt + v * m_distance;
            pt += offset; // 进行偏移

            // 目前暂时不考虑distance的变化
            //distance = insertPt.distanceTo(pt); // 计算新的距离

            // 计算角度
            v = pt - m_insertPt;
            m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
        }
    }
    return Acad::eOk;
}