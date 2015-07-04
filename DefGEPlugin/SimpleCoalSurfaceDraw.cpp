#include "StdAfx.h"
#include "SimpleCoalSurfaceDraw.h"
#include "DrawTool.h"
#include "DrawSpecial.h"

#include "../ArxHelper/HelperClass.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleCoalSurfaceDraw, MineGEDraw, 1 )

SimpleCoalSurfaceDraw::SimpleCoalSurfaceDraw () : MineGEDraw ()
{
}

SimpleCoalSurfaceDraw::~SimpleCoalSurfaceDraw ()
{
}

void SimpleCoalSurfaceDraw::setAllExtraParamsToDefault()
{
	//m_height = 0;
	//m_width = 0;
}

void SimpleCoalSurfaceDraw::configExtraParams()
{

}
void SimpleCoalSurfaceDraw::updateExtraParams()
{
}

void SimpleCoalSurfaceDraw::readKeyParam(DrawParamReader& reader)
{
	reader.readPoint(m_insertPt);
	//reader.readDouble(m_width);
	//reader.readDouble(m_height);
}

void SimpleCoalSurfaceDraw::writeKeyParam(DrawParamWriter& writer)
{
	writer.writePoint(m_insertPt);
	//writer.writeDouble(m_width);
	//writer.writeDouble(m_height);
}

void SimpleCoalSurfaceDraw::readExtraParam( DrawParamReader& reader )
{
    //MineGEDraw::readExtraParam( reader );
}

void SimpleCoalSurfaceDraw::writeExtraParam( DrawParamWriter& writer )
{
    //MineGEDraw::writeExtraParam( writer );
}

void SimpleCoalSurfaceDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "$几何宽度" ) );
    names.append( _T( "$几何高度" ) );
	//names.append( _T( "中心点坐标" ) );
}

void SimpleCoalSurfaceDraw::readPropertyDataFromGE( const AcStringArray& values )
{
   // m_width = abs(_tstof(values[0].kACharPtr()));
	//m_height = abs(_tstof(values[1].kACharPtr()));
	if(values.isEmpty()) return;
	m_width =_tstof(values[0].kACharPtr());
	m_height = _tstof(values[1].kACharPtr());

	//ArxUtilHelper::StringToPoint3d(values[2].kACharPtr(), m_pt);
}

Adesk::Boolean SimpleCoalSurfaceDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

	//绘制矩形
	if(m_width > 0 && m_height > 0) 
	{
		DrawRect(mode, m_insertPt, 0, m_width, m_height, false);
	}

	//绘制宽和高标注
	CString value;
	value.Format(_T("%.2f(m)"),m_width);
	AcGeVector3d v(AcGeVector3d::kYAxis);
	AcGePoint3d insertPt = m_insertPt + v*m_height*0.5;
	//DrawMText(mode,insertPt,PI*0.5,value,)

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleCoalSurfaceDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

	//AcGeVector3d v(m_width*0.5, -m_height*0.5, 0);
	gripPoints.append(m_insertPt);

    return Acad::eOk;
}

Acad::ErrorStatus SimpleCoalSurfaceDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
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

Acad::ErrorStatus SimpleCoalSurfaceDraw::subTransformBy( const AcGeMatrix3d& xform )
{
	m_insertPt.transformBy(xform);
	return Acad::eOk;
}

Acad::ErrorStatus SimpleCoalSurfaceDraw::subGetOsnapPoints (
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
	if( osnapMode != AcDb::kOsMaskCen && osnapMode != AcDb::kOsMaskEnd) return Acad::eOk;

	if( osnapMode == AcDb::kOsMaskCen )
	{
		snapPoints.append(m_insertPt);
	}
	else if(osnapMode == AcDb::kOsMaskEnd)
	{
		AcGeVector3d u(AcGeVector3d::kXAxis);
		AcGeVector3d v(AcGeVector3d::kYAxis);
		snapPoints.append(m_insertPt+u*m_width+v*m_height);

		u.rotateBy(PI, AcGeVector3d::kZAxis);
		snapPoints.append(m_insertPt+u*m_width+v*m_height);

		v.rotateBy(PI, AcGeVector3d::kZAxis);
		snapPoints.append(m_insertPt+u*m_width+v*m_height);

		u.rotateBy(PI, AcGeVector3d::kZAxis);
		snapPoints.append(m_insertPt+u*m_width+v*m_height);
	}

	return Acad::eOk;
}
