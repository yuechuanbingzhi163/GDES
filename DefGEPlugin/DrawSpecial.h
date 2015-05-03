#pragma once

#include "JointDrawType.h"

// Ãÿ ‚Õº–ŒªÊ÷∆
extern void DrawCross(AcGiWorldDraw *mode, const AcGePoint3d& pt, double radius);
extern void DrawSin(AcGiWorldDraw *mode, const AcGePoint3d& pt, double angle, double radius);
extern void DrawShaft(AcGiWorldDraw *mode, const AcGePoint3d& pt, double radius);
extern void DrawJoint(AcGiWorldDraw *mode, const AcGePoint3d& pt, double radius, JointDrawType jdt);
extern void DealSpPoints(CString& value);
extern void DrawBackGround(AcGiWorldDraw* mode ,const AcGePoint3dArray& pts,int colorIndx);
