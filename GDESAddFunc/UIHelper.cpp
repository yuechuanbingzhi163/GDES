#include "StdAfx.h"
#include "UIHelper.h"
#include "PumpCapacityDlg.h"
#include "RealGasQDlg.h"

void UIHelper::ShowPumpCapacityDlg()
{
	CAcModuleResourceOverride myResources;
	PumpCapacityDlg dlg;
	dlg.DoModal();
}

void UIHelper::ShowRealGasDlg()
{
	CAcModuleResourceOverride myResources;
	RealGasQDlg dlg;
	dlg.DoModal();

}