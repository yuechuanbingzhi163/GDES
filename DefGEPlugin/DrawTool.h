#pragma once
#include "../MineGE/config.h"
#define LESS_ZERO(x) (_tstof(x) < TOLERANCE)
/* 全局函数(实现在DrawTool.cpp) */

// 3重堆叠格式，参见cad帮助："创建多行文字"
//	1) 符号: ^ (上下堆叠) 
//	2) 符号: / (左右堆叠，中间一个左斜线)
//	3) 符号: # (上下堆叠，中间一个横线 )	
extern CString MakeUpperText(const CString& inStr);
extern CString MakeLowerText(const CString& inStr);

extern AcGePoint2d Point3D_To_2D(const AcGePoint3d& pt);
extern AcGePoint3d Point2D_To_3D(const AcGePoint2d& pt);

// 计算矩形框多边形
extern void BuildRect(const AcGePoint3d& pt, double angle, double width, double height, AcGePoint3dArray& pts);

// 基础图形绘制
extern void DrawLine(AcGiWorldDraw *mode, const AcGePoint3d& spt, const AcGePoint3d& ept);
extern void DrawLine(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, double length);
extern void DrawDotLine(AcGiWorldDraw *mode, const AcGePoint3d& spt, const AcGePoint3d& ept);
extern void DrawCircle(AcGiWorldDraw *mode, const AcGePoint3d& pt, double radius, bool fill);
extern void DrawArc(AcGiWorldDraw *mode, const AcGePoint3d& spt, const AcGePoint3d& pt, const AcGePoint3d& ept, bool fill);
extern void DrawArc(AcGiWorldDraw *mode, const AcGePoint3d& pt, double radius, double startAngle, double sweepAngle, bool fill);
extern void DrawPolyLine(AcGiWorldDraw *mode, const AcGePoint3d& spt, const AcGePoint3d& ept, double width);
extern void DrawPolyLine(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, double width, double length);
extern AcDbObjectId CreateTextStyle(const CString& style, const CString& winFont, Adesk::Boolean bold, Adesk::Boolean italic, int charset, int pitchAndFamily);
extern AcDbObjectId GetTextStyle(const CString& style);
extern void DrawMText(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, const CString& str, double height, AcDbMText::AttachmentPoint ap=AcDbMText::kMiddleCenter, const CString& style=_T("GDESS"));
extern void DrawRect(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, double width, double height, bool fill);
extern void DrawSpline(AcGiWorldDraw *mode, const AcGePoint3dArray& pts);
extern void DrawArrow(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, double length, double width);
extern void DrawPolygon(AcGiWorldDraw *mode, const AcGePoint3dArray& polygon, bool fill);

extern void CreatePolygonLoop(AcDbHatch* pHatch, const AcGePoint3dArray& pts);
extern void CreateCircleLoop(AcDbHatch* pHatch, const AcGePoint3d& pt, double radius);

extern void CreatePreDefinedHatch(AcDbHatch* pHatch, const CString& patName, double scale);
extern void CreateGradientObject(AcDbHatch* pHatch, const CString& gradName, const AcCmColor& c1, const AcCmColor& c2);

extern void DrawPolygonHatch(AcGiWorldDraw *mode, const AcGePoint3dArray& pts, const CString& patName, double scale);
extern void DrawCircleGradient(AcGiWorldDraw *mode, const AcGePoint3d& pt, double radius, const CString& gradName, const AcCmColor& c1, const AcCmColor& c2);

// 返回值: 
//		0  -- 错误
//		1  -- 顺时针
//	   -1  -- 逆时针
extern int ClockWise(const AcGePoint3dArray& polygon);

// 闭合多边形偏移
// 参数is_nner--true表示向内偏移，false表示向外偏移
extern bool OffSetPolygon(const AcGePoint3dArray& polygon, double offset, bool is_inner, AcGePoint3dArray& offset_polygon);

