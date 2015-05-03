#pragma once

#include "../MineGE/TagGE.h"
#include "dlimexp.h"

// ∑Á«Ω£∫”¿æ√µ≤∑Á«Ω£¨¡Ÿ ±µ≤∑Á«Ω

// ∑Á«Ω≥ÈœÛ¿‡
class DEFGE_EXPORT_API Wall : public DirectionTagGE
{
public:
	ACRX_DECLARE_MEMBERS(Wall) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

protected:
	Wall();
	Wall(const AcGePoint3d& insertPt, double angle);
};

// ”¿æ√µ≤∑Á«Ω
class DEFGE_EXPORT_API PermanentWall : public Wall {

public:
	ACRX_DECLARE_MEMBERS(PermanentWall) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	PermanentWall() ;
	PermanentWall(const AcGePoint3d& insertPt, double angle);
};

// ¡Ÿ ±µ≤∑Á«Ω
class DEFGE_EXPORT_API TemporaryWall : public Wall {

public:
	ACRX_DECLARE_MEMBERS(TemporaryWall) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	TemporaryWall() ;
	TemporaryWall(const AcGePoint3d& insertPt, double angle);
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Wall)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PermanentWall)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TemporaryWall)
#endif
