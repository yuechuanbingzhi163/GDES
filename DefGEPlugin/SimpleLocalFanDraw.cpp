#include "StdAfx.h"
#include "SimpleLocalFanDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleLocalFanDraw, MineGEDraw, 1 )

SimpleLocalFanDraw::SimpleLocalFanDraw () : MineGEDraw ()
{
}

SimpleLocalFanDraw::~SimpleLocalFanDraw ()
{
}

void SimpleLocalFanDraw::setAllExtraParamsToDefault()
{
    m_radius = 10;
	m_distance = 16;
}

void SimpleLocalFanDraw::configExtraParams()
{

}

void SimpleLocalFanDraw::updateExtraParams()
{

}

void SimpleLocalFanDraw::writeKeyParam( DrawParamWriter& writer )
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

void SimpleLocalFanDraw::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void SimpleLocalFanDraw::readExtraParam( DrawParamReader& reader )
{
    reader.readDouble( m_radius );
}

void SimpleLocalFanDraw::writeExtraParam( DrawParamWriter& writer )
{
    writer.writeDouble( m_radius );
}

static AcGePoint3d CaclLeftBottomPt( const AcGePoint3d& pt, double angle, double width, double height )
{
    AcGeVector3d v( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );
    v2.rotateBy( angle + PI / 2, AcGeVector3d::kZAxis );
    if( v.x > 0 ) // 位于第1和4象限
    {
        v.rotateBy( PI, AcGeVector3d::kZAxis );
        v2.rotateBy( PI, AcGeVector3d::kZAxis );
    }
    return ( pt + v * width / 2 + v2 * height / 2 );
}

Adesk::Boolean SimpleLocalFanDraw::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;
	drawFan(mode);
    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleLocalFanDraw::subTransformBy( const AcGeMatrix3d& xform )
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

Acad::ErrorStatus SimpleLocalFanDraw::subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    Adesk::GsMarker gsSelectionMark,
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
    }

    return es;
}

Acad::ErrorStatus SimpleLocalFanDraw::subGetGripPoints (
    AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds
) const
{
    assertReadEnabled () ;

    gripPoints.append( m_insertPt );

    return Acad::eOk;
}

Acad::ErrorStatus SimpleLocalFanDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );
        // 始节点
        if ( idx == 0 ) m_insertPt += offset;
    }
    return Acad::eOk;
}

static AcGePoint3d CaclPt( const AcGePoint3d& insertPt, const AcGeVector3d& v1, double width, const AcGeVector3d& v2, double height )
{
    return ( insertPt + v1 * width + v2 * height );
}

void SimpleLocalFanDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    /*AcGeVector3d v1(AcGeVector3d::kXAxis), v2(AcGeVector3d::kXAxis);
    v1.rotateBy(m_angle, AcGeVector3d::kZAxis);
    v2.rotateBy(m_angle+PI/2, AcGeVector3d::kZAxis);

    pts.append(CaclPt(m_insertPt, v1, m_radius, v2, m_radius));

    v1.rotateBy(PI, AcGeVector3d::kZAxis);
    pts.append(CaclPt(m_insertPt, v1, m_radius, v2, m_radius));

    v2.rotateBy(PI, AcGeVector3d::kZAxis);
    pts.append(CaclPt(m_insertPt, v1, m_radius, v2, m_radius));

    v1.rotateBy(PI, AcGeVector3d::kZAxis);
    pts.append(CaclPt(m_insertPt, v1, m_radius, v2, m_radius));*/
}

AcGePoint3d SimpleLocalFanDraw::vp_add( AcGeVector3d v,AcGePoint3d p )
{
	return AcGePoint3d(v.x+p.x,v.y+p.y,v.z+p.z);
}

void SimpleLocalFanDraw::drawFan(AcGiWorldDraw* mode)
{
	AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
	v1.rotateBy( m_angle, AcGeVector3d::kZAxis ); 
	v2.rotateBy( m_angle + PI * 0.5, AcGeVector3d::kZAxis );

	AcGePoint3d p1,p2,pt1,pt2,pt3,pt4,pt5,pt6,pt7,pt8;
	p1 = vp_add(v2 * m_distance * 0.25, m_insertPt);
	pt1 = vp_add(v1 * m_distance * 0.5, p1);
	v1.normalize();

	double sin45 = 0.70710678118654752440084436210485;
	v1.rotateBy(PI / 4, AcGeVector3d::kZAxis );
	pt2 = vp_add(v1 * m_distance * 0.25 / sin45, pt1);

	v1.normalize();
	v1.rotateBy(- 3 * PI / 4, AcGeVector3d::kZAxis );
	pt3 = vp_add(v1 * m_distance, pt2);

	v1.normalize();
	v1.rotateBy(- 3 * PI / 4, AcGeVector3d::kZAxis );
	pt4 = vp_add(v1 * m_distance * 0.25 / sin45, pt3);

	v1.normalize();
	v1.rotateBy(PI / 4, AcGeVector3d::kZAxis );
	pt5 = vp_add(v1 * m_distance, pt4);

	v1.normalize();
	v1.rotateBy(PI / 4, AcGeVector3d::kZAxis );
	pt6 = vp_add(v1 * m_distance * 0.25 / sin45, pt5);

	v1.normalize();
	v1.rotateBy(- 3 * PI / 4, AcGeVector3d::kZAxis );
	pt7 = vp_add(v1 * m_distance, pt6);

	v1.normalize();
	v1.rotateBy(- 3 * PI / 4, AcGeVector3d::kZAxis );
	pt8 = vp_add(v1 * m_distance * 0.25 / sin45, pt7);

	DrawLine(mode,pt1,pt2);
	DrawLine(mode,pt2,pt3);
	DrawLine(mode,pt3,pt4);
	DrawLine(mode,pt4,pt5);
	DrawLine(mode,pt5,pt6);
	DrawLine(mode,pt6,pt7);
	DrawLine(mode,pt7,pt8);
	DrawLine(mode,pt8,pt1);

	AcGePoint3d aPt1,aPt2,aPt3,aPt4,aPt5,aPt6,aPt7;

	aPt1 = vp_add((pt8-pt1)*0.5,pt1);
	aPt2 = vp_add((pt4-pt1)*0.5,pt1);
	aPt3 = vp_add((pt5-pt4)*0.5,pt4);
	aPt4 = vp_add((pt8-pt5)*1.0/3,pt5);
	aPt5 = vp_add((pt8-pt5)*1.0/3,aPt4);
	aPt6 = vp_add((pt1-pt8)*2.0/3,aPt4);
	aPt7 = vp_add((pt1-pt8)*2.0/3,aPt5);
	
	DrawLine(mode,aPt1,aPt2);
	DrawLine(mode,aPt2,aPt3);
	DrawLine(mode,aPt3,aPt6);
	DrawLine(mode,aPt6,aPt4);
	DrawLine(mode,aPt5,aPt7);
	DrawLine(mode,aPt7,aPt1);
	DrawLine(mode,aPt4,aPt5);
}