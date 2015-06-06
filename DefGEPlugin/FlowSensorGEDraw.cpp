#include "StdAfx.h"
#include "FlowSensorGEDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

ACRX_CONS_DEFINE_MEMBERS ( FlowSensorGEDraw, MineGEDraw, 1 )

FlowSensorGEDraw::FlowSensorGEDraw()
{

}

FlowSensorGEDraw::~FlowSensorGEDraw( void )
{
}

void FlowSensorGEDraw::setAllExtraParamsToDefault()
{
	m_radius = 3;
	m_temp = _T("");
}

void FlowSensorGEDraw::configExtraParams()
{

}

void FlowSensorGEDraw::updateExtraParams()
{

}

void FlowSensorGEDraw::writeKeyParam( DrawParamWriter& writer )
{
	writer.writePoint( m_insertPt );
	writer.writeDouble( m_angle );
}

void FlowSensorGEDraw::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble( m_angle );
}

void FlowSensorGEDraw::readExtraParam( DrawParamReader& reader )
{
	reader.readDouble( m_radius );
	reader.readString( m_temp );
}

void FlowSensorGEDraw::writeExtraParam( DrawParamWriter& writer )
{
	writer.writeDouble( m_radius );
	writer.writeString( m_temp );
}

Adesk::Boolean FlowSensorGEDraw::subWorldDraw( AcGiWorldDraw* mode )
{
	assertReadEnabled () ;

	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( m_angle, AcGeVector3d::kZAxis );

	if( v.x < 0 ) v.negate();
	m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

	DrawCircle( mode, m_insertPt, m_radius, false );
	v.rotateBy( PI / 2 , AcGeVector3d::kZAxis );
	v.normalize();

	AcGePoint3d pt = m_insertPt;
	if(!LESS_ZERO(m_temp))
	{
		pt += 0.2 * m_radius * v;
	}

	DrawMText(mode,pt,m_angle,_T("L"),0.6 * m_radius);

	v.rotateBy( PI,AcGeVector3d::kZAxis );
	pt = m_insertPt + 0.5 * m_radius * v;

	if(LESS_ZERO(m_temp)) return Adesk::kTrue;
	CString temp;
	DealSpPoints(m_temp);
	temp.Format(_T("%s"),m_temp);
	DrawMText(mode,pt,m_angle, temp ,0.2 * m_radius);

	pt = m_insertPt + 0.8 * m_radius * v;

	CString str;

	str.Format( _T( "m%s/min" ), MakeUpperText( _T( "3" ) ) ); // m^3/min
	DrawMText(mode,pt,m_angle, str ,0.2 * m_radius);

	return Adesk::kTrue;
}

Acad::ErrorStatus FlowSensorGEDraw::subTransformBy( const AcGeMatrix3d& xform )
{
	// 插入点变换
	//m_insertPt.transformBy( xform );
	//m_pt.transformBy( xform );
	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( m_angle, AcGeVector3d::kZAxis );
	v.transformBy( xform );
	//m_radius = v.length();

	// 1) 构造一个圆
	AcDbCircle circle(m_insertPt, AcGeVector3d::kZAxis, m_radius);
	// 2) 圆调用transformBy()方法进行变换
	circle.transformBy(xform);
	//3) 获取更新后的参数
	m_insertPt = circle.center();     // 获取变换后的圆心坐标
	m_radius = circle.radius(); // 获取变换后的圆半径
	m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis ); // 变换后的旋转角度

	return Acad::eOk;
}


Acad::ErrorStatus FlowSensorGEDraw::subGetOsnapPoints (
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
		snapPoints.append( m_insertPt );
	}

	return es;
}

Acad::ErrorStatus FlowSensorGEDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;

	gripPoints.append( m_insertPt );

	AcGeVector3d v(AcGeVector3d::kXAxis);
	v.rotateBy(m_angle,AcGeVector3d::kZAxis);
	v *= m_radius;

	gripPoints.append(m_insertPt + v);             // 正方向端点作为夹点

	return Acad::eOk;
}

Acad::ErrorStatus FlowSensorGEDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
	assertWriteEnabled () ;

	int len = indices.length();
	for(int i=0;i<len;i++)
	{
		int idx = indices.at(i);
		if(idx == 0)
		{
			// 当前夹点是圆心，移动图形
			m_insertPt += offset;       // 对圆心执行偏移变换
		}
		if(idx == 1)
		{
			// 缩放圆
			// 1) 计算x轴的端点坐标
			AcGeVector3d v(AcGeVector3d::kXAxis);
			v.rotateBy(m_angle,AcGeVector3d::kZAxis);
			AcGePoint3d pt = m_insertPt + v*m_radius;

			// 2) 进行坐标偏移计算
			pt += offset;

			// 3) 计算新坐标与圆心之间的长度，并作为圆半径
			// 坐标相减，得到一个向量，然后得到向量长度
			m_radius = (pt - m_insertPt).length();
		}
	}

	return Acad::eOk;
}

//static void DividArc( const AcGePoint3d& spt, const AcGePoint3d& ept, const AcGePoint3d& thirdPt,
//					 int count, AcGePoint3dArray& pts )
//{
//	pts.append( spt );
//
//	AcGeCircArc3d arc( spt, thirdPt, ept );
//	AcGePoint3d cnt = arc.center();
//	//	double radius = arc.radius();
//	AcGeVector3d v1 = spt - cnt;
//	AcGeVector3d v2 = ept - cnt;
//	int c = ( arc.normal().z > 0 ? 1 : -1 ); // 弧线方向
//
//	double a1 = v1.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
//	double a2 = v2.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
//
//	double dq = abs( a2 - a1 ) / count;
//	//acutPrintf(_T("\na1:%.3f, a2:%.3f dq:%.3f c:%d"), a1, a2, dq, c);
//	for( int i = 1; i < count; i++ )
//	{
//		v1.rotateBy( c * dq, AcGeVector3d::kZAxis );
//		pts.append( cnt + v1 );
//		//acutPrintf(_T("\n点%d:(%d,%d)"),i,pts[i].x,pts[i].y);
//	}
//	pts.append( ept );
//}
//
//void FlowSensorGEDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
//{
//	AcGeVector3d v0( AcGeVector3d::kXAxis );
//	//AcGeCircArc3d arc( m_insertPt, v,m_radius );
//	v0.rotateBy( m_angle, AcGeVector3d::kZAxis );
//	//AcGePoint3d cnt = arc.center();
//	AcGePoint3d spt = m_insertPt + v0*m_radius;
//	AcGePoint3d ept = m_insertPt - v0*m_radius;
//	v0.rotateBy(3.1415926/4,AcGeVector3d::kZAxis);
//	AcGePoint3d pt = m_insertPt + v0*m_radius;
//
//	AcGeCircArc3d arc( spt, pt, ept );
//	AcGePoint3d cnt = arc.center();
//	double radius = arc.radius();
//
//	AcGeVector3d v = pt - cnt;
//	v.normalize();
//
//	DividArc( spt, ept, cnt + v * ( radius ), 90, pts );
//	DividArc( ept, spt, cnt - v * ( radius ), 90, pts );
//}

void FlowSensorGEDraw::regPropertyDataNames( AcStringArray& names ) const
{
	names.append(_T("流量"));
}

void FlowSensorGEDraw::readPropertyDataFromGE( const AcStringArray& values )
{
	CString strLenth;
	strLenth.Format(_T("%s"),values[0].kACharPtr());
	m_temp = strLenth;
	//m_radius = _tstof(strLenth);
	//if( 0 >= m_radius ) m_radius = 6;
}