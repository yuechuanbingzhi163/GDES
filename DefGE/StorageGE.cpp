#include "StdAfx.h"
#include "StorageGE.h"

Adesk::UInt32 StorageGE::kCurrentVersionNumber = 1 ;
Adesk::UInt32 PowderStorage::kCurrentVersionNumber = 1 ;
Adesk::UInt32 MachineRoom::kCurrentVersionNumber = 1 ;
Adesk::UInt32 ChargeRoom::kCurrentVersionNumber = 1 ;

ACRX_NO_CONS_DEFINE_MEMBERS( StorageGE, LinkedGE )

ACRX_DXF_DEFINE_MEMBERS (
    PowderStorage, StorageGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ±¬Õ¨²ÄÁÏ¿â, DEFGEAPP
)

ACRX_DXF_DEFINE_MEMBERS (
    MachineRoom, StorageGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    »úµçíÏÊÒ, DEFGEAPP
)


ACRX_DXF_DEFINE_MEMBERS (
    ChargeRoom, StorageGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ³äµçÊÒ, DEFGEAPP
)

StorageGE::StorageGE () : LinkedGE ()
{
}

StorageGE::StorageGE( const AcGePoint3d& startPt, const AcGePoint3d& endPt ) : LinkedGE( startPt, endPt )
{
}

StorageGE::~StorageGE ()
{
}

PowderStorage::PowderStorage () : StorageGE ()
{
}

PowderStorage::PowderStorage( const AcGePoint3d& startPt, const AcGePoint3d& endPt ) : StorageGE( startPt, endPt )
{
}

PowderStorage::~PowderStorage ()
{
}

MachineRoom::MachineRoom () : StorageGE ()
{
}

MachineRoom::MachineRoom( const AcGePoint3d& startPt, const AcGePoint3d& endPt ) : StorageGE( startPt, endPt )
{
}

MachineRoom::~MachineRoom ()
{
}

ChargeRoom::ChargeRoom () : StorageGE ()
{
}

ChargeRoom::ChargeRoom( const AcGePoint3d& startPt, const AcGePoint3d& endPt ) : StorageGE( startPt, endPt )
{
}

ChargeRoom::~ChargeRoom ()
{
}