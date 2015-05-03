#include "StdAfx.h"
#include "TailraceDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

ACRX_CONS_DEFINE_MEMBERS ( TailraceGEDraw, MineGEDraw, 1 )

TailraceGEDraw::TailraceGEDraw()
{
}

TailraceGEDraw::~TailraceGEDraw( void )
{
}

void TailraceGEDraw::setAllExtraParamsToDefault()
{
	m_lenth = 3;
}

void TailraceGEDraw::configExtraParams()
{

}

void TailraceGEDraw::updateExtraParams()
{
	caculPts();
}

void TailraceGEDraw::writeKeyParam( DrawParamWriter& writer )
{
	writer.writePoint( m_insertPt );
	writer.writeDouble( m_angle );
}

void TailraceGEDraw::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble( m_angle );
}

void TailraceGEDraw::readExtraParam( DrawParamReader& reader )
{
	reader.readDouble( m_lenth );
	reader.readPoint(m_pt0);
	reader.readPoint(m_pt1);
	reader.readPoint(m_pt2);
	reader.readPoint(m_pt3);
	reader.readPoint(m_pt4);
	reader.readPoint(m_pt5);
	reader.readPoint(m_pt6);
	reader.readPoint(m_pt7);
	reader.readPoint(m_pt8);
	reader.readPoint(m_pt9);
	reader.readPoint(m_pt10);
	reader.readPoint(m_pt11);
	reader.readPoint(m_pt12);
	reader.readPoint(m_pt13);
	reader.readPoint(m_pt14);
	reader.readPoint(m_pt15);
}

void TailraceGEDraw::writeExtraParam( DrawParamWriter& writer )
{
	writer.writeDouble( m_lenth );
	writer.writePoint(m_pt0);
	writer.writePoint(m_pt1);
	writer.writePoint(m_pt2);
	writer.writePoint(m_pt3);
	writer.writePoint(m_pt4);
	writer.writePoint(m_pt5);
	writer.writePoint(m_pt6);
	writer.writePoint(m_pt7);
	writer.writePoint(m_pt8);
	writer.writePoint(m_pt9);
	writer.writePoint(m_pt10);
	writer.writePoint(m_pt11);
	writer.writePoint(m_pt12);
	writer.writePoint(m_pt13);
	writer.writePoint(m_pt14);
	writer.writePoint(m_pt15);
}

Adesk::Boolean TailraceGEDraw::subWorldDraw( AcGiWorldDraw* mode )
{
	assertReadEnabled () ;

	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( m_angle, AcGeVector3d::kZAxis );

	if( v.x < 0 ) v.negate();
	m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

	caculPts();

	DrawLine(mode,m_insertPt,m_pt0);
	DrawLine(mode,m_pt2,m_pt3);
	DrawLine(mode,m_pt3,m_pt5);
	DrawLine(mode,m_pt5,m_pt8);
	DrawLine(mode,m_pt2,m_pt15);
	DrawLine(mode,m_pt14,m_pt15);
	DrawLine(mode,m_pt13,m_pt14);
	DrawLine(mode,m_pt13,m_pt12);
	DrawLine(mode,m_pt7,m_pt10);
	DrawLine(mode,m_pt6,m_pt11);

	DrawArc(mode,m_pt2,m_pt0,m_pt3,false);
	AcGePoint3dArray pts;
	pts.append(m_pt4);
	pts.append(m_pt9);
	pts.append(m_pt15);
	pts.append(m_pt14);
	pts.append(m_pt13);
	pts.append(m_pt12);
	pts.append(m_pt8);
	pts.append(m_pt7);
	pts.append(m_pt5);

	DrawBackGround(mode,pts,7);
	return Adesk::kTrue;
}

Acad::ErrorStatus TailraceGEDraw::subTransformBy( const AcGeMatrix3d& xform )
{
	// 插入点变换
	m_insertPt.transformBy( xform );

	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( m_angle, AcGeVector3d::kZAxis );
	v.transformBy( xform );

	m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis ); // 变换后的旋转角度
	caculPts();

	return Acad::eOk;
}

Acad::ErrorStatus TailraceGEDraw::subGetOsnapPoints (
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

Acad::ErrorStatus TailraceGEDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;

	gripPoints.append( m_insertPt );
	//gripPoints.append(m_spt1);

	return Acad::eOk;
}

Acad::ErrorStatus TailraceGEDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
	assertWriteEnabled () ;

	for( int i = 0; i < indices.length(); i++ )
	{
		int idx = indices.at( i );
		if ( idx == 0 )
		{
			m_insertPt += offset; 			// 插入点偏移
			caculPts();
		}

// 		if(idx == 1)
// 		{
// 			// 1) 计算x轴的端点坐标
// 			AcGeVector3d v(AcGeVector3d::kXAxis);
// 			v.rotateBy(m_angle,AcGeVector3d::kZAxis);
// 			v.rotateBy(3.1415926/2,AcGeVector3d::kZAxis);
// 			AcGePoint3d pt = m_insertPt + v*m_lenth;
// 
// 			// 2) 进行坐标偏移计算
// 			pt += offset;
// 
// 			// 坐标相减，得到一个向量，然后得到向量长度
// 			m_lenth = (pt - m_insertPt).length();
// 			caculPts();
// 		}

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
void TailraceGEDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
	//AcGeCircArc3d arc( m_pt2, m_pt0, m_pt3 );
	//AcGePoint3d cnt = arc.center();
	//double radius = arc.radius();

	//AcGeVector3d v = pt - cnt;
	//v.normalize();

	DividArc( m_pt2, m_pt3, m_pt0, 90, pts );
	pts.append(m_pt8);
	pts.append(m_pt5);
	pts.append(m_pt4);
}

void TailraceGEDraw::caculPts()
{
	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( m_angle, AcGeVector3d::kZAxis );
	v.rotateBy( -0.5 * PI,AcGeVector3d::kZAxis);
	v.normalize();

	m_pt0 = m_insertPt + v*m_lenth / 4;
	m_pt1 = m_insertPt + v*m_lenth * 3 / 4;
	v.rotateBy(-PI * 0.5,AcGeVector3d::kZAxis);
	m_pt2 = m_pt1 + v * m_lenth / 2;
	v.rotateBy( PI, AcGeVector3d::kZAxis);
	m_pt3 = m_pt2 + v * m_lenth;
	v.rotateBy(-PI * 0.5,AcGeVector3d::kZAxis);
	m_pt4 = m_pt3 + v * m_lenth / 3;
	m_pt5 = m_pt4 + v * m_lenth / 3 * 2;
	v.rotateBy(-PI * 0.5,AcGeVector3d::kZAxis);
	m_pt6 = m_pt5 + v * m_lenth / 3;
	m_pt7 = m_pt6 + v * m_lenth / 3;
	m_pt8 = m_pt7 + v * m_lenth / 3;

	v.rotateBy(-PI * 0.5,AcGeVector3d::kZAxis);
	m_pt9 = m_pt8 + v * m_lenth / 3 * 2;
	m_pt12 = m_pt8 + v * m_lenth / 6;

	v.rotateBy(PI * 0.5,AcGeVector3d::kZAxis);
	m_pt13 = m_pt12 + v * m_lenth / 6;

	v.rotateBy(-PI * 0.5,AcGeVector3d::kZAxis);
	m_pt14 = m_pt13 + v * m_lenth / 6;

	v.rotateBy(-PI * 0.5,AcGeVector3d::kZAxis);
	m_pt15 = m_pt14 + v * m_lenth / 6;


	v.rotateBy(-PI * 3 / 4,AcGeVector3d::kZAxis);
	m_pt10 = m_pt7 + v * m_lenth / 4;
	v.rotateBy(PI * 0.5,AcGeVector3d::kZAxis);
	m_pt11 = m_pt6 + v * m_lenth / 4;

}

void TailraceGEDraw::regPropertyDataNames( AcStringArray& names ) const
{
	names.append(_T("大小"));
}

void TailraceGEDraw::readPropertyDataFromGE( const AcStringArray& values )
{
	CString strLenth;
	strLenth.Format(_T("%s"),values[0].kACharPtr());

	m_lenth = _tstof(strLenth);
	if( 0 >= m_lenth ) m_lenth = 3;
	caculPts();
}