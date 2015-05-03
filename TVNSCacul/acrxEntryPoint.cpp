#include "StdAfx.h"
#include "resource.h"

#include "UIHelper.h"
#include "ReactorHelper.h"
//#include "../MineGE/HelperClass.h"
//#include "../ArxHelper/HelperClass.h"
//#include "../ReportHelper/CreatReport.h"
//#include "ReportHelper/CreatReport.h"
#include "treeTestDlg.h"
//#include "PropertyDlg.h"
#include "../MineGE/DataListHelper.h"
#include "../MineGE/DataHelper.h"

#include <iostream>
#include <fstream>
using namespace std;

#include "AcFStream.h"

// 定义注册服务名称
#ifndef TVNSCACUL_SERVICE_NAME
#define TVNSCACUL_SERVICE_NAME _T("TVNSCACUL_SERVICE_NAME")
#endif

static CString GetAppPathDir()
{
	TCHAR szModulePath[_MAX_PATH];
	GetModuleFileName( _hdllInstance, szModulePath, _MAX_PATH );

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	_tsplitpath( szModulePath, drive, dir, NULL, NULL );

	TCHAR szPath[_MAX_PATH] = {0};
	_tmakepath( szPath, drive, dir, NULL, NULL );

	return CString( szPath );
}

static CString BuildPath( const CString& dir, const CString& fileName )
{
	CString path;
	path.Format( _T( "%s%s" ), dir, fileName );
	return path;
}

static bool GetFieldsValue()
{
	CString dataDirName = _T( "Datas\\" );
	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T( "fv.txt" ) );

	AcIfstream inFile(fileName);
	AcStringArray fields,values;
	//acutPrintf(_T("\n%s"),fileName);
	if(!inFile) return false;
	while( !inFile.eof() )
	{
		ACHAR valueField[_MAX_PATH], value[_MAX_PATH];
		inFile >> valueField >> value;
		if(inFile.fail()) break;
		CString valueFieldStr,valueStr;
		valueFieldStr.Format(_T("%s"),valueField);
		valueStr.Format(_T("%s"),value);
		fields.append(valueFieldStr);
		values.append(value);
	}

	inFile.close();

	dataDirName = _T( "Datas\\" );
	fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T( "vars.txt" ) );


	AcOfstream outFile(fileName);
	if (!outFile) return false;
	AcDbObjectId objId;
	SingleDataObjectHelper::GetObjectId(_T("计算参数"),objId);

	int len = fields.length();
	for ( int i = 0; i < len; i++ )
	{
		CString fieldValueStr;
		DataHelper::GetPropertyData(objId,fields[i].kACharPtr(),fieldValueStr);
		if (fieldValueStr.IsEmpty())
		{
			CString msg = fields[i].kACharPtr();
			msg.Append(_T("\n\t未知"));
			AfxMessageBox(msg);
			return false;
		}
		outFile << values[i].kACharPtr() << _T("\t") << (LPWSTR)(LPCTSTR)fieldValueStr << _T("\n");
		//acutPrintf(_T("\n写入:%s->%s"),values[i].kACharPtr(),fieldValueStr);
	}

	outFile.close();
	return true;
}
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CTVNSCaculApp : public AcRxArxApp {

public:
	CTVNSCaculApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		ReactorHelper::CreateSelectedGE_EditorReactor();	
		acrxRegisterService( TVNSCACUL_SERVICE_NAME );

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		
		ReactorHelper::RemoveSelectedGE_EditorReactor();
		delete acrxServiceDictionary->remove( TVNSCACUL_SERVICE_NAME );
		
		UIHelper::DestroyDataInputDockBar();
		UIHelper::DestroyBaseParamDockBar();

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

	static void JL_baseParamDlg( void )
	{
		UIHelper::ShowBaseParamDlg();
		//acedCommand(RTSTR,_T("_regen"),0);
	}

	static void JL_caculParamDlg()
	{
		UIHelper::InputBaseParam();
	}

	static void JL_caculateQ()
	{
		UIHelper::CaculateRequirAirFlow();
	}

	static void JL_caculateR()
	{
		UIHelper::CaculateChimwindage();
	}

	static void JL_CaculateLeakage()
	{
		UIHelper::LeakageCacul();
	}

	static void JL_LeakageParamInput()
	{
		UIHelper::InputLeakageParam();
	}

	static void JL_FansEnsured()
	{
		UIHelper::EnsureFan();
	}

	static void JL_FanCheaked()
	{
		UIHelper::CheakFan();
	}

	static void JL_InsertNewFan()
	{
		UIHelper::InsertFan();
	}

	static void JL_FindFan()
	{
		UIHelper::FindFanByDB();
	}
	static void JL_Test( void )
	{
// 		GetFieldsValue();
// 		acedCommand(RTSTR,_T("ARXWORD"),0);
		//TreeTestDlg dlg;
		//dlg.DoModal();
		//CreatReport(tempcurrentPath,savecurrentPath);
		//CAcModuleResourceOverride myResources;
		//PropertyDlg dlg;
		//dlg.DoModal();
		//UIHelper::EnsureFan();
		//CString ss;
		//acutPrintf(_T("\nxxx:%s"),ss);
		//TWSDatas();
		//TunnelDatas();
		//ChimneyDatas();
		//BaseparamDatas();
		//UIHelper::LeakageCacul();
		//SetAlpha(0);
		//CString ret = GetAlpha(0,_T("400"));
		//UIHelper::ChimSort();
		//acutPrintf(_T("\n\nret->%s"),ret);
	}

	static void JL_testDataInputDlg( void )
	{
		UIHelper::ShowDataInputDockBar();
		//acedCommand(RTSTR,_T("_regen"),0);
	}


} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CTVNSCaculApp)
ACED_ARXCOMMAND_ENTRY_AUTO( CTVNSCaculApp, JL, _Test, hdtest, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CTVNSCaculApp, JL, _testDataInputDlg, testDataInputDlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CTVNSCaculApp, JL, _baseParamDlg, hdBaseParamdlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CTVNSCaculApp, JL, _caculParamDlg, hdCaculParamdlg, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CTVNSCaculApp, JL, _caculateQ, hdCaculateQ, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CTVNSCaculApp, JL, _caculateR, caculateR, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CTVNSCaculApp, JL, _CaculateLeakage, CaculateLeakage, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CTVNSCaculApp, JL, _LeakageParamInput, LeakageParamInput, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CTVNSCaculApp, JL, _FansEnsured, FansEnsured, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CTVNSCaculApp, JL, _FanCheaked, FanCheaked, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CTVNSCaculApp, JL, _InsertNewFan, InsertNewFan, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CTVNSCaculApp, JL, _FindFan, FindFan, ACRX_CMD_TRANSPARENT, NULL )
