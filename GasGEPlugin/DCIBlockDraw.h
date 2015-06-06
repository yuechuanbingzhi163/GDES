#pragma once

#include "../MineGEDraw/BlockDraw.h"

class CSDCIBlockDraw : public BlockDraw 
{
public:
	ACRX_DECLARE_MEMBERS(CSDCIBlockDraw) ;

public:
	CSDCIBlockDraw();
	virtual void regPropertyDataNames(AcStringArray& names) const;
	virtual void readPropertyDataFromGE(const AcStringArray& values);
};

class WDCI1BlockDraw : public BlockDraw 
{
public:
	ACRX_DECLARE_MEMBERS(WDCI1BlockDraw) ;

public:
	WDCI1BlockDraw();
	virtual void regPropertyDataNames(AcStringArray& names) const;
};

class WDCI2BlockDraw : public BlockDraw 
{
public:
	ACRX_DECLARE_MEMBERS(WDCI2BlockDraw) ;

public:
	WDCI2BlockDraw();
	virtual void regPropertyDataNames(AcStringArray& names) const;
};

#ifdef GASGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CSDCIBlockDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(WDCI1BlockDraw)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(WDCI2BlockDraw)
#endif
