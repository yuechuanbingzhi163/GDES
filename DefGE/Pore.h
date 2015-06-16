#pragma once

#include "RcuGE.h"

//���ͼԪ(���ף��տ�)
class DEFGE_EXPORT_API Pore : public RcuGE
{
public:
	ACRX_DECLARE_MEMBERS(Pore) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	//���캯��
	Pore();

	// ��ȡ���������
	AcGePoint3d getInsertPt() const;

	// ���ò��������
	void setInsertPt(const AcGePoint3d& pt);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // ���������
};

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Pore)
#endif