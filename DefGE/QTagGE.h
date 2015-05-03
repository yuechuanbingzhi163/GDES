#pragma once
#include "../MineGE/TagGE.h"
#include "dlimexp.h"

class DEFGE_EXPORT_API QTagGE : public TagGE 
{
public:
	ACRX_DECLARE_MEMBERS(QTagGE) ;
	QTagGE();
	QTagGE(AcGePoint3d& insertPt,double q);
	virtual ~QTagGE();

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
	AcGePoint3d m_insertPt; //风机标签的的插入点，也就是风机的插入点

	double m_q;	//风量
};
#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(QTagGE)
#endif