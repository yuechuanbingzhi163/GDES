#pragma once

class MineGEDraw;

extern bool GetCurDraw(const CString& type, CString& draw);
extern bool SetCurDraw(const CString& type, const CString& draw);
extern void InitAllCurDraws();
