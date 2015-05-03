#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// 风机抽象类
class DEFGE_EXPORT_API Fan : public DirectionTagGE
{
public:
	ACRX_DECLARE_MEMBERS(Fan) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

protected:
	Fan() ;
	Fan(const AcGePoint3d& insertPt, double angle);
};

// 局部扇风机
class DEFGE_EXPORT_API LocalFan : public Fan {

public:
	ACRX_DECLARE_MEMBERS(LocalFan) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	LocalFan() ;
	LocalFan(const AcGePoint3d& insertPt, double angle);
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Fan)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(LocalFan)
#endif
