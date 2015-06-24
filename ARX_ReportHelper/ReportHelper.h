#pragma once
#include "dlimexp.h"

class ARX_REPORTHELPER_DLLIMPEXP ReportHelper
{
public:
	static void CreatReportHelper();
	static void CreatBaseReport();
	static void CreatRCUReport();
	static void OpenCHMFile();
	static void OpenCHMFile2();
	static void ListShow();
	static void ShowGasPlanDlg();
	static void ShowEPCMFDDlg();
	static void ShowSelfManageDlg();
	static void ShowInspectionDataDlg();
	static void ShowGasPumpDlg();
	static void ShowMeasureDlg();	//计量相关对话框
	static void ShowReleatedInfoDlg();	//评判相关资料
};

