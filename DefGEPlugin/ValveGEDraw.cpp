#include "StdAfx.h"
#include "ValveGEDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

ACRX_CONS_DEFINE_MEMBERS ( ValveGEDraw, MineGEDraw, 1 )

ValveGEDraw::ValveGEDraw()
{
}

ValveGEDraw::~ValveGEDraw( void )
{
}

void ValveGEDraw::setAllExtraParamsToDefault()
{
	m_lenth = 4;
}

void ValveGEDraw::configExtraParams()
{

}

void ValveGEDraw::updateExtraParams()
{
	caculPts();
}

void ValveGEDraw::writeKeyParam( DrawParamWriter& writer )
{
	writer.writePoint( m_insertPt );
	writer.writeDouble( m_angle );
}

void ValveGEDraw::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble( m_angle );
}

void ValveGEDraw::readExtraParam( DrawParamReader& reader )
{
	reader.readDouble( m_lenth );
	reader.readPoint(m_spt1);
	reader.readPoint(m_spt2);
	reader.readPoint(m_spt3);
	reader.readPoint(m_spt4);
	reader.readPoint(m_spt5);
	reader.readPoint(m_spt6);
	reader.readPoint(m_spt7);
}

void ValveGEDraw::writeExtraParam( DrawParamWriter& writer )
{
	writer.writeDouble( m_lenth );
	writer.writePoint(m_spt1);
	writer.writePoint(m_spt2);
	writer.writePoint(m_spt3);
	writer.writePoint(m_spt4);
	writer.writePoint(m_spt5);
	writer.writePoint(m_spt6);
	writer.writePoint(m_spt7);
}

Adesk::Boolean ValveGEDraw::subWorldDraw( AcGiWorldDraw* mode )
{
	assertReadEnabled () ;


	caculPts();
	DrawLine(mode,m_insertPt,m_spt1);
	DrawLine(mode,m_insertPt,m_spt2);
	DrawLine(mode,m_insertPt,m_spt3);
	DrawLine(mode,m_insertPt,m_spt4);
	DrawLine(mode,m_insertPt,m_spt5);
	DrawLine(mode,m_spt1,m_spt2);
	DrawLine(mode,m_spt3,m_spt4);
	DrawLine(mode,m_spt6,m_spt7);

	return Adesk::kTrue;
}

Acad::ErrorStatus ValveGEDraw::subTransformBy( const AcGeMatrix3d& xform )
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

Acad::ErrorStatus ValveGEDraw::subGetOsnapPoints (
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

Acad::ErrorStatus ValveGEDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;

	gripPoints.append( m_insertPt );

	//gripPoints.append(m_spt1);             // 正方向上端端点作为夹点

	return Acad::eOk;
}

Acad::ErrorStatus ValveGEDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
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

		//if(idx == 1)
		//{
		//	// 1) 计算x轴的端点坐标
		//	AcGeVector3d v(AcGeVector3d::kXAxis);
		//	v.rotateBy(m_angle,AcGeVector3d::kZAxis);
		//	v.rotateBy(3.1415926/6,AcGeVector3d::kZAxis);
		//	AcGePoint3d pt = m_insertPt + v*m_lenth;

		//	// 2) 进行坐标偏移计算
		//	pt += offset;

		//	// 坐标相减，得到一个向量，然后得到向量长度
		//	m_lenth = (pt - m_insertPt).length();
		//	caculPts();
		//}

	}
	return Acad::eOk;
}

void ValveGEDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
	pts.append(m_spt1);
	pts.append(m_spt2);
	pts.append(m_spt3);
	pts.append(m_spt4);
}

void ValveGEDraw::caculPts()
{
	double pi = 3.1415926;
	//AcGePoint3d spt1,spt2,spt3,spt4,spt5,spt6,spt7;
	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( m_angle, AcGeVector3d::kZAxis );
	v.rotateBy(pi/6,AcGeVector3d::kZAxis);

	m_spt1 = m_insertPt + v*m_lenth;
	v.rotateBy(-2*pi/3,AcGeVector3d::kZAxis);
	m_spt2 = m_spt1 + v * m_lenth;
	v.rotateBy(-pi/3,AcGeVector3d::kZAxis);
	m_spt3 = m_insertPt + v * m_lenth;
	v.rotateBy(-2*pi/3,AcGeVector3d::kZAxis);
	m_spt4 = m_spt3 + v * m_lenth;
	m_spt5 = m_insertPt + v * m_lenth * 3 / 4;
	v.rotateBy(pi * 0.5,AcGeVector3d::kZAxis);
	m_spt6 = m_spt5 + v * 1.7320508075688772 * m_lenth / 4;
	v.rotateBy(pi,AcGeVector3d::kZAxis);
	m_spt7 = m_spt5 + v * m_lenth * 1.7320508075688772 / 4;
}

void ValveGEDraw::readPropertyDataFromGE( const AcStringArray& values )
{
	CString strLenth;
	strLenth.Format(_T("%s"),values[0].kACharPtr());

	m_lenth = _tstof(strLenth);
	if( 0 >= m_lenth ) m_lenth = 5;
	caculPts();
	// 	acutPrintf(_T("\n长度(字符串):%s"),strLenth);
	// 	acutPrintf(_T("\n长度:%lf"),m_lenth );
}

void ValveGEDraw::regPropertyDataNames( AcStringArray& names ) const
{
	names.append(_T("大小"));
}