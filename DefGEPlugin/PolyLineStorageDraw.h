#pragma once

#include "PolyLineStorageDrawBase.h"

// »ðÒ©¿â
class PolyLinePowderStorageDraw : public PolyLineStorageDrawBase
{
public:
	ACRX_DECLARE_MEMBERS(PolyLinePowderStorageDraw);

public:
	PolyLinePowderStorageDraw();
};

// »úµçíÏÊÒ
class PolyLineMachineRoomDraw : public PolyLineStorageDrawBase
{
public:
	ACRX_DECLARE_MEMBERS(PolyLineMachineRoomDraw);

public:
	PolyLineMachineRoomDraw();
};


// ³äµçÊÒ
class PolyLineChargeRoomDraw : public PolyLineStorageDrawBase
{
public:
	ACRX_DECLARE_MEMBERS(PolyLineChargeRoomDraw);

public:
	PolyLineChargeRoomDraw();
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PolyLinePowderStorageDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PolyLineMachineRoomDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PolyLineChargeRoomDraw)
#endif