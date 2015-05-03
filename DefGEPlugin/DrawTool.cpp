#include "StdAfx.h"

#include "DrawTool.h"

#define PI 3.1415926535897932384626433832795

AcGePoint2d Point3D_To_2D( const AcGePoint3d& pt )
{
    return AcGePoint2d( pt.x, pt.y );
}

AcGePoint3d Point2D_To_3D( const AcGePoint2d& pt )
{
    return AcGePoint3d( pt.x, pt.y, 0 );
}

CString MakeUpperText( const CString& inStr )
{
    CString str;
    str.Format( _T( "{\\H0.618x;\\S%s^;}" ), inStr );
    return str;
}

CString MakeLowerText( const CString& inStr )
{
    CString str;
    str.Format( _T( "{\\H0.618x;\\S^%s;}" ), inStr );
    return str;
}

static AcGePoint3d CaclPt( const AcGePoint3d& pt, const AcGeVector3d& v1, double width, const AcGeVector3d& v2, double height )
{
    return ( pt + v1 * width / 2 + v2 * height / 2 );
}

void BuildRect( const AcGePoint3d& pt, double angle, double width, double height, AcGePoint3dArray& pts )
{
    AcGeVector3d v1( AcGeVector3d::kXAxis ), v2( AcGeVector3d::kXAxis );
    v1.rotateBy( angle, AcGeVector3d::kZAxis );
    v2.rotateBy( angle + PI / 2, AcGeVector3d::kZAxis );

    pts.append( CaclPt( pt, v1, width, v2, height ) );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( pt, v1, width, v2, height ) );

    v2.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( pt, v1, width, v2, height ) );

    v1.rotateBy( PI, AcGeVector3d::kZAxis );
    pts.append( CaclPt( pt, v1, width, v2, height ) );
}

/*
 * 在绘制闭合图形时，AcGiFillType默认为kAcGiFillAlways (始终填充)
 * 闭合图形包括：圆、多边形、网格等
 * 参见：AcGiSubEntityTraits::fillType()方法说明
 * 例如，绘制一个圆，当前颜色是黑底白色，那么采用自定义实体绘制的圆有2种情况:
 *	    1) arx程序加载的情况下-- 白边+黑底填充(正常效果，和cad的圆是一样的)
 *		2) arx程序卸载，cad采用代理实体模式显示图元 -- 白边+白底填充
 * 具体参见：绘制填充圆的一些说明.doc
 */
void DrawCircle( AcGiWorldDraw* mode, const AcGePoint3d& pt, double radius, bool fill )
{
    AcGiSubEntityTraits& traits = mode->subEntityTraits();
    AcGiFillType ft = traits.fillType();
    traits.setFillType( fill ? kAcGiFillAlways : kAcGiFillNever );

    mode->geometry().circle( pt, radius, AcGeVector3d::kZAxis );

    // 恢复属性
    traits.setFillType( ft );
}

extern void DrawArc( AcGiWorldDraw* mode, const AcGePoint3d& pt, double radius, double startAngle, double sweepAngle, bool fill )
{
    AcGiSubEntityTraits& traits = mode->subEntityTraits();
    AcGiFillType ft = traits.fillType();
    traits.setFillType( fill ? kAcGiFillAlways : kAcGiFillNever );

    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( startAngle, AcGeVector3d::kZAxis );
    AcGiArcType arcType = ( fill ? kAcGiArcChord : kAcGiArcSimple );
    mode->geometry().circularArc( pt, radius, AcGeVector3d::kZAxis, v, sweepAngle, arcType );

    // 恢复属性
    traits.setFillType( ft );
}

void DrawArc( AcGiWorldDraw* mode, const AcGePoint3d& spt, const AcGePoint3d& pt, const AcGePoint3d& ept, bool fill )
{
    AcGiSubEntityTraits& traits = mode->subEntityTraits();
    AcGiFillType ft = traits.fillType();
    traits.setFillType( fill ? kAcGiFillAlways : kAcGiFillNever );

    AcGeCircArc3d arc( spt, pt, ept );
    AcGePoint3d cnt = arc.center();
    double radius = arc.radius();
    AcGeVector3d sv = spt - cnt;
    AcGeVector3d ev = ept - cnt;
    double sa = sv.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );
    double ea = ev.angleTo( AcGeVector3d::kXAxis, -AcGeVector3d::kZAxis );

    if( arc.normal().z > 0 )
    {
        AcDbArc arcEnt( cnt, radius, sa, ea );
        arcEnt.worldDraw( mode );
    }
    else
    {
        AcDbArc arcEnt( cnt, radius, ea, sa );
        arcEnt.worldDraw( mode );
    }

    // 恢复属性
    traits.setFillType( ft );
}

void DrawLine( AcGiWorldDraw* mode, const AcGePoint3d& spt, const AcGePoint3d& ept )
{
    AcDbLine line( spt, ept );
    line.worldDraw( mode );
}

void DrawLine( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, double length )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );
    DrawLine( mode, pt, pt + v * length );
}

void DrawPolyLine( AcGiWorldDraw* mode, const AcGePoint3d& spt, const AcGePoint3d& ept, double width )
{
    AcDbPolyline pl( 2 );
    pl.addVertexAt( 0, Point3D_To_2D( spt ) );
    pl.addVertexAt( 1, Point3D_To_2D( ept ) );
    pl.setConstantWidth( width );

    pl.worldDraw( mode );
}

void DrawPolyLine( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, double length, double width )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );

    AcDbPolyline pl( 2 );
    pl.addVertexAt( 0, Point3D_To_2D( pt ) );
    pl.addVertexAt( 1, Point3D_To_2D( pt + v * length ) );
    pl.setConstantWidth( width );

    pl.worldDraw( mode );
}

//AcDbObjectId createTextStyle(CString fontName,CString bigFontName,CString textStyleName)
//{
//	AcGiTextStyle *TextStyle=new AcGiTextStyle
//		(fontName,
//		bigFontName,
//		0,
//		0.67,
//		0,
//		0,
//		Adesk::kFalse,
//		Adesk::kFalse,
//		Adesk::kFalse,
//		Adesk::kFalse,
//		Adesk::kFalse,
//		textStyleName); //字体名
//	AcDbObjectId textStyleId;
//	toAcDbTextStyle(*TextStyle,textStyleId);
//	return textStyleId;
//}
//
//AcDbObjectId createMutiText(AcGePoint3d BasePoint,AcDb::TextHorzMode hMode,CString Text,double texthight,double widthfactor,double angle,int color,CString smallFontName,CString bigFontName)
//{
//	AcDbMText *pMText=new AcDbMText();
//	AcDbObjectId TextStyleId;
//	TextStyleId=createTextStyle(smallFontName,bigFontName,"xianlu");
//	pMText->setTextStyle(TextStyleId);
//	pMText->setContents(Text.GetBuffer(Text.GetLength()));
//	pMText->setTextHeight(texthight);
//	pMText->setRotation(angle);
//	pMText->setLineSpacingFactor(0.8);
//	pMText->setColorIndex(color);
//	return TextStyleId;
//}

/*
 参考资料
 https://msdn.microsoft.com/zh-cn/library/ms858335.aspx
 https://msdn.microsoft.com/en-us/library/cc194829.aspx
 http://bbs.mjtd.com/thread-1560-1-1.html
 http://blog.csdn.net/sw283632534/article/details/5401999
 http://blog.csdn.net/jiangdong2007/article/details/39637369
 */

AcDbObjectId GetTextStyle(const CString& style)
{
	AcDbTextStyleTable *pTextStyleTbl;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pTextStyleTbl, AcDb::kForWrite);
	AcDbObjectId fontId;
	pTextStyleTbl->getAt(style, fontId);
	pTextStyleTbl->close();

	return fontId;
}

AcDbObjectId CreateTextStyle(const CString& style, const CString& winFont, Adesk::Boolean bold, Adesk::Boolean italic, int charset, int pitchAndFamily)
{
	/*
	//pTextStyleTblRcd->setFileName(_T("simfang.ttf"));
	//pTextStyleTblRcd->setBigFontFileName("hzdx");
	pTextStyleTblRcd->setXScale(0.8);
	//pTextStyleTblRcd->setFont("_T(楷体_GB2312"),Adesk::kFalse,Adesk::kFalse,GB2312_CHARSET,49);
	//pTextStyleTblRcd->setFont(_T("宋体"),Adesk::kFalse,Adesk::kFalse,GB2312_CHARSET,2);
	pTextStyleTblRcd->setFont(winFont,Adesk::kFalse,Adesk::kFalse,GB2312_CHARSET,16);
	*/

	AcDbTextStyleTable *pTextStyleTbl;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pTextStyleTbl, AcDb::kForWrite);
	AcDbObjectId fontId;
	if(pTextStyleTbl->getAt(style, fontId) == Acad::eKeyNotFound)
	{
		AcDbTextStyleTableRecord *pTextStyleTblRcd=new AcDbTextStyleTableRecord;
		pTextStyleTblRcd->setName(style);
		pTextStyleTblRcd->setFont(winFont,bold,italic,charset,pitchAndFamily);
		pTextStyleTbl->add(fontId,pTextStyleTblRcd);
		pTextStyleTblRcd->close();
	}
	pTextStyleTbl->close();

	return fontId;
	//acdbHostApplicationServices()->workingDatabase()->setTextstyle(fontId);
}

void DrawMText( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, const CString& str, double height, AcDbMText::AttachmentPoint ap, const CString& style )
{
    //acutPrintf(_T("\n绘制前--文字颜色:%d"), mode->subEntityTraits().color());
    AcDbMText mt;

 //   //AcDbObjectId style; // 文字样式
	AcDbObjectId fontId = GetTextStyle(style);
	if(!fontId.isNull())
	{
		acutPrintf(_T("\n设置样式为罗马字体"));
		mt.setTextStyle(fontId);
	}
    mt.setLocation( pt );
    mt.setTextHeight( height );

	mt.setAttachment( ap );
    mt.setRotation( angle );

    mt.setContents( str );

    // 经过测试发现，AcDbMText调用worldDraw的同时会修改mode的一些属性
    mt.setColorIndex( mode->subEntityTraits().color() );
	
    mt.worldDraw( mode );

    //acutPrintf(_T("\n绘制后--文字颜色:%d"), mode->subEntityTraits().color());
}

void DrawRect( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, double width, double height, bool fill )
{
    AcGiSubEntityTraits& traits = mode->subEntityTraits();

    AcGePoint3dArray pts;
    BuildRect( pt, angle, width, height, pts );

    // 是否填充
    AcGiFillType ft = traits.fillType();
    traits.setFillType( fill ? kAcGiFillAlways : kAcGiFillNever );

    mode->geometry().polygon( pts.length(), pts.asArrayPtr() );

    traits.setFillType( ft );
}

void DrawSpline( AcGiWorldDraw* mode, const AcGePoint3dArray& pts )
{
    // 绘制样条曲线
    AcDbSpline sp( pts );
    sp.worldDraw( mode );
}

// 绘制箭头
void DrawArrow( AcGiWorldDraw* mode, const AcGePoint3d& pt, double angle, double width, double length )
{
    AcGeVector3d v( AcGeVector3d::kXAxis );
    v.rotateBy( angle, AcGeVector3d::kZAxis );

    AcDbPolyline pl( 2 );
    pl.addVertexAt( 0, Point3D_To_2D( pt ) );
    pl.addVertexAt( 1, Point3D_To_2D( pt + v * length ) );
    pl.setWidthsAt( 0, width, 0 );
    pl.setWidthsAt( 1, 0, 0 );

    pl.worldDraw( mode );
}

void CreatePolygonLoop( AcDbHatch* pHatch, const AcGePoint3dArray& pts )
{
    AcGeIntArray edgeTypes;
    AcGeVoidPointerArray edgePtrs;

    int n = pts.length();
    for( int i = 0; i < n; i++ )
    {
        AcGePoint2d p1 = Point3D_To_2D( pts[i] );
        AcGePoint2d p2 = Point3D_To_2D( pts[( i + 1 ) % n] );
        AcGeLineSeg2d* pLine = new AcGeLineSeg2d( p1, p2 );

        edgePtrs.append( pLine );
        edgeTypes.append( AcDbHatch::kLine );
    }

    pHatch->appendLoop( AcDbHatch::kDefault, edgePtrs, edgeTypes );
}

void CreateCircleLoop( AcDbHatch* pHatch, const AcGePoint3d& pt, double radius )
{
    AcGeCircArc2d* cirArc = new AcGeCircArc2d();
    cirArc->setCenter( Point3D_To_2D( pt ) );
    cirArc->setRadius( radius );
    cirArc->setAngles( 0.0, PI * 2 );

    AcGeIntArray edgeTypes;
    AcGeVoidPointerArray edgePtrs;

    edgeTypes.append( AcDbHatch::kCirArc );
    edgePtrs.append( ( void* )cirArc );

    pHatch->appendLoop( AcDbHatch::kDefault, edgePtrs, edgeTypes );
}

void CreatePreDefinedHatch( AcDbHatch* pHatch, const CString& patName, double scale )
{
    // Set hatch plane
    pHatch->setNormal( AcGeVector3d::kZAxis );
    pHatch->setElevation( 0.0 );

    // Set non associative hatch
    pHatch->setAssociative( Adesk::kFalse );

    // Set hatch pattern to SolidFill type
    // 设置hatch pattern之后，再调用setPattern才能生效
    pHatch->setPatternScale( scale );
    pHatch->setPattern( AcDbHatch::kPreDefined, patName );

    // Set hatch style to kNormal
    pHatch->setHatchStyle( AcDbHatch::kNormal );
}

void CreateGradientObject( AcDbHatch* pHatch, const CString& gradName, const AcCmColor& c1, const AcCmColor& c2 )
{
    // Set hatch plane
    pHatch->setNormal( AcGeVector3d::kZAxis );
    pHatch->setElevation( 0.0 );

    // Set non associative hatch
    pHatch->setAssociative( Adesk::kFalse );

    // Set hatch pattern to SolidFill type
    //pHatch->setPattern(AcDbHatch::kPreDefined, _T("SOLID"));

    // Set hatch style to kNormal
    //pHatch->setHatchStyle(AcDbHatch::kNormal);

    // 设置为渐变色填充
    pHatch->setHatchObjectType( AcDbHatch::kGradientObject );

    // set gradient
    pHatch->setGradient( AcDbHatch::kPreDefinedGradient, gradName );
    pHatch->setGradientOneColorMode( false ); // 实质上都是双色

    AcCmColor colors[2] = {c1, c2};
    float values[2] = {0.0f, 1.0f};

    pHatch->setGradientColors( 2, colors, values );
}

void DrawPolygonHatch( AcGiWorldDraw* mode, const AcGePoint3dArray& pts, const CString& patName, double scale )
{
    if( pts.isEmpty() ) return;

    AcDbHatch hatch;
    // set color
    //AcCmColor color;
    //color.setRGB(r, g, b);
    //pHatch->setColor(color);

    CreatePreDefinedHatch( &hatch, patName, scale );
    CreatePolygonLoop( &hatch, pts );

    // Elaborate solid fill
    hatch.evaluateHatch();

    hatch.worldDraw( mode );
}

void DrawCircleGradient( AcGiWorldDraw* mode, const AcGePoint3d& pt, double radius, const CString& gradName, const AcCmColor& c1, const AcCmColor& c2 )
{
    AcDbHatch hatch;

    CreateGradientObject( &hatch, gradName, c1, c2 );
    CreateCircleLoop( &hatch, pt, radius );

    //pHatch->evaluateHatch();
    hatch.worldDraw( mode );
}

void DrawPolygon( AcGiWorldDraw* mode, const AcGePoint3dArray& polygon, bool fill )
{
    AcGiSubEntityTraits& traits = mode->subEntityTraits();

    // 是否填充
    AcGiFillType ft = traits.fillType();
    traits.setFillType( fill ? kAcGiFillAlways : kAcGiFillNever );

    mode->geometry().polygon( polygon.length(), polygon.asArrayPtr() );

    traits.setFillType( ft );
}

// 返回值:
//		0  -- 错误
//		1  -- 顺时针
//	   -1  -- 逆时针
int ClockWise( const AcGePoint3dArray& polygon )
{
    int n = polygon.length();
    if ( n < 3 ) return 0;

    int count = 0;
    for( int i = 0; i < n; i++ )
    {
        int j = ( i + 1 ) % n;
        int k = ( i + 2 ) % n;
        double z  = ( polygon[j].x - polygon[i].x ) * ( polygon[k].y - polygon[j].y );
        z -= ( polygon[j].y - polygon[i].y ) * ( polygon[k].x - polygon[j].x );
        if ( z < 0 )
        {
            count--;
        }
        else if ( z > 0 )
        {
            count++;
        }
    }
    if ( count > 0 )
        return -1;
    else if ( count < 0 )
        return 1;
    else
        return 0;
}

// 闭合多边形向内偏移
bool OffSetPolygon( const AcGePoint3dArray& polygon, double offset, bool is_inner, AcGePoint3dArray& offset_polygon )
{
    // 判断多边形方向
    int dir = ClockWise( polygon );
    if( dir == 0 ) return false;

    // 向内或向外???
    int c = ( is_inner ? -1 : 1 );

    // 偏移方向角度
    //		1) 与多边形的方向有关(dir)
    //		2) 与要偏移的方向有关(is_inner)
    double angle = c * dir * PI * 0.5;

    bool ret = true;

    int n = polygon.length();
    for( int i = 0; i < n; i++ )
    {
        int p1 = ( n + i - 1 ) % n, p2 = i % n, p3 = ( i + 1 ) % n;

        // 对点进行偏移
        // 计算偏移向量
        AcGeVector3d v1 = polygon[p2] - polygon[p1];
        v1.normalize();
        v1.rotateBy( angle, AcGeVector3d::kZAxis );

        AcGeVector3d v2 = polygon[p3] - polygon[p2];
        v2.normalize();
        v2.rotateBy( angle, AcGeVector3d::kZAxis );

        // 求两个向量的夹角
        double angle = v1.angleTo( v2 );
        double L = abs( offset / cos( angle * 0.5 ) );

        AcGeVector3d v = v1 + v2;
        v.normalize();

        offset_polygon.append( polygon[p2] + v * L );
    }
    return ret;
}

static Acad::ErrorStatus GetLinetypeId(const CString& linetype, AcDbObjectId &linetypeId)
{
	AcDbLinetypeTable *pLtpTbl;
	acdbHostApplicationServices()->workingDatabase()->getLinetypeTable(pLtpTbl, AcDb::kForRead);
	if (!pLtpTbl->has(linetype))
	{
		pLtpTbl->close();
		return Acad::eBadLinetypeName;
	}
	pLtpTbl->getAt(linetype, linetypeId);
	pLtpTbl->close();
	return Acad::eOk;
}

static void AddLineType(CString lineTypeName)
{
	// 加载线型（两种方法）
	Acad::ErrorStatus es;
	//es = acdbHostApplicationServices()->workingDatabase()->loadLineTypeFile(_T("CENTER"), _T("acadiso.lin"));
	es = acdbLoadLineTypeFile(lineTypeName, _T("acadiso.lin"),acdbHostApplicationServices()->workingDatabase());
	// 创建新的AcDbMlineStyle对象
	AcDbMlineStyle *pMlStyle = new AcDbMlineStyle;
	pMlStyle->initMlineStyle();
	pMlStyle->setName(_T("NewStyle"));
	int index; // 多线样式中的元素索引
	//AcCmColor color; // 颜色
	AcDbObjectId linetypeId; // 线型的ID
	// 添加第一个元素（红色的中心线）
	//color.setColorIndex(1); // 红色
	GetLinetypeId(lineTypeName, linetypeId);
	//pMlStyle->addElement(index, 0, color, linetypeId);
	//// 添加第二个元素（蓝色的虚线）
	//color.setColorIndex(5); // 蓝色
	//GetLinetypeId("HIDDEN", linetypeId);
	//pMlStyle->addElement(index, 0.5, color, linetypeId);
	//// 添加第三个元素（蓝色的虚线）
	//pMlStyle->addElement(index, -0.5, color, linetypeId);
	//// 将多线样式添加到多线样式字典中
	AcDbDictionary *pDict;
	acdbHostApplicationServices()->workingDatabase()->getMLStyleDictionary(pDict, AcDb::kForWrite);
	AcDbObjectId mlStyleId;
	es = pDict->setAt(_T("NewStyle"), pMlStyle, mlStyleId);
	pDict->close();
	pMlStyle->close();
}

void DrawDotLine( AcGiWorldDraw *mode, const AcGePoint3d& spt, const AcGePoint3d& ept )
{
	AddLineType(_T("CENTER"));
	AcDbLine line( spt, ept );
	line.setLinetype(_T("CENTER"));
	line.worldDraw( mode );

}