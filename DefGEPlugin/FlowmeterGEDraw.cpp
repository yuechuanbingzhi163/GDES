#include "StdAfx.h"
#include "FlowmeterGEDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

ACRX_CONS_DEFINE_MEMBERS ( FlowmeterGEDraw, MineGEDraw, 1 )

FlowmeterGEDraw::FlowmeterGEDraw()
{
}

FlowmeterGEDraw::~FlowmeterGEDraw( void )
{
}

void FlowmeterGEDraw::setAllExtraParamsToDefault()
{
	m_lenth = 5;
}

void FlowmeterGEDraw::configExtraParams()
{

}

void FlowmeterGEDraw::updateExtraParams()
{
	caculPts();
}

void FlowmeterGEDraw::writeKeyParam( DrawParamWriter& writer )
{
	writer.writePoint( m_insertPt );
	writer.writeDouble( m_angle );
}

void FlowmeterGEDraw::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble( m_angle );
}

void FlowmeterGEDraw::readExtraParam( DrawParamReader& reader )
{
	reader.readDouble( m_lenth );
	reader.readPoint(m_spt1);
	reader.readPoint(m_spt2);
	reader.readPoint(m_spt3);
	reader.readPoint(m_spt4);
	reader.readPoint(m_spt5);
}

void FlowmeterGEDraw::writeExtraParam( DrawParamWriter& writer )
{
	writer.writeDouble( m_lenth );
	writer.writePoint(m_spt1);
	writer.writePoint(m_spt2);
	writer.writePoint(m_spt3);
	writer.writePoint(m_spt4);
	writer.writePoint(m_spt5);
}

Adesk::Boolean FlowmeterGEDraw::subWorldDraw( AcGiWorldDraw* mode )
{
	assertReadEnabled () ;

	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( m_angle, AcGeVector3d::kZAxis );

	if( v.x < 0 ) v.negate();
	m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

	caculPts();

	DrawLine(mode,m_spt1,m_spt2);
	DrawLine(mode,m_spt2,m_spt3);
	DrawLine(mode,m_spt3,m_spt4);
	DrawLine(mode,m_spt4,m_spt1);
	DrawLine(mode,m_spt1,m_spt3);
	AcGePoint3dArray pts;
	pts.append(m_spt1);
	pts.append(m_spt4);
	pts.append(m_spt3);
	//caclBackGroundMinPolygon(pts);
	DrawBackGround(mode,pts,7);
	return Adesk::kTrue;

	return Adesk::kTrue;
}

Acad::ErrorStatus FlowmeterGEDraw::subTransformBy( const AcGeMatrix3d& xform )
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

Acad::ErrorStatus FlowmeterGEDraw::subGetOsnapPoints (
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

Acad::ErrorStatus FlowmeterGEDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;

	gripPoints.append( m_insertPt );

	//gripPoints.append(m_spt1);
	return Acad::eOk;
}

Acad::ErrorStatus FlowmeterGEDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
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
		//	v.rotateBy(3.1415926/2,AcGeVector3d::kZAxis);
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

void FlowmeterGEDraw::caculPts()
{
	double pi = 3.1415926;
	AcGePoint3d spt;
	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( m_angle, AcGeVector3d::kZAxis );
	v.rotateBy(pi * 0.5,AcGeVector3d::kZAxis);

	spt = m_insertPt + v*m_lenth / 2;
	v.rotateBy(-pi * 0.5,AcGeVector3d::kZAxis);
	m_spt1 = spt + v * m_lenth / 4;
	v.rotateBy(-pi * 0.5,AcGeVector3d::kZAxis);
	m_spt2 = m_spt1 + v * m_lenth;
	v.rotateBy(-pi * 0.5,AcGeVector3d::kZAxis);
	m_spt3 = m_spt2 + v * m_lenth / 2;
	v.rotateBy(-pi * 0.5,AcGeVector3d::kZAxis);
	m_spt4 = m_spt3 + v * m_lenth;
}

void FlowmeterGEDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
	pts.append(m_spt1);
	pts.append(m_spt2);
	pts.append(m_spt3);
	pts.append(m_spt4);
}

void FlowmeterGEDraw::regPropertyDataNames( AcStringArray& names ) const
{
	names.append(_T("大小"));
}

void FlowmeterGEDraw::readPropertyDataFromGE( const AcStringArray& values )
{
	CString strLenth;
	strLenth.Format(_T("%s"),values[0].kACharPtr());

	m_lenth = _tstof(strLenth);
	if( 0 >= m_lenth ) m_lenth = 10;
	caculPts();
}