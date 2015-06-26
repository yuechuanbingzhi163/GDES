#include "StdAfx.h"
#include "SimplePoreDraw.h"
#include "DrawTool.h"
#include "DrawSpecial.h"

#include "../ArxHelper/HelperClass.h"

ACRX_CONS_DEFINE_MEMBERS ( SimplePoreDraw, MineGEDraw, 1 )

SimplePoreDraw::SimplePoreDraw () : MineGEDraw ()
{
}

SimplePoreDraw::~SimplePoreDraw ()
{
}

void SimplePoreDraw::setAllExtraParamsToDefault()
{
	m_pore_size = 1;
	m_id = _T("");
}

void SimplePoreDraw::configExtraParams()
{

}
void SimplePoreDraw::updateExtraParams()
{

}

void SimplePoreDraw::readKeyParam(DrawParamReader& reader)
{
	reader.readPoint( m_insertPt );
}

void SimplePoreDraw::writeKeyParam(DrawParamWriter& writer)
{
	writer.writePoint(m_insertPt);
}

void SimplePoreDraw::readExtraParam( DrawParamReader& reader )
{
    //MineGEDraw::readExtraParam( reader );
}

void SimplePoreDraw::writeExtraParam( DrawParamWriter& writer )
{
    //MineGEDraw::writeExtraParam( writer );
}

void SimplePoreDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "编号" ) );
    names.append( _T( "孔径" ) );
	//names.append( _T( "$坐标" ) );
}

void SimplePoreDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    m_id = values[0].kACharPtr();
	m_pore_size = abs(_tstof(values[1].kACharPtr()));
	//ArxUtilHelper::StringToPoint3d(values[2].kACharPtr(), m_insertPt);
}

static AcGePoint3d CaclLeftBottomPt( const AcGePoint3d& pt, double angle, double width, double height )
{
    AcGeVector3d v( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );
    v2.rotateBy( angle + PI / 2, AcGeVector3d::kZAxis );
    if( v.x > 0 ) // 位于第1和4象限
    {
        v.rotateBy( PI, AcGeVector3d::kZAxis );
        v2.rotateBy( PI, AcGeVector3d::kZAxis );
    }
    return ( pt + v * width / 2 + v2 * height / 2 );
}

Adesk::Boolean SimplePoreDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    //绘制一个圆
	DrawCircle(mode, m_insertPt, m_pore_size*0.5, false);
	//绘制十字
	//DrawCross(mode, m_insertPt, m_pore_size*0.382);

    // 绘制编号
    AcGePoint3d pt1, pt2;
	AcGePoint3d pt = CaclLeftBottomPt(m_insertPt, 0, m_pore_size*0.5, m_pore_size*0.5);
	//DrawMText(mode, pt, 0, m_id, 2*m_pore_size);

    return Adesk::kTrue;
}

Acad::ErrorStatus SimplePoreDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

	gripPoints.append(m_insertPt);

    return Acad::eOk;
}

Acad::ErrorStatus SimplePoreDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

	for( int i = 0; i < indices.length(); i++ )
	{
		int idx = indices.at( i );

		if ( idx == 0 )
		{
			m_insertPt += offset;
		}
	}
    return Acad::eOk;
}

Acad::ErrorStatus SimplePoreDraw::subTransformBy( const AcGeMatrix3d& xform )
{
	m_insertPt.transformBy(xform);
	return Acad::eOk;
}

Acad::ErrorStatus SimplePoreDraw::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d& pickPoint,
	const AcGePoint3d& lastPoint,
	const AcGeMatrix3d& viewXform,
	AcGePoint3dArray& snapPoints,
	AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;
	// 只捕捉1种类型的点：端点
	if( osnapMode != AcDb::kOsMaskEnd ) return Acad::eOk;

	if( osnapMode == AcDb::kOsMaskEnd )
	{
		snapPoints.append(m_insertPt);
	}

	return Acad::eOk;
}