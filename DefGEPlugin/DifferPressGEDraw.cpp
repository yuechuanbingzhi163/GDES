#include "StdAfx.h"
#include "DifferPressSensorGEDraw.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

ACRX_CONS_DEFINE_MEMBERS ( DifferPressSensorGEDraw, MineGEDraw, 1 )

DifferPressSensorGEDraw::DifferPressSensorGEDraw()
{

}

DifferPressSensorGEDraw::~DifferPressSensorGEDraw( void )
{
}

void DifferPressSensorGEDraw::setAllExtraParamsToDefault()
{
	m_radius = 3;
	m_temp = _T("");
}

void DifferPressSensorGEDraw::configExtraParams()
{

}

void DifferPressSensorGEDraw::updateExtraParams()
{

}

void DifferPressSensorGEDraw::writeKeyParam( DrawParamWriter& writer )
{
	writer.writePoint( m_insertPt );
	writer.writeDouble( m_angle );
}

void DifferPressSensorGEDraw::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble( m_angle );
}

void DifferPressSensorGEDraw::readExtraParam( DrawParamReader& reader )
{
	reader.readDouble( m_radius );
	reader.readString( m_temp );
}

void DifferPressSensorGEDraw::writeExtraParam( DrawParamWriter& writer )
{
	writer.writeDouble( m_radius );
	writer.writeString( m_temp );
}

Adesk::Boolean DifferPressSensorGEDraw::subWorldDraw( AcGiWorldDraw* mode )
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
		pt = pt + 0.2 * m_radius * v;
	}

	DrawMText(mode,pt,m_angle,_T("P"),0.68 * m_radius);

	v.rotateBy( PI,AcGeVector3d::kZAxis );
	pt = m_insertPt + 0.5 * m_radius * v;

	if(LESS_ZERO(m_temp)) return Adesk::kTrue;
	CString temp;
	DealSpPoints(m_temp);
	temp.Format(_T("%sP%s"), m_temp, MakeLowerText(_T("a")));
	DrawMText(mode,pt,m_angle, temp ,0.2 * m_radius);

	return Adesk::kTrue;
}

Acad::ErrorStatus DifferPressSensorGEDraw::subTransformBy( const AcGeMatrix3d& xform )
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


Acad::ErrorStatus DifferPressSensorGEDraw::subGetOsnapPoints (
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

Acad::ErrorStatus DifferPressSensorGEDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;

	gripPoints.append( m_insertPt );

	AcGeVector3d v(AcGeVector3d::kXAxis);
	v.rotateBy(m_angle,AcGeVector3d::kZAxis);
	v *= m_radius;

	gripPoints.append(m_insertPt + v);             // 正方向端点作为夹点

	return Acad::eOk;
}

Acad::ErrorStatus DifferPressSensorGEDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
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


void DifferPressSensorGEDraw::regPropertyDataNames( AcStringArray& names ) const
{
	names.append(_T("压差"));
}

void DifferPressSensorGEDraw::readPropertyDataFromGE( const AcStringArray& values )
{
	CString strLenth;
	strLenth.Format(_T("%s"),values[0].kACharPtr());
	m_temp = strLenth;
	//m_radius = _tstof(strLenth);
	//if( 0 >= m_radius ) m_radius = 6;
}