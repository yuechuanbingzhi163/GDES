#pragma once

#include "PolyLineTunnelDraw.h"

class PolyLineTTunnelDraw : public PolyLineTunnelDraw
{
public:
	ACRX_DECLARE_MEMBERS(PolyLineTTunnelDraw);

public:
	PolyLineTTunnelDraw(void);

protected:
	void drawText(AcGiWorldDraw *mode);
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PolyLineTTunnelDraw)
#endif