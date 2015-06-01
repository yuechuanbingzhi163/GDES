#include "StdAfx.h"
#include "UIHelper.h"
#include "PumpCapacityDlg.h"

void UIHelper::ShowPumpCapacityDlg()
{
	CAcModuleResourceOverride myResources;
	PumpCapacityDlg dlg;
	dlg.DoModal();
}