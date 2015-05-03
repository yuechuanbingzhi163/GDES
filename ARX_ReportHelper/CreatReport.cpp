#include "stdafx.h"
#include "Resource.h"
#include "CmyWord.h"
#include "CreatReport.h"

#include "ReportDataHelper.h"

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

static BOOL GoToBMark(const CString& strBMark)
{
	COleVariant varBMark(strBMark);	//设置变量
	return MyWord->Goto(varBMark);
}

static void DeleteExcludeLine(const CString& bookMks)
{
	if(GoToBMark(bookMks))
	{
		MyWord->NoneSelectMoveUp(1,wdLine);
		MyWord->Delete(1);
	}
}

static bool CreatTable( const AcStringArray& Name_Sys,const AcStringArray& is_Runing,const AcStringArray& resons, int Num_Sys )
{
	if(Num_Sys <= 0) return false;
	MyWord->CreateTable(Num_Sys+1,3);
	MyWord->SetTableText(1,1,_T("名称"));
	MyWord->SetTableText(1,2,_T("是否正常运转"));
	MyWord->SetTableText(1,3,_T("备注(原因)"));
	
	for(int i = 0; i < Num_Sys; i++)
	{
		MyWord->SetTableText(i+2,1,Name_Sys[i].kACharPtr());
		MyWord->SetTableText(i+2,2,is_Runing[i].kACharPtr());
		MyWord->SetTableText(i+2,3,resons[i].kACharPtr());
	}

	MyWord->NoneSelectMoveDown((short)(Num_Sys+1),wdLine);
	return true;
}

static void WriteMineName(CString& mineName)
{
	if(mineName.IsEmpty())
	{
		mineName = ISNULL;
	}
	if(-1 == mineName.Find(_T("矿")))
	{
		mineName.Append(_T("矿"));
	}
	MyWord->FindWord(_T("{{Mine_Name}}"),mineName);
}


static BOOL ReadDataFromFile(const CString& txtName,AcStringArray& names,AcStringArray& isRunings,AcStringArray& resons)
{
	names.removeAll();
	isRunings.removeAll();
	resons.removeAll();
	ArrayVector dataVector;
	ReportDataHelper::ReadDatas(txtName,dataVector,3);
	if(dataVector.empty()) return FALSE;
	for(int i = 0; i < dataVector.size(); i++)
	{
		names.append(dataVector[i][0]);
		isRunings.append(dataVector[i][1]);
		resons.append(dataVector[i][2]);
	}
	return TRUE;
}

static BOOL ReadTableDataFromFile(const CString& txtName,AcStringArray& names,AcStringArray& datas)
{
	names.removeAll();
	datas.removeAll();
	ArrayVector dataVector;
	ReportDataHelper::ReadDatas(txtName,dataVector,2);
	if(dataVector.empty()) return FALSE;
	for(int i = 0; i < dataVector.size(); i++)
	{
		names.append(dataVector[i][0]);
		datas.append(dataVector[i][1]);
	}
	return TRUE;

}

static void SetGroundSys()
{
	AcStringArray Name_Sys,is_Runing,resons;
	ReadDataFromFile(PERMENT_OBJCT_NAME,Name_Sys,is_Runing,resons);
	GoToBMark(_T("GasSysBookPerm"));
	int Num_Sys = Name_Sys.length();
	if( Num_Sys <= 0 )
	{
		MyWord->InsertSymbol(_T("Wingdings"),-3843);//×
		Num_Sys = 0;
	}
	else
	{
		MyWord->InsertSymbol(_T("Wingdings"),-3842);//√
	}

	CString strNum;
	strNum.Format(_T("%d"),Num_Sys);
	MyWord->FindWord(_T("{{Num_Perm_Sys}}"),strNum);
	GoToBMark(_T("GasSysTablePerm"));
	if(CreatTable(Name_Sys,is_Runing,resons,Num_Sys))
	{
		DeleteExcludeLine(_T("GasSysBookTemp"));
	}
}

static void SetUndergroundSys()
{
	AcStringArray Name_Sys,is_Runing,resons;
	ReadDataFromFile(TEMPGAS_OBJCT_NAME,Name_Sys,is_Runing,resons);
	GoToBMark(_T("GasSysBookTemp"));
	int Num_Sys = Name_Sys.length();
	if( 0 >= Num_Sys )
	{
		MyWord->InsertSymbol(_T("Wingdings"),-3843);//×
		Num_Sys = 0;
	}
	else
	{
		MyWord->InsertSymbol(_T("Wingdings"),-3842);//√
	}

	CString strNum;
	strNum.Format(_T("%d"),Num_Sys);
	MyWord->FindWord(_T("{{Num_Temp_Sys}}"),strNum);

	GoToBMark(_T("GasSysTableTemp"));
	if(CreatTable(Name_Sys,is_Runing,resons,Num_Sys))
	{
		DeleteExcludeLine(_T("GasSys_Ret"));
	}

}

static void SetTableVal(CString txtName)
{
	AcStringArray bookMarks,datas;
	ReadTableDataFromFile(txtName,bookMarks,datas);
	for(int i = 0; i < bookMarks.length(); i++)
	{
		if(!GoToBMark(bookMarks[i].kACharPtr())) continue;
		if(_ttoi(datas[i].kACharPtr()))
		{
			MyWord->InsertSymbol(_T("Wingdings"),-3842);//√
		}
		else
		{
			MyWord->InsertSymbol(_T("Wingdings"),-3843);//×
		}
	}
}

static BOOL ReadSysPumpDatas(ArrayVector& datas)
{
	ArrayVector dataVector;
	ReportDataHelper::ReadDatas(GAS_PUMP_OBJCT_NAME,dataVector,4);
	if(dataVector.empty()) return FALSE;
	AcStringArray sysNames,pumpStates,pumpTypes,pumpAbilities;
	for(int i = 0; i < dataVector.size(); i++)
	{
		sysNames.append(dataVector[i][0]);
		pumpStates.append(dataVector[i][1]);
		pumpTypes.append(dataVector[i][2]);
		pumpAbilities.append(dataVector[i][3]);
	}

	datas.push_back(sysNames);
	datas.push_back(pumpStates);
	datas.push_back(pumpTypes);
	datas.push_back(pumpAbilities);

	return TRUE;
}

static void SetPumpTable()
{
	GoToBMark(_T("PumpTableBook"));
	ArrayVector datas;
	if(!ReadSysPumpDatas(datas)) return;
	//加1表示表头
	int rows = (datas[0].length()) + 1;
	if(rows <= 0) return;

	//写表头
	MyWord->CreateTable(rows,4);
	MyWord->SetTableText(1,1,_T("抽采系统"));
	MyWord->SetTableText(1,2,_T("抽采泵状态"));
	MyWord->SetTableText(1,3,_T("型号"));
	MyWord->SetTableText(1,4,_T("单泵最大抽采能力"));

	//填充抽采系统数据
	for(int i = 2; i <= rows; i += 2)
	{
		int inx = i -2;
		CString strName = datas[0][inx].kACharPtr();
		MyWord->SetTableText(i,1,strName);
	}

	//填充抽采泵状态数据
	for( int i = 2; i <= rows; i++ )
	{
		int inx = i -2;
		CString strStatus = datas[1][inx].kACharPtr();
		MyWord->SetTableText(i,2,strStatus);
	}

	for( int i = 2; i <= rows; i++ )
	{
		int inx = i -2;
		CString strType = datas[2][inx].kACharPtr();
		MyWord->SetTableText(i,3,strType);
	}

	for( int i = 2; i <= rows; i++ )
	{
		int inx = i -2;
		CString strAbility = datas[3][inx].kACharPtr();
		MyWord->SetTableText(i,4,strAbility);
	}

	//合并单元格,最好放在最后(也就是填充数据完毕之后)
 	for(int i = 2; i <= rows; i += 2)
 	{
 		MyWord->MergeTable(i,1,i+1,1);
 	}

	DeleteExcludeLine(_T("GasPump_Ret"));
}

static BOOL SetConclusionTable(CString txtName)
{
	AcStringArray bookMarks,datas;
	if(!ReadTableDataFromFile(txtName,bookMarks,datas)) 
	{
		CString msg = _T("没有操作");
		if (bookMarks.isEmpty())
		{
			msg.Append(txtName);
		}
		AfxMessageBox(msg);
		return FALSE;
	}

	if( GoToBMark(bookMarks[0].kACharPtr()) ) 
	{
		MyWord->WriteText(datas[0].kACharPtr());
	}
	
	int yesOrNoIndx = bookMarks.length() -1;
	for(int i = 1; i < yesOrNoIndx; i++)
	{
		if(!GoToBMark(bookMarks[i].kACharPtr())) continue;
		MyWord->WriteText(datas[i].kACharPtr());
	}

	if( GoToBMark(bookMarks[yesOrNoIndx].kACharPtr()) )
	{
		if(_ttoi(datas[yesOrNoIndx].kACharPtr()))
		{
			MyWord->WriteText(_T("√"));//√
		}
		else
		{
			MyWord->WriteText(_T("×"));//×
		}

	}

	return TRUE;
}

static BOOL SetBaseReportVal(CString& mineName)
{
	SetGroundSys();
	SetUndergroundSys();

	//瓦斯抽采达标规划和年度实施计划表格
	SetTableVal(GAS_PLAN_BOOKMKS_OBJCT_NAME);
	//抽采达标工艺方案设计和采掘工作面施工设计表格
	SetTableVal(EPCMFD_BOOKMKS_OBJCT_NAME);
	//矿井瓦斯抽采达标自评价体系和瓦斯抽采管理制度表格
	SetTableVal(SM_BOOKMKS_OBJCT_NAME);
	//瓦斯抽采工程验收资料表格
	SetTableVal(INSPECT_BOOKMKS_OBJCT_NAME);
	//瓦斯泵站能力
	SetPumpTable();

	AcStringArray names;
	names.append(RET_GASSYS_OBJCT_NAME);
	names.append(RET_EPCMFD_OBJCT_NAME);
	names.append(RET_GASPLAN_OBJCT_NAME);
	names.append(RET_INSPECT_OBJCT_NAME);
	names.append(RET_SM_OBJCT_NAME);
	names.append(RET_GASPUMP_OBJCT_NAME);
	names.append(RET_MERSURE_OBJCT_NAME);
	names.append(RET_REALATE_OBJCT_NAME);

	//结论表格的书写
	for(int i = 0; i < names.length(); i++)
	{
		CString name = names[i].kACharPtr();
		if(!SetConclusionTable(name)) return FALSE;
	}
	WriteMineName(mineName);
	//MyWord->SetSeekView(mineName,wdAlignParagraphJustify,wdSeekCurrentPageHeader,_T("{{MineName_Header}}"));
	//DeleteExcludeLine();
	return TRUE;
}

static void SetFieldsValue()
{
	//把你生成的数据文件固定在datas路径下
	//文件名固定为vars.txt即可
	CString dataDirName = _T( "Datas\\" );
	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T( "vars.txt" ) );

	AcIfstream inFile(fileName);
	//acutPrintf(_T("\n%s"),fileName);
	if(!inFile) return;
	int i = 0;
	CString mineName;
	while( !inFile.eof() )
	{
		i = i + 1;
		//acutPrintf(_T("\n读取文件%d"),i);
		ACHAR valueField[_MAX_PATH], value[_MAX_PATH];
		inFile >> valueField >> value;
		if(inFile.fail()) break;
		CString valueFieldStr,valueStr;
		valueFieldStr.Format(_T("%s"),valueField);
		valueStr.Format(_T("%s"),value);
		//acutPrintf(_T("\n名字:%s,值:%s"),valueFieldStr,valueStr);
		MyWord->FindWord(valueFieldStr,valueStr);
		if(_T("{{Mine_Name}}") == valueFieldStr)
		{
			mineName = valueStr;
		}
		//MyWord->FindWord(_T("Quality_Water"),_T("√"));
	}
	inFile.close();
	//MyWord->SetSeekView(mineName,wdAlignParagraphJustify,wdSeekCurrentPageHeader,_T("{{MineName_Header}}"));
}

static BOOL SaveReport(CString savePath)
{
	if(CheckDocIsUsing(savePath)) return FALSE;
	return MyWord->SaveDocumentAs(savePath);
}

bool wordOprate(CString templPath,CString savePath,CString& mineName)
{
	AfxGetMainWnd()->BeginWaitCursor();//设置等待光标
	if(CheckDocIsUsing(templPath)) return false;
	MyWord->CreateApp();
	//MyWord->ShowApp();
	if(!MyWord->Open(/*strPath*/templPath))
	{
		//MyWord->CloseApp();
		return false;
	}

	//基础条件达标评价
	if(-1 != templPath.Find(_T("tplBase"))) 
	{
		if(!SetBaseReportVal(mineName)) return false;
	}

	//抽采达标评价报告
	else
	{
		SetFieldsValue();
	}
	MyWord->UpdateField(_T("CONT_UPDATE"));
	bool ret;
	if(!SaveReport(savePath)) ret = false;
	else ret = true;

	//关闭之前把书签关闭
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

bool CreatReport(const CString& tplPath,const CString& savePath,CString& mineName)
{
	return wordOprate(tplPath,savePath,mineName);
}

void OpenDoc(const CString& docPath,BOOL isVisiable)
{
	MyWord->CreateApp();
	if(isVisiable) MyWord->ShowApp();
	else MyWord->HideApp();
	if(!MyWord->Open(/*strPath*/docPath))
	{
		MyWord->CloseApp();
		return;
	}
	SaveReport(docPath);
}

void OpenWordDocument( const CString& filePath )
{
	ShellExecute( NULL, _T( "open" ), filePath, NULL, NULL, SW_SHOWNORMAL );
}