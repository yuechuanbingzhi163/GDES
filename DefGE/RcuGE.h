#pragma once

#include "../MineGE/MineGE.h"
#include "../MineGE/TagGE.h"
#include "dlimexp.h"

class DEFGE_EXPORT_API RcuGE : public MineGE
{
public:
	ACRX_DECLARE_MEMBERS(RcuGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	RcuGE();
};

class DEFGE_EXPORT_API RcuTagGE : public TagGE
{
public:
	ACRX_DECLARE_MEMBERS(RcuTagGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	RcuTagGE();
};

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(RcuGE)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(RcuTagGE)
#endif
