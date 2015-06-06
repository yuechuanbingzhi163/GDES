#include "StdAfx.h"
#include "ReportHelper.h"
#include "CreatReport.h"
#include "GasSysDlg.h"
#include "GasPlanDlg.h"
#include "EPCMFDDlg.h"
#include "SelfManageDlg.h"
#include "InspectionDataDlg.h"
#include "GasPumpDlg.h"
#include "MEREDlg.h"
#include "config.h"

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

static void GetDocPath( CString& defaultPath )
{
	TCHAR pPath[MAX_PATH]={0};
	SHGetSpecialFolderPath(NULL,pPath,CSIDL_PERSONAL,0);

	defaultPath.Format(_T("%s"),pPath);
}

static BOOL SaveAndOpenReport(CString tplName,CString outName,CString mineName = _T(""))
{
	TCHAR szFileFilter[] = _T("doc文档(*.doc)|*.doc||");
	TCHAR szFileExt[] = _T("doc");

	CString defaultPath;
	GetDocPath(defaultPath);

	CFileDialog dlg(FALSE,szFileExt,defaultPath,OFN_READONLY,szFileFilter);///TRUE为OPEN对话框，FALSE为SAVE AS对话框

	TCHAR* pFileBuf = new TCHAR[MAX_PATH * 1024];
	if (NULL == pFileBuf)
	{
		return FALSE;
	}
	_tcscpy(pFileBuf,outName);

	dlg.m_ofn.lpstrFile = pFileBuf;
	//dlg.m_ofn.lpstrFile[0] = NULL;
	dlg.m_ofn.nMaxFile = MAX_PATH * 1024;
	//dlg.m_ofn.lpstrInitialDir = (LPWSTR)(LPCTSTR)defaultPath;
	CString selectedPath;
	if(IDOK == dlg.DoModal())
	{
		selectedPath = dlg.GetPathName();
	}

	else
	{
		return FALSE;
	}

	delete pFileBuf;
	pFileBuf = NULL;

	acutPrintf( _T( "\n报告生成中...\n" ) );
	//初始化com
	if(initword())
	{
		if(!CreatReport(tplName,selectedPath,mineName)) 
		{
			//卸载com
			uninitword();
			acutPrintf( _T( "\n报告生成失败!\n" ) );
			return FALSE;
		}
		//卸载com
		uninitword();
	}
	acutPrintf( _T( "\n报告保存到:%s" ),selectedPath);
	if(IDYES == AfxMessageBox(_T("报告生成成功!是否现在打开?"),MB_YESNO))
	{
		if(initword())
		{
			OpenWordDocument(selectedPath);
			//卸载com
			uninitword();
		}
	}
	return TRUE;
}
void ReportHelper::CreatReportHelper()
{
	CAcModuleResourceOverride myResources;

	AcDbObjectId objId;
	SingleDataObjectHelper::GetObjectId(_T("计算参数"),objId);
	CString gasFromStr;
	DataHelper::GetPropertyData(objId,_T("工作面瓦斯来源"),gasFromStr);

	CString fileName,tplName,outName;
	if (0 == _ttoi(gasFromStr))
	{
		fileName = _T("Datas\\JL\\tpl\\tplOwn.doc");
		outName = _T("瓦斯抽采(本煤层)评测报告");
	}
	else
	{
		fileName = _T("Datas\\JL\\tpl\\tplNear.doc");
		outName = _T("瓦斯抽采(邻近煤层)评测报告");
	}
	tplName = BuildPath( GetAppPathDir(), fileName );
	if(!SaveAndOpenReport(tplName,outName)) return;
}

void ReportHelper::OpenCHMFile()
{
	CString fileName,filePath;
	fileName = _T("\\help\\瓦斯抽采达标暂行规定.chm");
	filePath = BuildPath( GetAppPathDir(), fileName );
	//OpenWordDocument(_T("C:\\Users\\hd\\Desktop\\test\\瓦斯抽采达标暂行规定.chm"));
	OpenWordDocument(filePath);
}

//矿上自己的条例
void ReportHelper::OpenCHMFile2()
{
	CString fileName,filePath;
	fileName = _T("\\help\\瓦斯抽采达标评价工作体系(试行).chm");
	filePath = BuildPath( GetAppPathDir(), fileName );
	OpenWordDocument(filePath);
}
void ReportHelper::CreatBaseReport()
{
	CAcModuleResourceOverride myResources;

	TCHAR szFileFilter[] = _T("doc文档(*.doc)|*.doc||");
	TCHAR szFileExt[] = _T("doc");
	CString defaultPath;
	GetDocPath(defaultPath);
	AcDbObjectId objId;
	SingleDataObjectHelper::GetObjectId(_T("计算参数"),objId);
	CString mineName;
	DataHelper::GetPropertyData(objId,_T("矿名"),mineName);

	CString fileName =  _T("Datas\\JL\\tpl\\tplBase.doc");
	CString outName = mineName + _T("瓦斯抽采基础条件达标评价");
	CString tplName = BuildPath( GetAppPathDir(), fileName );

	tplName = BuildPath( GetAppPathDir(), fileName );
	if(!SaveAndOpenReport(tplName,outName,mineName)) return;
}

void ReportHelper::ListShow()
{
	CAcModuleResourceOverride myResources;
	GasSysDlg dlg;
	dlg.DoModal();
}

void ReportHelper::ShowGasPlanDlg()
{
	CAcModuleResourceOverride myResources;
	GasPlanDlg dlg;
	dlg.DoModal();
}

void ReportHelper::ShowEPCMFDDlg()
{
	CAcModuleResourceOverride myResources;
	EPCMFDDlg dlg;
	dlg.DoModal();
}

void ReportHelper::ShowSelfManageDlg()
{
	CAcModuleResourceOverride myResources;
	SelfManageDlg dlg;
	dlg.DoModal();
}

void ReportHelper::ShowInspectionDataDlg()
{
	CAcModuleResourceOverride myResources;
	InspectionDataDlg dlg;
	dlg.DoModal();
}

void ReportHelper::ShowGasPumpDlg()
{
	CAcModuleResourceOverride myResources;
	GasPumpDlg dlg;
	dlg.DoModal();
}

void ReportHelper::ShowMeasureDlg()
{
	CAcModuleResourceOverride myResources;
	MEREDlg dlg;
	dlg.setObjectName(RET_MERSURE_OBJCT_NAME);
	dlg.setDesText(_T("抽采计量测点是否充足、计量器具是否符合计量标准要求"));
	AcStringArray bookMks;
	bookMks.append(_T("MESURE_Ret"));
	bookMks.append(_T("MESURE_Method"));
	bookMks.append(_T("MESURE_Conclusion"));
	bookMks.append(_T("MESURE_YesOrNo"));
	dlg.setBookMarks(bookMks);
	dlg.DoModal();
}

void ReportHelper::ShowReleatedInfoDlg()
{
	CAcModuleResourceOverride myResources;
	MEREDlg dlg;
	dlg.setObjectName(RET_REALATE_OBJCT_NAME);
	dlg.setDesText(_T("抽采效果评判用相关测试条件是否符合标准要求"));
	AcStringArray bookMks;
	bookMks.append(_T("ReleatInfo_Ret"));
	bookMks.append(_T("ReleatInfo_Method"));
	bookMks.append(_T("ReleatInfo_Conclusion"));
	bookMks.append(_T("ReleatInfo_YesOrNo"));
	dlg.setBookMarks(bookMks);
	dlg.DoModal();
}