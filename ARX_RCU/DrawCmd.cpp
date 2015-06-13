#include "StdAfx.h"
#include "DrawCmd.h"
#include "RcuNewDlg.h"
#include "RcuDesignDlg.h"
#include "RcuEditDlg.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

#include "../MineGE/DataObject.h"
#include "../ArxHelper/HelperClass.h"
#include "../DefGE/RcuGE.h"
#include "../DefGE/Pore.h"
#include "../DefGE/DrillSite.h"
#include "../DefGE/RockGate.h"
#include "../DefGE/CoalSurface.h"

void DrawCmd::DrawRcu()
{
	CAcModuleResourceOverride myResources;
	RcuDesignDlg dlg;
	dlg.DoModal();

	AcGePoint3d pt;
	ArxUtilHelper::PromptPt( _T( "\n请选择插入点坐标: " ), pt );
}
