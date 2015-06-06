#include "StdAfx.h"

#include "DrawTool.h"

#define PI 3.1415926535897932384626433832795

AcGePoint2d Point3D_To_2D( const AcGePoint3d& pt )
{
    return AcGePoint2d( pt.x, pt.y );
}

AcGePoint3d Point2D_To_3D( const AcGePoint2d& pt )
{
    return AcGePoint3d( pt.x, pt.y, 0 );
}

CString MakeUpperText( const CString& inStr )
{
    CString str;
    str.Format( _T( "{\\H0.618x;\\S%s^;}" ), inStr );
    return str;
}

CString MakeLowerText( const CString& inStr )
{
    CString str;
    str.Format( _T( "{\\H0.618x;\\S^%s;}" ), inStr );
    return str;
}

/*
 * 在绘制闭合图形时，AcGiFillType默认为kAcGiFillAlways (始终填充)
 * 闭合图形包括：圆、多边形、网格等
 * 参见：AcGiSubEntityTraits::fillType()方法说明
 * 例如，绘制一个圆，当前颜色是黑底白色，那么采用自定义实体绘制的圆有2种情况:
 *	    1) arx程序加载的情况下-- 白边+黑底填充(正常效果，和cad的圆是一样的)
 *		2) arx程序卸载，cad采用代理实体模式显示图元 -- 白边+白底填充
 * 具体参见：绘制填充圆的一些说明.doc
 */
void DrawCircle( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double radius, bool fill )
{
    AcGiSubEntityTraits& traits = mode->subEntityTraits();
    AcGiFillType ft = traits.fillType();
    traits.setFillType( fill ? kAcGiFillAlways : kAcGiFillNever );

    mode->geometry().circle( insertPt, radius, AcGeVector3d::kZAxis );

    // 恢复属性
    traits.setFillType( ft );
}

extern void DrawArc( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double radius, double startAngle, double sweepAngle, bool fill )
{
    AcGiSubEntityTraits& traits = mode->subEntityTraits();
    AcGiFillType ft = traits.fillType();
    traits.setFillType( fill ? kAcGiFillAlways : kAcGiFillNever );

    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( startAngle, AcGeVector3d::kZAxis );
    AcGiArcType arcType = ( fill ? kAcGiArcChord : kAcGiArcSimple );
    mode->geometry().circularArc( insertPt, radius, AcGeVector3d::kZAxis, v, sweepAngle, arcType );

    // 恢复属性
    traits.setFillType( ft );
}

void DrawArc( AcGiWorldDraw* mode, const AcGePoint3d& spt, const AcGePoint3d& pt, const AcGePoint3d& ept, bool fill )
{
    AcGiSubEntityTraits& traits = mode->subEntityTraits();
    AcGiFillType ft = traits.fillType();
    traits.setFillType( fill ? kAcGiFillAlways : kAcGiFillNever );

    AcGeCircArc3d arc( spt, pt, ept );
    AcGePoint3d cnt = arc.center();
    double radius = arc.radius();
    AcGeVector3d sv = spt - cnt;
    AcGeVector3d ev = ept - cnt;
    double sa = sv.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
    double ea = ev.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    if( arc.normal().z > 0 )
    {
        AcDbArc arcEnt( cnt, radius, sa, ea );
        arcEnt.worldDraw( mode );
    }
    else
    {
        AcDbArc arcEnt( cnt, radius, ea, sa );
        arcEnt.worldDraw( mode );
    }

    // 恢复属性
    traits.setFillType( ft );
}

void DrawLine( AcGiWorldDraw* mode, const AcGePoint3d& spt, const AcGePoint3d& ept )
{
    AcDbLine line( spt, ept );
    line.worldDraw( mode );
}

void DrawLine( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, double length )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );
    DrawLine( mode, pt, pt + v * length );
}

void DrawPolyLine( AcGiWorldDraw* mode, const AcGePoint3d& spt, const AcGePoint3d& ept, double width )
{
    AcDbPolyline pl( 2 );
    pl.addVertexAt( 0, Point3D_To_2D( spt ) );
    pl.addVertexAt( 1, Point3D_To_2D( ept ) );
    pl.setConstantWidth( width );

    pl.worldDraw( mode );
}

void DrawPolyLine( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, double length, double width )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );

    AcDbPolyline pl( 2 );
    pl.addVertexAt( 0, Point3D_To_2D( pt ) );
    pl.addVertexAt( 1, Point3D_To_2D( pt + v * length ) );
    pl.setConstantWidth( width );

    pl.worldDraw( mode );
}

void DrawMText( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, const CString& str, double height, AcDbMText::AttachmentPoint ap )
{
    //acutPrintf(_T("\n绘制前--文字颜色:%d"), mode->subEntityTraits().color());
    AcDbMText mt;
    //AcDbObjectId style; // 文字样式
    //pMText->setTextStyle(style);
    mt.setLocation( pt );
    mt.setTextHeight( height );
    mt.setAttachment( ap );
    mt.setRotation( angle );

    mt.setContents( str );

    // 经过测试发现，AcDbMText调用worldDraw的同时会修改mode的一些属性
    mt.setColorIndex( mode->subEntityTraits().color() );

    mt.worldDraw( mode );

    //acutPrintf(_T("\n绘制后--文字颜色:%d"), mode->subEntityTraits().color());
}

static AcGePoint3d CaclPt( const AcGePoint3d& insertPt, const AcGeVector3d& v1, double width, const AcGeVector3d& v2, double height )
{
    return ( insertPt + v1 * width / 2 + v2 * height / 2 );
}

void DrawRect( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double angle, double width, double height, bool fill )
{
    AcGiSubEntityTraits& traits = mode->subEntityTraits();

    AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    v1.rotateBy( angle, AcGeVector3d::kZAxis );
    v2.rotateBy( angle + PI / 2, AcGeVector3d::kZAxis );

    AcGePoint3dArray pts;
    pts.append( CaclPt( insertPt, v1, width, v2, height ) );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( insertPt, v1, width, v2, height ) );

    v2.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( insertPt, v1, width, v2, height ) );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( insertPt, v1, width, v2, height ) );

    // 是否填充
    AcGiFillType ft = traits.fillType();
    traits.setFillType( fill ? kAcGiFillAlways : kAcGiFillNever );

    mode->geometry().polygon( pts.length(), pts.asArrayPtr() );

    traits.setFillType( ft );
}

void DrawSpline( AcGiWorldDraw* mode, const AcGePoint3dArray& pts )
{
    // 绘制样条曲线
    AcDbSpline sp( pts );
    sp.worldDraw( mode );
}

// 绘制箭头
void DrawArrow( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double angle, double width, double length )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );

    AcDbPolyline pl( 2 );
    pl.addVertexAt( 0, Point3D_To_2D( insertPt ) );
    pl.addVertexAt( 1, Point3D_To_2D( insertPt + v * length ) );
    pl.setWidthsAt( 0, width, 0 );
    pl.setWidthsAt( 1, 0, 0 );

    pl.worldDraw( mode );
}
