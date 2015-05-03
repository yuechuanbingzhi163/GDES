#pragma once

#include "DoubleStorageDrawBase.h"

// »ðÒ©¿â
class DoublePowderStorageDraw : public DoubleStorageDrawBase
{
public:
	ACRX_DECLARE_MEMBERS(DoublePowderStorageDraw);

public:
	DoublePowderStorageDraw();
};

// »úµçíÏÊÒ
class DoubleMachineRoomDraw : public DoubleStorageDrawBase
{
public:
	ACRX_DECLARE_MEMBERS(DoubleMachineRoomDraw);

public:
	DoubleMachineRoomDraw();
};


// ³äµçÊÒ
class DoubleChargeRoomDraw : public DoubleStorageDrawBase
{
public:
	ACRX_DECLARE_MEMBERS(DoubleChargeRoomDraw);

public:
	DoubleChargeRoomDraw();
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoublePowderStorageDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleMachineRoomDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DoubleChargeRoomDraw)
#endif