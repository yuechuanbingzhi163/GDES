#pragma once

#include "PointGE.h"

// 瓦斯涌出量点抽象类
// GFP ==> Gas Flow Point
class GASGE_EXPORT_API GFP : public PointGE
{
public:
	ACRX_DECLARE_MEMBERS(GFP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

protected:
	GFP();
	GFP(const AcGePoint3d& insertPt);
};

// 回采工作面瓦斯涌出量点(包括绝对瓦斯涌出量和相对瓦斯涌出量)
class GASGE_EXPORT_API WGFP : public GFP
{
public:
	ACRX_DECLARE_MEMBERS(WGFP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	WGFP();
	WGFP(const AcGePoint3d& insertPt);
};

// 掘进工作面绝对瓦斯涌出量点
class GASGE_EXPORT_API TGFP : public GFP
{
public:
	ACRX_DECLARE_MEMBERS(TGFP) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	TGFP();
	TGFP(const AcGePoint3d& insertPt);
};

#ifdef GASGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(GFP)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(WGFP)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TGFP)
#endif
