#include "StdAfx.h"
#include "FanTagGEDraw.h"

#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( FanTagGEDraw, MineGEDraw, 1 )

FanTagGEDraw::FanTagGEDraw() : MineGEDraw ()
{
}

FanTagGEDraw::~FanTagGEDraw( void )
{
}

void FanTagGEDraw::setAllExtraParamsToDefault()
{
	m_distance = 20;
	m_pt1 = AcGePoint3d(0,0,0);
	m_pt2 = AcGePoint3d(0,0,0);
}

void FanTagGEDraw::configExtraParams()
{

}

void FanTagGEDraw::updateExtraParams()
{

}

void FanTagGEDraw::writeKeyParam( DrawParamWriter& writer )
{
	writer.writePoint( m_insertPt );
	writer.writeDouble( m_q );
	writer.writeDouble( m_h );
	writer.writeString(m_name);
	writer.writeString(m_way);
}

void FanTagGEDraw::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble( m_q );
	reader.readDouble( m_h );
	reader.readString(m_name);
	reader.readString(m_way);
}

void FanTagGEDraw::readExtraParam( DrawParamReader& reader )
{
	reader.readDouble( m_distance );
}

void FanTagGEDraw::writeExtraParam( DrawParamWriter& writer )
{
	writer.writeDouble( m_distance );
}

Adesk::Boolean FanTagGEDraw::subWorldDraw( AcGiWorldDraw* mode )
{
	assertReadEnabled () ;

	drawFanTag(mode);
	drawText(mode);
	return Adesk::kTrue;
}

Acad::ErrorStatus FanTagGEDraw::subTransformBy( const AcGeMatrix3d& xform )
{
	// 插入点变换
	m_insertPt.transformBy( xform );

	//AcGeVector3d v( AcGeVector3d::kXAxis );
	//v.rotateBy( m_angle, AcGeVector3d::kZAxis );
	//v.transformBy( xform );

	//m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis ); // 变换后的旋转角度

	return Acad::eOk;
}

Acad::ErrorStatus FanTagGEDraw::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d& pickPoint,
	const AcGePoint3d& lastPoint,
	const AcGeMatrix3d& viewXform,
	AcGePoint3dArray& snapPoints,
	AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;
	// 捕捉2种类型的点：端点和中心点
	if( osnapMode != AcDb::kOsModeCen )
		return Acad::eOk;

	Acad::ErrorStatus es = Acad::eOk;

	if( osnapMode == AcDb::kOsModeCen )
	{
		snapPoints.append( m_insertPt );
	}

	return es;
}

Acad::ErrorStatus FanTagGEDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;

	gripPoints.append( m_insertPt );

	return Acad::eOk;
}

Acad::ErrorStatus FanTagGEDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
	assertWriteEnabled () ;

	for( int i = 0; i < indices.length(); i++ )
	{
		int idx = indices.at( i );
		if ( idx == 0 )
		{
			m_insertPt += offset; 			// 插入点偏移
		}
	}
	return Acad::eOk;
}

void FanTagGEDraw::drawFanTag(AcGiWorldDraw* mode)
{
	AcGeVector3d v1( AcGeVector3d::kXAxis );
	//v1.rotateBy( m_angle, AcGeVector3d::kZAxis ); 
	v1.rotateBy(PI / 4,AcGeVector3d::kZAxis);
	v1.normalize();

	AcGePoint3d pt1 = m_insertPt + v1 * m_distance;
	m_pt1 = pt1;

	v1.rotateBy(- PI / 4,AcGeVector3d::kZAxis);
	AcGePoint3d pt2 = pt1 + v1 * m_distance / 4;
	m_pt2 = pt2;
	DrawLine(mode,m_insertPt,pt1);
	DrawLine(mode,pt1,pt2);
}

void FanTagGEDraw::drawText( AcGiWorldDraw *mode )
{
	// 绘制文字
	AcGeVector3d v = m_pt2 - m_pt1;
	AcGePoint3d pt = m_pt2; // 中心点

	if( v.x < 0 ) v.negate();
	double angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

	//v.normalize();
	//v.rotateBy( PI / 2, AcGeVector3d::kZAxis ); 
	CString msg = _T( "建议型号:" );
	CString qStr,hStr;
	if (m_name.IsEmpty())
	{
		m_name = _T("没有选择风机");
	}
	msg.Append(m_name);
	msg.Append(_T("\n工作方式:"));
	msg.Append(m_way);
	msg.Append(_T("\n工作风量:"));
	qStr.Format(_T("%.2lf"),m_q*60);
	hStr.Format(_T("%.2lf"),m_h);

	qStr.Append(_T("m3/min\n工作风压:"));
	hStr.Append(_T("Pa"));

	msg.Append(qStr);
	msg.Append(hStr);
	DrawMText( mode, pt, angle, msg, 4 * 0.618, AcDbMText::kMiddleLeft );

}
