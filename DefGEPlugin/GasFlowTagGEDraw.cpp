#include "StdAfx.h"
#include "GasFlowTagGEDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( GasFlowTagGEDraw, MineGEDraw, 1 )

GasFlowTagGEDraw::GasFlowTagGEDraw ()
{
}

GasFlowTagGEDraw::~GasFlowTagGEDraw ()
{
}

void GasFlowTagGEDraw::setAllExtraParamsToDefault()
{
	m_lenth = 4;
}

void GasFlowTagGEDraw::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble( m_angle );
}

void GasFlowTagGEDraw::readExtraParam( DrawParamReader& reader )
{
	reader.readDouble( m_lenth );
}
void GasFlowTagGEDraw::writeKeyParam( DrawParamWriter& writer )
{
	writer.writePoint( m_insertPt );
	writer.writeDouble( m_angle );
}

void GasFlowTagGEDraw::writeExtraParam( DrawParamWriter& writer )
{
	writer.writeDouble( m_lenth );
}
void GasFlowTagGEDraw::regPropertyDataNames( AcStringArray& names ) const
{
	names.append( _T( "瓦斯流量" ) );
}

void GasFlowTagGEDraw::readPropertyDataFromGE( const AcStringArray& values )
{
	Q = values[0].kACharPtr();
}

static void DrawLabelText( AcGiWorldDraw* mode, const AcGePoint3d& insertPt,double angle, double height, const CString& Q )
{
	double q = _tstof( Q );

	// {\\H0.7x;\\S2^;}表示上标xx^2
	CString str;

	str.Format( _T( "%.2fm%s/min" ), q , MakeUpperText( _T( "3" ) ) ); // m^3/min
	DrawMText( mode, insertPt, angle , str, height, AcDbMText::kMiddleCenter );
}

Adesk::Boolean GasFlowTagGEDraw::subWorldDraw( AcGiWorldDraw* mode )
{
	assertReadEnabled () ;

	// 绘制一个矩形框
	//DrawRect( mode, m_insertPt, m_angle, m_width, m_height, false ); // 不填充

	AcGeVector3d v( AcGeVector3d::kXAxis );
	v.rotateBy( m_angle, AcGeVector3d::kZAxis );

	if( v.x < 0 ) v.negate();
	m_angle = v.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

	v.normalize();
	v.rotateBy( PI / 2, AcGeVector3d::kZAxis ); // 始终与文字反向

	AcGePoint3d pt = m_insertPt + 0.5 * m_lenth * v;

	DrawLabelText( mode, pt, m_angle, m_lenth * 0.5 , Q );
	
	return Adesk::kTrue;
}

Acad::ErrorStatus GasFlowTagGEDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;

	// 插入点作为夹点
	gripPoints.append( m_insertPt );

	return Acad::eOk;
}

Acad::ErrorStatus GasFlowTagGEDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
	assertWriteEnabled () ;

	for( int i = 0; i < indices.length(); i++ )
	{
		int idx = indices.at( i );
		// 插入点
		if ( idx == 0 ) m_insertPt += offset;
	}
	return Acad::eOk;
}

void GasFlowTagGEDraw::configExtraParams()
{

}

void GasFlowTagGEDraw::updateExtraParams()
{

}