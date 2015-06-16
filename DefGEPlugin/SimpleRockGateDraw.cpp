#include "StdAfx.h"
#include "SimpleRockGateDraw.h"
#include "DrawTool.h"
#include "DrawSpecial.h"

#include "../ArxHelper/HelperClass.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleRockGateDraw, MineGEDraw, 1 )

SimpleRockGateDraw::SimpleRockGateDraw () : MineGEDraw ()
{
}

SimpleRockGateDraw::~SimpleRockGateDraw ()
{
}

void SimpleRockGateDraw::setAllExtraParamsToDefault()
{
}

void SimpleRockGateDraw::configExtraParams()
{

}
void SimpleRockGateDraw::updateExtraParams()
{
}

void SimpleRockGateDraw::readKeyParam(DrawParamReader& reader)
{
	reader.readPoint(m_insertPt);
}

void SimpleRockGateDraw::writeKeyParam(DrawParamWriter& writer)
{
	writer.writePoint(m_insertPt);
}

void SimpleRockGateDraw::readExtraParam( DrawParamReader& reader )
{
    //MineGEDraw::readExtraParam( reader );
}

void SimpleRockGateDraw::writeExtraParam( DrawParamWriter& writer )
{
    //MineGEDraw::writeExtraParam( writer );
}

void SimpleRockGateDraw::regPropertyDataNames( AcStringArray& names ) const
{
	names.append( _T( "长度" ) );
    names.append( _T( "宽度" ) );
    names.append( _T( "高度" ) );
	//names.append( _T( "迎头煤层投影点坐标" ) );
}

void SimpleRockGateDraw::readPropertyDataFromGE( const AcStringArray& values )
{
	m_length = abs(_tstof(values[0].kACharPtr()));
    m_width = abs(_tstof(values[1].kACharPtr()));
	m_height = abs(_tstof(values[2].kACharPtr()));
	//ArxUtilHelper::StringToPoint3d(values[2].kACharPtr(), m_pt);
}

Adesk::Boolean SimpleRockGateDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

	AcGeVector3d v(AcGeVector3d::kXAxis);
	AcGePoint3d p1 = m_insertPt + v*m_width*0.5;
	v.rotateBy(PI, AcGeVector3d::kZAxis);
	AcGePoint3d p2 = m_insertPt + v*m_width*0.5;

	DrawLine(mode, p1, p2);

	v.rotateBy(PI/2, AcGeVector3d::kZAxis);

	DrawLine(mode, p1, p1+v*m_length);
	DrawLine(mode, p2, p2+v*m_length);

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleRockGateDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

	gripPoints.append(m_insertPt);

    return Acad::eOk;
}

Acad::ErrorStatus SimpleRockGateDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
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

Acad::ErrorStatus SimpleRockGateDraw::subTransformBy( const AcGeMatrix3d& xform )
{
	m_insertPt.transformBy(xform);
	return Acad::eOk;
}

Acad::ErrorStatus SimpleRockGateDraw::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d& pickPoint,
	const AcGePoint3d& lastPoint,
	const AcGeMatrix3d& viewXform,
	AcGePoint3dArray& snapPoints,
	AcDbIntArray& geomIds ) const
{
	assertReadEnabled () ;
	//// 只捕捉1种类型的点：端点
	//if( osnapMode != AcDb::kOsMaskCen ) return Acad::eOk;

	//if( osnapMode == AcDb::kOsMaskCen )
	//{
	//	snapPoints.append(m_insertPt);
	//}

	return Acad::eOk;
}