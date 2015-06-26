#include "StdAfx.h"
#include "SimpleDrillSiteDraw.h"
#include "DrawTool.h"
#include "DrawSpecial.h"

#include "../ArxHelper/HelperClass.h"

ACRX_CONS_DEFINE_MEMBERS ( SimpleDrillSiteDraw, MineGEDraw, 1 )

SimpleDrillSiteDraw::SimpleDrillSiteDraw () : MineGEDraw ()
{
}

SimpleDrillSiteDraw::~SimpleDrillSiteDraw ()
{
}

void SimpleDrillSiteDraw::setAllExtraParamsToDefault()
{
}

void SimpleDrillSiteDraw::configExtraParams()
{

}
void SimpleDrillSiteDraw::updateExtraParams()
{
}

void SimpleDrillSiteDraw::readKeyParam(DrawParamReader& reader)
{
	reader.readPoint(m_insertPt);
	reader.readPoint(m_linkPt);
}

void SimpleDrillSiteDraw::writeKeyParam(DrawParamWriter& writer)
{
	writer.writePoint(m_insertPt);
	writer.writePoint(m_linkPt);
}

void SimpleDrillSiteDraw::readExtraParam( DrawParamReader& reader )
{
    //MineGEDraw::readExtraParam( reader );
}

void SimpleDrillSiteDraw::writeExtraParam( DrawParamWriter& writer )
{
    //MineGEDraw::writeExtraParam( writer );
}

void SimpleDrillSiteDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "名称" ) );
    names.append( _T( "深度" ) );
	names.append( _T( "高度" ) );
	names.append( _T( "与迎头的距离" ) );
	names.append( _T( "位置" ) );
}

void SimpleDrillSiteDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    m_name = values[0].kACharPtr();
	m_width = abs(_tstof(values[1].kACharPtr()));
	m_height = abs(_tstof(values[2].kACharPtr()));
	m_dist = abs(_tstof(values[3].kACharPtr()));
}

Adesk::Boolean SimpleDrillSiteDraw::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

	//如果距离小于1m，则不绘制钻场的引线
	//if(m_dist > 1.0)
	//{
		DrawLine(mode, m_insertPt, m_linkPt);
	//}

	AcGeVector3d v = m_linkPt - m_insertPt;

	//向量v在x轴上的投影
	AcGeVector3d v1 = v.dotProduct( AcGeVector3d::kXAxis ) * AcGeVector3d::kXAxis;
	v1.normalize();
	if(v1.y < 0) v1.negate();

	//向量v在y轴上的投影
	AcGeVector3d v2 = v.dotProduct( AcGeVector3d::kYAxis ) * AcGeVector3d::kYAxis;
	v2.normalize();
	if(v2.y < 0) v2.negate();

	//求矩形的中心点坐标
	AcGePoint3d pt = m_linkPt;
	pt += v2*m_height*0.5;

	//计算角度
	//double angle = v1.angleTo(AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis);
	//绘制矩形
	DrawRect(mode, pt, 0, m_width, m_height, false);

    return Adesk::kTrue;
}

Acad::ErrorStatus SimpleDrillSiteDraw::subGetGripPoints( AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds ) const
{
    assertReadEnabled () ;

	gripPoints.append(m_insertPt);
	gripPoints.append(m_linkPt);

    return Acad::eOk;
}

Acad::ErrorStatus SimpleDrillSiteDraw::subMoveGripPointsAt ( const AcDbIntArray& indices, const AcGeVector3d& offset )
{
    assertWriteEnabled () ;

	for( int i = 0; i < indices.length(); i++ )
    {
        int idx = indices.at( i );

        if ( idx == 0 )
        {
			//m_insertPt += offset;
			//m_linkPt += offset;
        }

        if ( idx == 1 )
        {
           m_linkPt += offset;
        }
    }

    return Acad::eOk;
}

Acad::ErrorStatus SimpleDrillSiteDraw::subTransformBy( const AcGeMatrix3d& xform )
{
	m_insertPt.transformBy(xform);
	m_linkPt.transformBy(xform);
	return Acad::eOk;
}

Acad::ErrorStatus SimpleDrillSiteDraw::subGetOsnapPoints (
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