#include "StdAfx.h"
#include "QTagGEDraw.h"

#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS ( QTagGEDraw, MineGEDraw, 1 )

QTagGEDraw::QTagGEDraw() : MineGEDraw ()
{
}

QTagGEDraw::~QTagGEDraw( void )
{
}

void QTagGEDraw::setAllExtraParamsToDefault()
{
}

void QTagGEDraw::configExtraParams()
{

}

void QTagGEDraw::updateExtraParams()
{

}

void QTagGEDraw::writeKeyParam( DrawParamWriter& writer )
{
	writer.writePoint( m_insertPt );
	writer.writeDouble( m_q );
}

void QTagGEDraw::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble( m_q );
}

void QTagGEDraw::readExtraParam( DrawParamReader& reader )
{
}

void QTagGEDraw::writeExtraParam( DrawParamWriter& writer )
{
}

Adesk::Boolean QTagGEDraw::subWorldDraw( AcGiWorldDraw* mode )
{
	assertReadEnabled () ;

	drawText(mode);
	return Adesk::kTrue;
}

Acad::ErrorStatus QTagGEDraw::subTransformBy( const AcGeMatrix3d& xform )
{
	// 插入点变换
	m_insertPt.transformBy( xform );
	return Acad::eOk;
}

Acad::ErrorStatus QTagGEDraw::subGetOsnapPoints (
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

Acad::ErrorStatus QTagGEDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;

	gripPoints.append( m_insertPt );

	return Acad::eOk;
}

Acad::ErrorStatus QTagGEDraw::subMoveGripPointsAt( const AcDbIntArray& indices, const AcGeVector3d& offset )
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

void QTagGEDraw::drawText( AcGiWorldDraw *mode )
{
	CString msg = _T( "Q=" );
	CString qStr;
	qStr.Format(_T("%.2lf"),m_q * 60);
	qStr.Append(_T("m3/min"));

	msg.Append(qStr);
	mode->subEntityTraits().setColor(1);
	DrawMText( mode, m_insertPt, 0, msg, 3 * 0.618, AcDbMText::kMiddleCenter );

}
