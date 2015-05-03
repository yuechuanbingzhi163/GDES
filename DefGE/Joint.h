#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

class DEFGE_EXPORT_API Joint : public DirectionTagGE
{
public:
	ACRX_DECLARE_MEMBERS(Joint) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	Joint();
	Joint(const AcGePoint3d& insertPt, double angle);
};

class DEFGE_EXPORT_API Test : public AcDbEntity
{
public:
	ACRX_DECLARE_MEMBERS(Test) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	Test();
};


#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Joint)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Test)
#endif
