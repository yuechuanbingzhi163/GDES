#pragma once
#include "../MineGE/TagGE.h"
#include "dlimexp.h"

class DEFGE_EXPORT_API FanTagGE : public TagGE 
{
public:
	ACRX_DECLARE_MEMBERS(FanTagGE) ;
	FanTagGE();
	FanTagGE(AcGePoint3d& insertPt,CString name,CString way,double q,double h);
	virtual ~FanTagGE();

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

	double m_q;	//风机工作风量
	double m_h;	//风机工作风压
	CString m_name;	//风机型号
	CString m_way;	//风机的工作方式

};
#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(FanTagGE)
#endif