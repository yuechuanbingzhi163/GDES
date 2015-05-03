#pragma once

class UIHelper
{
public:
	static void ShowBaseParamDlg();
	static void DestroyBaseParamDockBar();
	static void InputBaseParam();

	static void ShowProjectInfoDlg();

	static void ShowDataInputDockBar();
	static void DestroyDataInputDockBar();

	static void ShowInstrumentDlg();


	static void ShowDataListDockBar();
	static void DestroyDataListDockBar();

	static void CaculateRequirAirFlow();

	static void CaculateChimwindage();

	static void LeakageCacul();
	static void InputLeakageParam();

	static void EnsureFan();
	static void CheakFan();

	static void InsertFan();
	static void FindFanByDB();

	//static AcDbObjectIdArray SelectFan();
	//static AcDbObjectIdArray SelectLib();
	//static void ChimSort();
	//static void SetTunnelLenToDia( AcDbObjectId objId,double lenth);
};

