#include "stdafx.h"
#include "CmyWord.h"
#include "CreatReport.h"
#include "RcuHelper.h"
#include "../ArxHelper/ArxUtilHelper.h"

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

static BOOL CheckDocIsUsing(const CString& fileName)
{
	if (MyWord->isUsing(fileName))
	{
		CString msg;
		msg.Format(_T("\"%s\"\n文档已经打开，请先关闭。。。"),fileName);
		AfxMessageBox(msg,MB_OK|MB_ICONSTOP);
		return TRUE;
	}
	return FALSE;
}

static void CreatDrillTable(const DrillSiteLink& ds_link)
{
	//MyWord->SetFont(_T("宋体"),22);
	MyWord->WriteText(_T("钻场参数"));
	//加1表示表头
	int rows = 2;
	if(rows <= 0) return;

	//写表头
	MyWord->CreateTable(rows,10);
	MyWord->SetTableText(1,1,_T("名称"));
	MyWord->SetTableText(1,2,_T("宽度"));
	MyWord->SetTableText(1,3,_T("高度"));
	MyWord->SetTableText(1,4,_T("最小法距"));
	MyWord->SetTableText(1,5,_T("迎头底板中心点坐标"));
	MyWord->SetTableText(1,6,_T("左帮保护"));
	MyWord->SetTableText(1,7,_T("右帮保护"));
	MyWord->SetTableText(1,8,_T("上帮保护"));
	MyWord->SetTableText(1,9,_T("下帮保护"));
	MyWord->SetTableText(1,10,_T("孔径"));

	MyWord->SetTableText(2,1,ds_link.m_name);
	CString value;
	value.Format(_T("%.2f"),ds_link.m_width);
	MyWord->SetTableText(2,2,value);
	value.Format(_T("%.2f"),ds_link.m_height);
	MyWord->SetTableText(2,3,value);
	value.Format(_T("%.2f"),ds_link.m_dist);
	MyWord->SetTableText(2,4,value);
	value = ArxUtilHelper::Point3dToString(ds_link.m_pt);
	MyWord->SetTableText(2,5,value);
	value.Format(_T("%.2f"),ds_link.m_left);
	MyWord->SetTableText(2,6,value);
	value.Format(_T("%.2f"),ds_link.m_right);
	MyWord->SetTableText(2,7,value);
	value.Format(_T("%.2f"),ds_link.m_top);
	MyWord->SetTableText(2,8,value);
	value.Format(_T("%.2f"),ds_link.m_bottom);
	MyWord->SetTableText(2,9,value);
	value.Format(_T("%.2f"),ds_link.m_pore_size);
	MyWord->SetTableText(2,10,value);
	MyWord->NoneSelectMoveDown(rows,5);
	//MyWord->InsetBookMark(_T("DRILL_SITE_TABLE"));
}

static void CreatCoalTable(const CoalSurfaceLink& cs_link)
{
	//MyWord->Goto(_T("DRILL_SITE_TABLE"));
	MyWord->WriteText(_T("煤层参数"));
	//加1表示表头
	int rows = 2;
	if(rows <= 0) return;

	//写表头
	MyWord->CreateTable(rows,5);
	MyWord->SetTableText(1,1,_T("煤厚"));
	MyWord->SetTableText(1,2,_T("倾角"));
	MyWord->SetTableText(1,3,_T("钻孔抽采半径"));
	MyWord->SetTableText(1,4,_T("抽采高度"));
	MyWord->SetTableText(1,5,_T("抽采宽度"));

	CString value;
	value.Format(_T("%.2f"),cs_link.m_thick);
	MyWord->SetTableText(2,1,value);
	value.Format(_T("%.2f"),cs_link.m_angle);
	MyWord->SetTableText(2,2,value);
	value.Format(_T("%.2f"),cs_link.m_gas_radius);
	MyWord->SetTableText(2,3,value);
	value.Format(_T("%.2f"),cs_link.m_height);
	MyWord->SetTableText(2,4,value);
	value.Format(_T("%.2f"),cs_link.m_width);
	MyWord->SetTableText(2,5,value);
	MyWord->NoneSelectMoveDown(rows,5);
	//MyWord->InsetBookMark(_T("COAL_SURFACE_TABLE"));
}

static void CreatPoreTable(const AcDbObjectId& drill_site, const AcDbObjectId& coal_surf)
{
	MyWord->WriteText(_T("钻孔参数"));

	AcDbObjectIdArray openPores,closePores;
	RcuHelper::GetRelatedOpenPores(drill_site,openPores);
	RcuHelper::GetRelatedClosePores(coal_surf,closePores);
	if(openPores.length() != closePores.length()) return;
	int rows = openPores.length() + 1;
	if(rows <= 0) return;

	//写表头
	MyWord->CreateTable(rows,7);
	MyWord->SetTableText(1,1,_T("开孔编号"));
	MyWord->SetTableText(1,2,_T("开孔坐标"));
	MyWord->SetTableText(1,3,_T("终孔编号"));
	MyWord->SetTableText(1,4,_T("终孔坐标"));
	MyWord->SetTableText(1,5,_T("仰角"));
	MyWord->SetTableText(1,6,_T("偏角"));
	MyWord->SetTableText(1,7,_T("长度"));
	
	//数据填充，从表格中的第三行开始些数据
	//列数据分别为:开孔编号、开孔坐标、终孔编号、终孔坐标、仰角、偏角、长度
	for(int i = 0; i < openPores.length(); i++)
	{
		PoreLink op_link,cp_link;
		RcuHelper::ReadPoreData(openPores[i],op_link);
		RcuHelper::ReadPoreData(closePores[i],cp_link);
		CString value;
		value.Format(_T("%d"),op_link.m_pore_num);
		MyWord->SetTableText(2+i,1,value);
		value = ArxUtilHelper::Point3dToString(op_link.m_pt);
		MyWord->SetTableText(2+i,2,value);
		value.Format(_T("%d"),cp_link.m_pore_num);
		MyWord->SetTableText(2+i,3,value);
		value = ArxUtilHelper::Point3dToString(cp_link.m_pt);
		MyWord->SetTableText(2+i,4,value);

	}
	//MyWord->NoneSelectMoveDown(rows,5);
}

static bool WriteRCUDataToReport()
{
	MyWord->WriteText(_T("石门设计报告"),wdAlignParagraphCenter);
	MyWord->TypeParagraph();
	AcDbObjectIdArray drill_sites;
	RcuHelper::FindAllDrillSite(drill_sites);
	if(drill_sites.isEmpty())
	{
		AfxMessageBox(_T("未找到相关钻场信息!"),MB_OK | MB_ICONSTOP);
		return false;
	}

	for(int i = 0; i < drill_sites.length(); i++)
	{
		AcDbObjectId drill_site = drill_sites[i];
		AcDbObjectId coal_surf;
		RcuHelper::GetRelatedCoalSurface(drill_site,coal_surf);
		//获取钻场数据
		DrillSiteLink ds_link;
		if(!RcuHelper::ReadDrillSiteData(drill_site, ds_link)) return false;
		CreatDrillTable(ds_link);
		//获取煤层数据
		CoalSurfaceLink cs_link;
		if(!RcuHelper::ReadCoalSurfaceData(coal_surf,cs_link)) return false;
		CreatCoalTable(cs_link);
		CreatPoreTable(drill_site,coal_surf);
	}
	return true;
}

static BOOL SaveReport(CString savePath)
{
	if(CheckDocIsUsing(savePath)) return FALSE;
	return MyWord->SaveDocumentAs(savePath);
}


static bool wordOprate(CString savePath)
{
	AfxGetMainWnd()->BeginWaitCursor();//设置等待光标
	if(!MyWord->CreateDocuments())
	{
		return false;
	}
	if(!MyWord->CreateDocument())
	{
		return false;
	}

	if(!WriteRCUDataToReport()) 
	{
		MyWord->CloseDocument();
		MyWord->CloseApp();
		AfxGetMainWnd()->EndWaitCursor();//结束等待光标
		return false;
	}
	bool ret;
	if(!SaveReport(savePath)) ret = false;
	else ret = true;

	MyWord->ShowBookmarks(FALSE);
	MyWord->CloseDocument();
	MyWord->CloseApp();
	AfxGetMainWnd()->EndWaitCursor();//结束等待光标
	return ret;
}

bool initword()
{
	//CoInitialize返回的是HResult类型的值
	//要用SUCCEDED或FAILED宏来判断成功或失败
	//如果用S_OK或S_FALSE来判断是错误的!!!
	//你可以用VC助手看看这4个宏的定义!!!
	if(FAILED(CoInitialize(NULL)))
	//if(CoInitialize(NULL)!=S_OK) // 错误的判断方法!!!
	{
		AfxMessageBox(_T("初始化com库失败"));
		return false;
	}

	MyWord = new CmyWord;
	return true;
}

void uninitword()
{
	delete MyWord;
	MyWord = 0;

	//释放资源最好从 小到大的顺序来释放。这个和c里面一些释放资源的道理是一样的
	//和c+= 先析构儿子 再析构父亲是一样的。
	CoUninitialize();
}

bool CreatReport( const CString& savePath )
{
	return wordOprate(savePath);
}

void OpenWordDocument( const CString& filePath )
{
	ShellExecute( NULL, _T( "open" ), filePath, NULL, NULL, SW_SHOWNORMAL );
}