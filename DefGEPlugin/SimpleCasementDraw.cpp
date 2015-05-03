#include "StdAfx.h"
#include "SimpleCasementDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleCasementDraw, MineGEDraw, 1 )

SimpleCasementDraw::SimpleCasementDraw () : MineGEDraw (), m_offset( 0 )
{
}

SimpleCasementDraw::~SimpleCasementDraw ()
{
}

void SimpleCasementDraw::setAllExtraParamsToDefault()
{
    m_radius = 40;
    m_length = 50;
    m_width = 15;
    m_linewidth = 4;
    m_dTWidth = 40; // 与双线巷道的宽度是相同的，双线巷道宽度调整，同时也应需要修改该宽度值
}

void SimpleCasementDraw::configExtraParams()
{

}

void SimpleCasementDraw::updateExtraParams()
{

}

void SimpleCasementDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

void SimpleCasementDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void SimpleCasementDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_radius );
    reader.readDouble( m_length );
    reader.readDouble( m_width );
    reader.readDouble( m_linewidth );
    reader.readDouble( m_dTWidth );
}

void SimpleCasementDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_radius );
    writer.writeDouble( m_length );
    writer.writeDouble( m_width );
    writer.writeDouble( m_linewidth );
    writer.writeDouble( m_dTWidth );
}

static void DrawT( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double angle, double offset, double length, double width, double lineWidth )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );
    if( v.x < 0 ) v.negate();

    // 注意：rotateBy函数同时会修改对应的this对象
    v.rotateBy( -PI / 2, AcGeVector3d::kZAxis );
    DrawPolyLine( mode, insertPt + v * offset, insertPt + v * length, lineWidth );

    v.rotateBy( PI, AcGeVector3d::kZAxis );
    DrawPolyLine( mode, insertPt + v * offset, insertPt + v * length, lineWidth );

    DrawPolyLine( mode, insertPt + v * length, angle, width, lineWidth );
    DrawPolyLine( mode, insertPt + v * length, angle + PI, width, lineWidth );
}

Adesk::Boolean SimpleCasementDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    DrawArc( mode, m_insertPt, m_radius, m_angle + PI / 2, PI, false ); // 绘制半圆
    DrawT( mode, m_insertPt, m_angle, m_offset, m_length, m_width, m_linewidth ); // 绘制两侧的直线

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleCasementDraw::subTransformBy( const AcGeMatrix3d& xform )
{
    m_insertPt.transformBy( xform ); // 变化插入点

    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( m_angle, AcGeVector3d::kZAxis );
    v.transformBy( xform );

    m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis ); // 变换后的旋转角度
    //m_radius = v.length(); // 变化后的半径

    /*AcGeVector3d startVector(AcGeVector3d::kXAxis*m_length);
    startVector.rotateBy(PI/2+m_angle, AcGeVector3d::kZAxis);
    startVector.transformBy(xform);*/

    //m_length = startVector.length(); // 变化后的长度

    return Acad::eOk;
}

//- Osnap points protocol
Acad::ErrorStatus SimpleCasementDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    int gsSelectionMark,
    const AcGePoint3d& pickPoint,
    const AcGePoint3d& lastPoint,
    const AcGeMatrix3d& viewXform,
    AcGePoint3dArray& snapPoints,
    AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // 只捕捉1种类型的点：中点
    if( osnapMode != AcDb::kOsModeCen ) return Acad::eOk;

    if( osnapMode == AcDb::kOsModeCen )
    {
        snapPoints.append( m_insertPt );
    }

    return Acad::eOk;
}

//- Grip points protocol
Acad::ErrorStatus SimpleCasementDraw::subGetGripPoints ( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    // 插入点作为夹点
    gripPoints.append( m_insertPt );

    // 圆弧的的一个顶点
    AcGeVector3d v( AcGeVector3d::kXAxis * m_radius );
    v.rotateBy( m_angle + PI, AcGeVector3d::kZAxis );
    gripPoints.append( m_insertPt + v );

    // 直线的2个端点
    /*AcGeVector3d startVector(AcGeVector3d::kXAxis*m_length),
    	         endVector(AcGeVector3d::kXAxis*m_length);
    startVector.rotateBy(m_angle-PI/2, AcGeVector3d::kZAxis);
    endVector.rotateBy(m_angle+PI/2, AcGeVector3d::kZAxis);

    gripPoints.append(m_insertPt + startVector);
    gripPoints.append(m_insertPt + endVector);*/

    return Acad::eOk;
}

Acad::ErrorStatus SimpleCasementDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        // 插入点
        if ( idx == 0 ) m_insertPt += offset;

        // 圆弧顶点
        if ( idx == 1 )
        {
            // 计算圆弧半径
            //AcGeVector3d v(AcGeVector3d::kXAxis*m_radius);
            //v.rotateBy(m_angle+PI, AcGeVector3d::kZAxis);
            //v += offset; // 半径向量进行偏移

            //m_radius = v.length(); // 更新半径
            //m_angle = v.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis); // 变换后的旋转角度
        }

        //if(idx == 2)
        //{
        //	AcGeVector3d startVector(AcGeVector3d::kXAxis*m_length);

        //	// 注意：rotateBy函数同时会修改对应的this对象
        //	startVector.rotateBy(m_angle-PI/2, AcGeVector3d::kZAxis);
        //	startVector += offset; // 端点向量进行偏移

        //	// 暂时不更新直线长度
        //	//m_length = startVector.length()*2; // 更新直线长度

        //	startVector.rotateBy(PI/2, AcGeVector3d::kZAxis); // 顺时针旋转90度
        //	m_angle = startVector.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis); // 变换后的旋转角度
        //}

        //if(idx == 3)
        //{
        //	AcGeVector3d endVector(AcGeVector3d::kXAxis*m_length);
        //	endVector.rotateBy(PI/2+m_angle, AcGeVector3d::kZAxis);
        //	endVector += offset; // 端点向量进行偏移

        //	// 暂时不更新直线长度
        //	//m_length = endVector.length()*2; // 更新直线长度

        //	endVector.rotateBy(-PI/2, AcGeVector3d::kZAxis); // 逆时针旋转90度
        //	m_angle = endVector.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis); // 变换后的旋转角度
        //}
    }
    return Acad::eOk;
}

static AcGePoint3d CaclPt( const AcGePoint3d& insertPt, const AcGeVector3d& v1, double width, const AcGeVector3d& v2, double height )
{
    return ( insertPt + v1 * width + v2 * height );
}

static double CaclWidth( double w, double dw )
{
    double width = w;
    if( w > dw / 2 )
    {
        width = sqrt( w * w - dw * dw / 4 );
    }
    return width;
}

void SimpleCasementDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    v1.rotateBy( m_angle + PI / 2, AcGeVector3d::kZAxis );
    v2.rotateBy( m_angle + PI, AcGeVector3d::kZAxis );

    double width = CaclWidth( m_radius, m_dTWidth );

    pts.append( m_insertPt + v1 * m_length );
    pts.append( CaclPt( m_insertPt, v1, m_length, v2, width ) );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( m_insertPt, v1, m_length, v2, width ) );
    pts.append( m_insertPt + v1 * m_length );
}