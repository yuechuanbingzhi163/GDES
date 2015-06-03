#include "StdAfx.h"
#include "UIHelper.h"
#include "PumpCapacityDlg.h"
#include "RealGasQDlg.h"
#include "PreGasDlg.h"
#include "RateGasDlg.h"

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

void UIHelper::ShowPreGasDlg()
{
	CAcModuleResourceOverride myResources;
	PreGasDlg dlg;
	dlg.DoModal();
}

void UIHelper::ShowRateGasDlg()
{
	CAcModuleResourceOverride myResources;
	RateGasDlg dlg;
	dlg.DoModal();
}