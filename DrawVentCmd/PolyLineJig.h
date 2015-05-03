#pragma once

class PolyLineJig : public AcEdJig 
{
public:
	PolyLineJig () ;
	~PolyLineJig () ;

	Adesk::Boolean doJig(AcGePoint3dArray& polygon);
} ;
