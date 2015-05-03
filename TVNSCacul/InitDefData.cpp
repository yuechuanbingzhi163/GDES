#include "stdafx.h"

#include "../MineGE/HelperClass.h"
#include "NetWorkData.h"

#include "../ArxHelper/HelperClass.h"

extern CString ReadAlpha(CString chimType, CString diam);

// 初始化默认数据
void InitDefVentData(AcDbObjectId objId)
{
	Vent_InitDataLink vtl;
    vtl.setDataSource( objId );
    vtl.updateData( false );
	if (0 == vtl.T)
	{
		vtl.T = 1800;
	}

	if (0 == vtl.ro)
	{
		vtl.ro = 1.2;
	}

	//if (vtl.speed < 0.15 || vtl.speed > 0.25)
	//{
	//	vtl.speed = 0.25;
	//}

	if (0 == vtl.q0)
	{
		vtl.q0 = 4;
	}

	if (0 == vtl.Cp)
	{
		vtl.Cp = 1.01;
	}

    vtl.updateData( true );
}
