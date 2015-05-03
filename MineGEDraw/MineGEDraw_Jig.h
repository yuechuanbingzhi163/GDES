#pragma once

#include "dlimexp.h"

class MineGEDraw;

class MINEGE_DRAW_DLLIMPEXP MineGEDraw_Jig : public AcEdJig
{
public:
	Adesk::Boolean startJig(MineGEDraw* pMineGEDraw);

protected:
	MineGEDraw_Jig(const CString& geType, const CString& drawName);
	virtual Adesk::Boolean doJig(MineGEDraw* pMineGEDraw) = 0;	// Ö´ÐÐjig

private:
	CString m_geType;
	CString m_drawName;
} ;