#include "StdAfx.h"
#include "EffectRanTagGEDraw.h"

#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( EffectRanTagGEDraw, MineGEDraw, 1 )

EffectRanTagGEDraw::EffectRanTagGEDraw() : MineGEDraw ()
{
}

EffectRanTagGEDraw::~EffectRanTagGEDraw( void )
{
}

void EffectRanTagGEDraw::setAllExtraParamsToDefault()
{
	//m_distance = 10;
}

void EffectRanTagGEDraw::configExtraParams()
{

}

void EffectRanTagGEDraw::updateExtraParams()
{

}

void EffectRanTagGEDraw::writeKeyParam( DrawParamWriter& writer )
{
	writer.writePoint( m_insertPt );
	writer.writeDouble(m_angle);
	writer.writeDouble( m_minDistan );
	writer.writeDouble( m_maxDistan );
	writer.writeDouble( m_distance );
}

void EffectRanTagGEDraw::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble(m_angle);
	reader.readDouble( m_minDistan );
	reader.readDouble( m_maxDistan );
	reader.readDouble( m_distance );
}

void EffectRanTagGEDraw::readExtraParam( DrawParamReader& reader )
{
}

void EffectRanTagGEDraw::writeExtraParam( DrawParamWriter& writer )
{
}

Adesk::Boolean EffectRanTagGEDraw::subWorldDraw( AcGiWorldDraw* mode )
{
	assertReadEnabled () ;

	drawTag(mode);
	drawText(mode);
	return Adesk::kTrue;
}

void EffectRanTagGEDraw::drawTag(AcGiWorldDraw* mode)
{
	AcGePoint3dArray pts;
	drawTagInit(pts);
	DrawLine(mode,pts[0],pts[1]); //靠近工作面的辅助线
	DrawLine(mode,pts[2],pts[3]); //靠近风筒的辅助线
	DrawLine(mode,pts[4],pts[5]); //中间带箭头的线
	DrawLine(mode,pts[5],pts[8]); //箭头线
	DrawLine(mode,pts[5],pts[9]); //箭头线
	DrawLine(mode,pts[4],pts[6]); //箭头线
	DrawLine(mode,pts[4],pts[7]); //箭头线
}

void EffectRanTagGEDraw::drawText( AcGiWorldDraw *mode )
{
	// 绘制文字
	AcGeVector3d v(AcGeVector3d::kXAxis);
	v.rotateBy(m_angle,AcGeVector3d::kZAxis);
	if(v.x < 0) v.negate();
	double angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

	CString minDis,maxDis;
	minDis.Format(_T("%.2lf"),m_minDistan);
	maxDis.Format(_T("%.2lf"),m_maxDistan);
	
	CString msg = _T("≤");

	if( 0 == m_minDistan )
	{
		msg.Append(maxDis);
		msg.Append(_T("m"));
	}
	else
	{
		msg.Append(minDis);
		msg.Append(_T("m~"));
		msg.Append(maxDis);
		msg.Append(_T("m"));
	}
	DrawMText( mode, m_centrPt, angle, msg, m_distance* 0.1 * 0.618, AcDbMText::kBottomCenter );

}

void EffectRanTagGEDraw::drawTagInit(AcGePoint3dArray& pts)
{
	AcGeVector3d v1( AcGeVector3d::kXAxis);
	v1.rotateBy(m_angle,AcGeVector3d::kZAxis);
	m_pt = m_insertPt - v1*m_distance;

	if(v1.x < 0) v1.negate();
	v1.rotateBy( PI / 2,AcGeVector3d::kZAxis);
	v1.normalize();

	AcGePoint3d pt1 = m_insertPt + v1 * 20;
	AcGePoint3d pt2 = pt1 + v1 * m_distance*0.1;

	AcGeVector3d v2( AcGeVector3d::kXAxis);
	v2.rotateBy(m_angle,AcGeVector3d::kZAxis);
	if(v2.x < 0) v2.negate();
	v2.rotateBy( PI / 2,AcGeVector3d::kZAxis);
	v2.normalize();

	AcGePoint3d pt3 = m_pt + v2 * 20;
	AcGePoint3d pt4 = pt3 + v2* m_distance*0.1;
	AcGePoint3d pt5 = AcGePoint3d((pt1.x+pt2.x)*0.5,(pt1.y+pt2.y)*0.5,0);
	AcGePoint3d pt6 = AcGePoint3d((pt3.x+pt4.x)*0.5,(pt3.y+pt4.y)*0.5,0);

	v1 = pt5 - pt6;
	m_centrPt = pt6 + v1 * 0.5;

	v1.rotateBy(PI / 6,AcGeVector3d::kZAxis);
	v1.normalize();
	AcGePoint3d pt9 = pt6 + v1 * m_distance*0.05;

	v1.rotateBy(-PI / 3,AcGeVector3d::kZAxis);
	AcGePoint3d pt10 = pt6 + v1 * m_distance*0.05;

	v1 = pt6 - pt5;
	v1.rotateBy(-PI / 6,AcGeVector3d::kZAxis);
	v1.normalize();
	AcGePoint3d pt7 = pt5 + v1*m_distance*0.05;

	v1.rotateBy(PI / 3,AcGeVector3d::kZAxis);
	AcGePoint3d pt8 = pt5 + v1*m_distance*0.05;

	pts.append(pt1);
	pts.append(pt2);
	pts.append(pt3);
	pts.append(pt4);
	pts.append(pt5);
	pts.append(pt6);
	pts.append(pt7);
	pts.append(pt8);
	pts.append(pt9);
	pts.append(pt10);
}
