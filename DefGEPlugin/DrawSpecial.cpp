#include "StdAfx.h"

#include "DrawTool.h"
#include "DrawSpecial.h"

#define PI 3.1415926535897932384626433832795

void DrawCross( AcGiWorldDraw* mode, const AcGePoint3d& pt, double radius )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v *= radius;

    for( int i = 0; i < 4; i++ )
    {
        AcGePoint3dArray pts;
        pts.append( pt );
        pts.append( pt + v );
        mode->geometry().worldLine( pts.asArrayPtr() );

        v.rotateBy( PI / 2, AcGeVector3d::kZAxis );
    }
}

// 绘制正弦(2个半圆)
void DrawSin( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, double radius )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );

    // 绘制第1个半圆
    mode->geometry().circularArc( pt + v * radius, radius, AcGeVector3d::kZAxis, v, PI );

    // 绘制第2个半圆
    // 注：不使用v.negate()方法, 该方法会修改v
    mode->geometry().circularArc( pt + v * radius * 3, radius, AcGeVector3d::kZAxis, -v, PI );
}

void DrawShaft( AcGiWorldDraw* mode, const AcGePoint3d& pt, double radius )
{
    // 绘制外环
    DrawCircle( mode, pt, radius, false );

    // 绘制内环
    DrawArc( mode, pt, radius * 0.8, PI / 6, PI, true );
    DrawArc( mode, pt, radius * 0.8, PI * 7 / 6, PI, false );
}

void DrawJoint( AcGiWorldDraw* mode, const AcGePoint3d& pt, double radius, JointDrawType jdt )
{
    if( jdt == JDT_CROSS )
    {
        DrawCircle( mode, pt, radius, false );
        DrawCross( mode, pt, radius );
    }
    else if( jdt == JDT_FILL )
    {
        DrawCircle( mode, pt, radius, true );
    }
}

void DealSpPoints( CString& value )
{
	//小数点前面的0补全,并且除掉左右多余的0
	if(value.Find(_T('.')) == -1) return;
	CString strValue;
	strValue.Format(_T("%.2f"),_tstof(value));
	value = strValue;
	value.Replace(_T("0"),_T(" "));	//替换0为空格
	value.Trim();	//裁剪
	value.Replace(_T(" "),_T("0"));
	if(value[0] == _T('.')) value.Insert(0,_T("0"));
	int lenth = value.GetLength();
	if(0 == lenth)
	{
		return;
	}
	if(value[lenth-1] == _T('.'))
	{
		value.Replace(_T("."),_T(" "));
		value.Trim();	//裁剪
	}

}

void DrawBackGround( AcGiWorldDraw* mode ,const AcGePoint3dArray& pts ,int colorIndx)
{
	// 用户没有定义边界
	if( pts.isEmpty() ) return;

	Adesk::UInt8 r, g, b;

	AcGiSubEntityTraits& traits = mode->subEntityTraits();
	// 保存原有的属性
	Adesk::UInt16 cl = traits.color();;
	AcGiFillType ft = traits.fillType();

	//AcCmEntityColor bgColor( r, g, b );
	//traits.setTrueColor( bgColor );
	if(colorIndx < 0 || colorIndx > 256) colorIndx = 7;
	traits.setColor(colorIndx);
	traits.setFillType( kAcGiFillAlways ); // 填充
	//acutPrintf(_T("\n颜色索引：%d"), bgColor.colorIndex());
	mode->geometry().polygon( pts.length(), pts.asArrayPtr() );

	// 恢复属性
	traits.setFillType( ft );
	traits.setColor( cl );
}