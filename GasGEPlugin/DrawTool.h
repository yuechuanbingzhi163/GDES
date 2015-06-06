#pragma once

/* 全局函数(实现在DrawTool.cpp) */

// 3重堆叠格式，参见cad帮助："创建多行文字"
//	1) 符号: ^ (上下堆叠) 
//	2) 符号: / (左右堆叠，中间一个左斜线)
//	3) 符号: # (上下堆叠，中间一个横线 )	
extern CString MakeUpperText(const CString& inStr);
extern CString MakeLowerText(const CString& inStr);

extern AcGePoint2d Point3D_To_2D(const AcGePoint3d& pt);
extern AcGePoint3d Point2D_To_3D(const AcGePoint2d& pt);

// 基础图形绘制
extern void DrawLine(AcGiWorldDraw *mode, const AcGePoint3d& spt, const AcGePoint3d& ept);
extern void DrawLine(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, double length);
extern void DrawCircle(AcGiWorldDraw *mode, const AcGePoint3d& insertPt, double radius, bool fill);
extern void DrawArc(AcGiWorldDraw *mode, const AcGePoint3d& spt, const AcGePoint3d& pt, const AcGePoint3d& ept, bool fill);
extern void DrawArc(AcGiWorldDraw *mode, const AcGePoint3d& insertPt, double radius, double startAngle, double sweepAngle, bool fill);
extern void DrawPolyLine(AcGiWorldDraw *mode, const AcGePoint3d& spt, const AcGePoint3d& ept, double width);
extern void DrawPolyLine(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, double width, double length);
extern void DrawMText(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, const CString& str, double height, AcDbMText::AttachmentPoint ap=AcDbMText::kMiddleCenter);
extern void DrawRect(AcGiWorldDraw *mode, const AcGePoint3d& insertPt, double angle, double width, double height, bool fill);
extern void DrawSpline(AcGiWorldDraw *mode, const AcGePoint3dArray& pts);

// 特殊图形绘制
extern void DrawArrow(AcGiWorldDraw *mode, const AcGePoint3d& insertPt, double angle, double length, double width);