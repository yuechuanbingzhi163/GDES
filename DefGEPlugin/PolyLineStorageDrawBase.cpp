#include "StdAfx.h"
#include "PolyLineStorageDrawBase.h"
//#include "DoubleTunnelDraw_ConfigDlg.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

#include <cmath>

ACRX_NO_CONS_DEFINE_MEMBERS ( PolyLineStorageDrawBase, PolyLineTunnelDraw )

PolyLineStorageDrawBase::PolyLineStorageDrawBase( const CString& text ) : m_text( text )
{
}

void PolyLineStorageDrawBase::setAllExtraParamsToDefault()
{
    PolyLineTunnelDraw::setAllExtraParamsToDefault();
    m_boxHeight = 100;
    m_minLength = 150;
}

void PolyLineStorageDrawBase::configExtraParams()
{
}

void PolyLineStorageDrawBase::updateExtraParams()
{
    PolyLineTunnelDraw::updateExtraParams();
    updateBoxWidth();
}

void PolyLineStorageDrawBase::updateBoxWidth()
{
    AcGeVector3d v = m_endPt - m_startPt;
    double c = 0.618;
    m_boxWidth = v.length() * ( 1 - c );
}

void PolyLineStorageDrawBase::readExtraParam( DrawParamReader& reader )
{
    PolyLineTunnelDraw::readExtraParam( reader );
    reader.readDouble( m_boxHeight );
    reader.readDouble( m_boxWidth );
    reader.readDouble( m_minLength );
}

void PolyLineStorageDrawBase::writeExtraParam( DrawParamWriter& writer )
{
    PolyLineTunnelDraw::writeExtraParam( writer );
    writer.writeDouble( m_boxHeight );
    writer.writeDouble( m_boxWidth );
    writer.writeDouble( m_minLength );
}

void PolyLineStorageDrawBase::drawBox( AcGiWorldDraw* mode )
{
    AcGeVector3d v = m_endPt - m_startPt;
    double L = v.length();
    if( m_boxWidth < m_minLength ) return; // 如果小于m_minLength，则不显示文字框

    v.normalize();
    AcGeVector3d v2( v );
    v2.rotateBy( PI / 2, AcGeVector3d::kZAxis );
    AcGePoint3d boxPt1 = m_startPt + v * ( L - m_boxWidth ) / 2 + v2 * m_width / 2;
    AcGePoint3d boxPt2 = boxPt1 + v2 * m_boxHeight;

    AcGePoint3d boxPt4 = m_startPt + v * ( L + m_boxWidth ) / 2 + v2 * m_width / 2;
    AcGePoint3d boxPt3 = boxPt4 + v2 * m_boxHeight;

    // 绘制box
    AcGePoint3dArray pts;
    pts.append( boxPt1 );
    pts.append( boxPt2 );
    pts.append( boxPt3 );
    pts.append( boxPt4 );

    //mode->geometry().polygon(pts.length(), pts.asArrayPtr());
    mode->geometry().polyline( pts.length(), pts.asArrayPtr() );
}


//	AcGeVector3d v = m_endPt - m_startPt;
//	double L = v.length();
//	if(m_boxWidth < m_minLength) return; // 如果小于m_minLength，则不显示文字框
//
//	v.normalize();
//	double angle = v.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis);
//
//	AcGeVector3d v2(v);
//	v2.rotateBy(PI/2, AcGeVector3d::kZAxis);
//
//	double c = 0.618; // 黄金比例
//	DrawMText(mode, m_startPt + v*L*0.5 + v2*(m_width+m_boxHeight)*0.5, angle, m_text, m_boxHeight*c);

void PolyLineStorageDrawBase::drawText( AcGiWorldDraw* mode )
{
    // 绘制文字
    AcGeVector3d v = m_endPt - m_startPt;
    AcGePoint3d pt = m_startPt + v * 0.5; // 中心点

    if( v.x < 0 ) v.negate();
    double angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    v.normalize();
    v.rotateBy( PI / 2, AcGeVector3d::kZAxis ); // 始终与文字反向
    pt += v * m_width * 0.5;

    DrawMText( mode, pt, angle, _T( "回采工作面" ), m_width * 0.618, AcDbMText::kBottomCenter );
}

Adesk::Boolean PolyLineStorageDrawBase::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled();

//	PolyLineTunnelDraw::subWorldDraw(mode);
    DrawPolyLine( mode, m_startPt, m_endPt, m_width );
    DrawJoint( mode, m_startPt, m_width * 0.5, jdt ); // 绘制始节点
    DrawJoint( mode, m_endPt, m_width * 0.5, jdt ); // 绘制末节点

    //drawBox(mode);     // 绘制文字框
    drawText( mode );

    return Adesk::kTrue;
}