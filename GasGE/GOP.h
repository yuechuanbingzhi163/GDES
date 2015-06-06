#pragma once

#include "PointGE.h"

// 瓦斯突出点抽象类
// GOP ==> Gas Outburst Point
class GASGE_EXPORT_API GOP : public PointGE
{
public:
	ACRX_DECLARE_MEMBERS(GOP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

protected:
	GOP();
	GOP(const AcGePoint3d& insertPt);
};

// 小型突出点
class GASGE_EXPORT_API SmallGOP : public GOP {

public:
	ACRX_DECLARE_MEMBERS(SmallGOP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	SmallGOP() ;
	SmallGOP(const AcGePoint3d& insertPt);
};

// 中型突出点
class GASGE_EXPORT_API MidGOP : public GOP {

public:
	ACRX_DECLARE_MEMBERS(MidGOP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	MidGOP() ;
	MidGOP(const AcGePoint3d& insertPt);
} ;

// 大型突出点
class GASGE_EXPORT_API LargeGOP : public GOP {

public:
	ACRX_DECLARE_MEMBERS(LargeGOP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	LargeGOP() ;
	LargeGOP(const AcGePoint3d& insertPt);
};

// 特大型突出点
class GASGE_EXPORT_API LargerGOP : public GOP {

public:
	ACRX_DECLARE_MEMBERS(LargerGOP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	LargerGOP() ;
	LargerGOP(const AcGePoint3d& insertPt);
};

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GOP)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SmallGOP)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MidGOP)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(LargeGOP)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(LargerGOP)
#endif
