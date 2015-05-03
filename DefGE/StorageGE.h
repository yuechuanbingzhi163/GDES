#pragma once

#include "../MineGE/LinkedGE.h"
#include "dlimexp.h"

// 3类硐室图元的数据都不是相同的，不应看做是定制的可视化效果
// 不应同于巷道下的弧线巷道
// 弧线巷道只是可视化效果的更进一步定制, 但数据和Tunnel类是一样的
// 因此都直接从StorageGE抽象类派生, 作为"顶层标志图元"

// 硐室图元基类(抽象类)
class DEFGE_EXPORT_API StorageGE : public LinkedGE {

public:
	ACRX_DECLARE_MEMBERS(StorageGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	virtual ~StorageGE();

protected:
	StorageGE();
	StorageGE(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
} ;

// 火药库图元
class DEFGE_EXPORT_API PowderStorage : public StorageGE {

public:
	ACRX_DECLARE_MEMBERS(PowderStorage) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	PowderStorage();
	PowderStorage(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
	virtual ~PowderStorage();
} ;

// 机电硐室图元
class DEFGE_EXPORT_API MachineRoom : public StorageGE {

public:
	ACRX_DECLARE_MEMBERS(MachineRoom) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	MachineRoom();
	MachineRoom(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
	virtual ~MachineRoom();
} ;

// 充电室图元
class DEFGE_EXPORT_API ChargeRoom : public StorageGE {

public:
	ACRX_DECLARE_MEMBERS(ChargeRoom) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	ChargeRoom();
	ChargeRoom(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
	virtual ~ChargeRoom();
} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(StorageGE)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PowderStorage)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MachineRoom)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ChargeRoom)
#endif
