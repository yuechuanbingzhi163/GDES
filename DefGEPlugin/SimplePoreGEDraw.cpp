#include "StdAfx.h"
#include "SimplePoreGEDraw.h"
#include "DrawTool.h"
#include "DrawSpecial.h"

ACRX_CONS_DEFINE_MEMBERS ( SimplePoreGEDraw, MineGEDraw, 1 )

SimplePoreGEDraw::SimplePoreGEDraw () : MineGEDraw ()
{
}

SimplePoreGEDraw::~SimplePoreGEDraw ()
{
}

void SimplePoreGEDraw::setAllExtraParamsToDefault()
{
	m_radius = 1;
}

void SimplePoreGEDraw::configExtraParams()
{

}
void SimplePoreGEDraw::updateExtraParams()
{

}

void SimplePoreGEDraw::readKeyParam(DrawParamReader& reader)
{
	reader.readPoint( m_insertPt );
}

void SimplePoreGEDraw::writeKeyParam(DrawParamWriter& writer)
{
	writer.writePoint(m_insertPt);
}

void SimplePoreGEDraw::readExtraParam( DrawParamReader& reader )
{
    //MineGEDraw::readExtraParam( reader );
}

void SimplePoreGEDraw::writeExtraParam( DrawParamWriter& writer )
{
    //MineGEDraw::writeExtraParam( writer );
}

void SimplePoreGEDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "编号" ) );
    names.append( _T( "孔径" ) );
}

void SimplePoreGEDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    id = values[0].kACharPtr();
	m_radius = abs(_tstof(values[1].kACharPtr()));
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

Adesk::Boolean SimplePoreGEDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    //绘制一个圆
	DrawCircle(mode, m_insertPt, m_radius, false);
	//绘制十字
	DrawCross(mode, m_insertPt, m_radius*0.382);

    // 绘制编号
    AcGePoint3d pt1, pt2;
	AcGePoint3d pt = CaclLeftBottomPt(m_insertPt, 0, m_radius, m_radius);
	DrawMText(mode, pt, 0, id, 2*m_radius);

    return Adesk::kTrue;
}

Acad::ErrorStatus SimplePoreGEDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

    return Acad::eOk;
}

Acad::ErrorStatus SimplePoreGEDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

    return Acad::eOk;
}

Acad::ErrorStatus SimplePoreGEDraw::subTransformBy( const AcGeMatrix3d& xform )
{
	m_insertPt.transformBy(xform);
	return Acad::eOk;
}

Acad::ErrorStatus SimplePoreGEDraw::subGetOsnapPoints (
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
	if( osnapMode != AcDb::kOsMaskCen ) return Acad::eOk;

	if( osnapMode == AcDb::kOsMaskCen )
	{
		snapPoints.append(m_insertPt);
	}

	return Acad::eOk;
}