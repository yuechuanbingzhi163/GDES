#pragma once
#include "../MineGE/TagGE.h"
#include "dlimexp.h"

class DEFGE_EXPORT_API EffectRanTagGE : public TagGE 
{
public:
	ACRX_DECLARE_MEMBERS(EffectRanTagGE) ;
	EffectRanTagGE();
	EffectRanTagGE(AcGePoint3d& insertPt,double angle,double minDistan,double maxDistan,double diatance);
	virtual ~EffectRanTagGE();

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);


private:
	AcGePoint3d m_insertPt; 
	double m_angle;
	double m_minDistan;
	double m_maxDistan;
	double m_diatance;
};
#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(EffectRanTagGE)
#endif