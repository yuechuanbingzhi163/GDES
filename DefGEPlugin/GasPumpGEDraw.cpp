#include "StdAfx.h"
#include "GasPumpGEDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

ACRX_CONS_DEFINE_MEMBERS ( GasPumpGEDraw, LinkedGEDraw, 1 )

GasPumpGEDraw::GasPumpGEDraw()
{
}

void GasPumpGEDraw::setAllExtraParamsToDefault()
{
	m_radius = 3;
}

void GasPumpGEDraw::configExtraParams()
{
}

void GasPumpGEDraw::updateExtraParams()
{
	update();
}


void GasPumpGEDraw::readExtraParam( DrawParamReader& reader )
{
	reader.readDouble( m_radius );
	reader.readDouble( m_angle );
}

void GasPumpGEDraw::writeExtraParam( DrawParamWriter& writer )
{
	writer.writeDouble( m_radius );
	writer.writeDouble( m_angle );
}

Adesk::Boolean GasPumpGEDraw::subWorldDraw( AcGiWorldDraw* mode )
{
	assertReadEnabled () ;

	AcGeVector3d v = m_endPt - m_startPt;
	v.normalize();

	// 绘制箭头
	DrawArrow( mode, m_startPt - v * 0.5 * m_radius, m_angle, 2*0.8660254037844386 * m_radius, 1.5 * m_radius * 0.9 );
	DrawCircle( mode, m_startPt, m_radius, false );
	
	AcGePoint3d spt,ept;
	spt = m_startPt - v * 0.5 * m_radius + v * 1.5 * m_radius * 0.9;
	ept = m_startPt - v * 0.5 * m_radius + v * 1.5 * m_radius;
	DrawArrow( mode, spt, m_angle, 2*0.8660254037844386 * m_radius * 0.09, 1.5 * m_radius * 0.1 );

	update();
	return Adesk::kTrue;
}

Acad::ErrorStatus GasPumpGEDraw::subTransformBy( const AcGeMatrix3d& xform )
{
	// 插入点变换
	//m_insertPt.transformBy( xform );
	//m_pt.transformBy( xform );
	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( m_angle, AcGeVector3d::kZAxis );
	v.transformBy( xform );
	//m_radius = v.length();

	// 1) 构造一个圆
	AcDbCircle circle(m_startPt, AcGeVector3d::kZAxis, m_radius);
	// 2) 圆调用transformBy()方法进行变换
	circle.transformBy(xform);
	//3) 获取更新后的参数
	m_startPt = circle.center();     // 获取变换后的圆心坐标
	m_radius = circle.radius(); // 获取变换后的圆半径
	m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis ); // 变换后的旋转角度

	update();
	return Acad::eOk;
}


Acad::ErrorStatus GasPumpGEDraw::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d& pickPoint,
	const AcGePoint3d& lastPoint,
	const AcGeMatrix3d& viewXform,
	AcGePoint3dArray& snapPoints,
	AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;
	if( osnapMode != AcDb::kOsModeCen )
		return Acad::eOk;

	Acad::ErrorStatus es = Acad::eOk;

	if( osnapMode == AcDb::kOsModeCen )
	{
		snapPoints.append( m_startPt );
	}

	return es;
}

Acad::ErrorStatus GasPumpGEDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;

	gripPoints.append( m_startPt );

	return Acad::eOk;
}

Acad::ErrorStatus GasPumpGEDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
	assertWriteEnabled () ;

	int len = indices.length();
	for(int i=0;i<len;i++)
	{
		int idx = indices.at(i);
		if(idx == 0)
		{
			// 当前夹点是圆心，移动图形
			m_startPt += offset;   
			m_endPt += offset;   
		}
	}

	return Acad::eOk;
}

static void DividArc( const AcGePoint3d& spt, const AcGePoint3d& ept, const AcGePoint3d& thirdPt,
					 int count, AcGePoint3dArray& pts )
{
	pts.append( spt );

	AcGeCircArc3d arc( spt, thirdPt, ept );
	AcGePoint3d cnt = arc.center();
	//	double radius = arc.radius();
	AcGeVector3d v1 = spt - cnt;
	AcGeVector3d v2 = ept - cnt;
	int c = ( arc.normal().z > 0 ? 1 : -1 ); // 弧线方向

	double a1 = v1.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
	double a2 = v2.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

	double dq = abs( a2 - a1 ) / count;
	//acutPrintf(_T("\na1:%.3f, a2:%.3f dq:%.3f c:%d"), a1, a2, dq, c);
	for( int i = 1; i < count; i++ )
	{
		v1.rotateBy( c * dq, AcGeVector3d::kZAxis );
		pts.append( cnt + v1 );
		//acutPrintf(_T("\n点%d:(%d,%d)"),i,pts[i].x,pts[i].y);
	}
	pts.append( ept );
}

void GasPumpGEDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
	AcGeVector3d v0( AcGeVector3d::kXAxis );
	//AcGeCircArc3d arc( m_insertPt, v,m_radius );
	v0.rotateBy( m_angle, AcGeVector3d::kZAxis );
	//AcGePoint3d cnt = arc.center();
	AcGePoint3d spt = m_startPt + v0*m_radius;
	AcGePoint3d ept = m_startPt - v0*m_radius;
	v0.rotateBy(3.1415926/4,AcGeVector3d::kZAxis);
	AcGePoint3d pt = m_startPt + v0*m_radius;

	AcGeCircArc3d arc( spt, pt, ept );
	AcGePoint3d cnt = arc.center();
	double radius = arc.radius();

	AcGeVector3d v = pt - cnt;
	v.normalize();

	DividArc( spt, ept, cnt + v * ( radius ), 90, pts );
	DividArc( ept, spt, cnt - v * ( radius ), 90, pts );
}

void GasPumpGEDraw::regPropertyDataNames( AcStringArray& names ) const
{
	names.append(_T("大小"));
}

void GasPumpGEDraw::readPropertyDataFromGE( const AcStringArray& values )
{
	CString strLenth;
	strLenth.Format(_T("%s"),values[0].kACharPtr());

	m_radius = _tstof(strLenth);
	if( 0 >= m_radius ) m_radius = 3;
	AcGeVector3d v = m_endPt - m_startPt;
	v.normalize();
	m_endPt = m_startPt + v * m_radius;
}

void GasPumpGEDraw::extendByLength( double length )
{
	AcGeVector3d v = m_endPt - m_startPt;
	v.normalize();

	m_endPt = m_endPt + v * length; // 更新末点坐标
	update();
}


void GasPumpGEDraw::reverse()
{
	LinkedGEDraw::reverse();
	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( m_angle, AcGeVector3d::kZAxis );
	v.negate();
	m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
	update();
}

void GasPumpGEDraw::update()
{
	AcGeVector3d v = m_endPt - m_startPt;
	v.normalize();
	m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
}
