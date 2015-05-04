#include "StdAfx.h"

#include "UIHelper.h"

#include "PathBuild.h"

#include "../MineGE/DataListHelper.h"
#include "../MineGE/DataHelper.h"
#include "../ArxHelper/ArxUtilHelper.h"
#include "../ARX_ReportHelper/ReportHelper.h"

#include <iostream>
#include <fstream>
using namespace std;
#include "AcFStream.h"

#include "DataList_DockBar.h"

#define NONRET _T("**")

enum suportPatten
{
	FULLY_MECHANIZED_SUPORT	=	0,		//综采支撑式
	FULLY_MECHANIZED_CORVE	=	1,		//综采掩护式
	CONCENTIONAL_MONOMER	=	2		//普采单体式 

};

DataList_DockBar* pDataList_DockBar = 0;

static void GetGasLitmit(const CString& dayOutputStr ,CString& valueStr)
{
	if(dayOutputStr.IsEmpty()) return;
	int dayOutput = _ttoi(dayOutputStr);
	double value;
	if(dayOutput <= 1000 ) value = 8;
	if(dayOutput >= 1001 && dayOutput <= 2500) value = 7;
	if(dayOutput >= 2501 && dayOutput <= 4000) value = 6;
	if(dayOutput >= 4001 && dayOutput <= 6000) value = 5.5;
	if(dayOutput >= 6001 && dayOutput <= 8000) value = 5;
	if(dayOutput >= 8001 && dayOutput <= 10000) value = 4.5;
	if(dayOutput > 10000 ) value = 4;
	valueStr.Format(_T("%.1lf"),value);
}

static void GetMineDesc( CString& locaDesc ,CString& numDesc)
{
	CString dataDirName = _T( "Datas\\" );
	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("locaDesc.txt") );

	AcIfstream inFile(fileName);
	if(!inFile) return;
	while( !inFile.eof() )
	{
		ACHAR /*charDesName[_MAX_PATH],*/ charLoca[_MAX_PATH];
		inFile /*>> charDesName*/ >> charLoca;
		CString chDesc;
		chDesc.Format(_T("%s"),charLoca);
		if(chDesc != _T("\n"))
		{
			locaDesc.Append(chDesc);
		}
	}
	inFile.close();

	fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("numDesc.txt") );

	//inFile.open(fileName);
	AcIfstream inFile2(fileName);
	if(!inFile2) return;
	while( !inFile2.eof() )
	{
		ACHAR charLoca[_MAX_PATH];
		inFile2 >> charLoca;
		CString chDesc;
		chDesc.Format(_T("%s"),charLoca);
		if(chDesc != _T("\n"))
		{
			numDesc.Append(chDesc);
		}
		//acutPrintf(_T("\n数据:%s"),numDesc);
	}
	inFile2.close();

	if(locaDesc.IsEmpty()) locaDesc = NONRET;
	if(numDesc.IsEmpty()) numDesc = NONRET;
}

static void CaculFaceSV( const AcDbObjectId& objId, CString& faceAre, CString& faceSpeed)
{
	CString suportPattenIndx,thickless,minCtrl,airQuantity;
	DataHelper::GetPropertyData(objId,_T("支护方式"),suportPattenIndx);
	DataHelper::GetPropertyData(objId,_T("工作面开采高度"),thickless);
	DataHelper::GetPropertyData(objId,_T("最小控顶距"),minCtrl);
	DataHelper::GetPropertyData(objId,_T("工作面配风量"),airQuantity);

	if(suportPattenIndx.IsEmpty())
	{
		if(faceAre.IsEmpty()) faceAre = NONRET;
		if (faceSpeed.IsEmpty()) faceSpeed = NONRET;
		return;
	}

	double fThickless = _tstof(thickless);
	double fMinCtrl = _tstof(minCtrl);
	double fQ	= _tstof(airQuantity);

	double fArea,fSpped;
	switch(_ttoi(suportPattenIndx))
	{
	case FULLY_MECHANIZED_SUPORT:
		fArea = 3.75 * (fThickless - 0.3 );
		break;
	case FULLY_MECHANIZED_CORVE:
		fArea = 3 * (fThickless - 0.3 );
		break;
	case CONCENTIONAL_MONOMER:
		fArea = fMinCtrl * fThickless;
		break;
	default:
		break;
	}

	fSpped = fQ / fArea / 60;

	faceAre.Format(_T("%lf"),fArea);
	faceSpeed.Format(_T("%lf"),fSpped);

	if(faceAre.IsEmpty()) faceAre = NONRET;
	if (faceSpeed.IsEmpty()) faceSpeed = NONRET;
}

static void CaculGasRate( const AcDbObjectId& objId, CString& gasRate )
{
	CString gasGussing,airQuantity;
	DataHelper::GetPropertyData(objId,_T("风排瓦斯量"),gasGussing);
	DataHelper::GetPropertyData(objId,_T("工作面配风量"),airQuantity);

	if(gasGussing.IsEmpty() || airQuantity.IsEmpty()) return;
	double gG = _tstof(gasGussing);
	double aQ = _tstof(airQuantity);
	double gR = gG / (aQ);
	gR = gR * 100;
	gasRate.Format(_T("%lf"),gR);
	DataHelper::SetPropertyData(objId,_T("工作面瓦斯浓度"),gasRate);
}

static void DealSpPoints(CString& value)
{
	//小数点前面的0补全,并且除掉左右多余的0
	if(value.Find(_T('.')) == -1) return;
	CString strValue;
	strValue.Format(_T("%.2f"),_tstof(value));
	value = strValue;
	value.Replace(_T("0"),_T(" "));	//替换0为空格
	value.Trim();	//裁剪
	value.Replace(_T(" "),_T("0"));
	if(value[0] == _T('.')) value.Insert(0,_T("0"));
	int lenth = value.GetLength();
	if(0 == lenth)
	{
		value = NONRET;
		return;
	}
	if(value[lenth-1] == _T('.'))
	{
		value.Replace(_T("."),_T(" "));
		value.Trim();	//裁剪
	}
}
static void DealWithSpicFields(const AcDbObjectId& objId, const CString& field, CString& value)
{
	//没有设定值的处理
	if (value.IsEmpty() /*|| _T("") == value*//* || _tstof(value) <= 0*/)
	{
		//fieldValueStr = _T("");
		value = NONRET;
		//fieldValueStr.Append(fields[i].kACharPtr());
		//fieldValueStr.Append(_T("未知值**"));
	}

	//矿名的补全
	if(_T("矿名") == field && -1 == value.Find(_T("矿")))
	{
		value.Append(_T("矿"));
	}

	//工作面名的补全
	if(_T("采煤工作面") == field && -1 == value.Find(_T("工作面")))
	{
		value.Append(_T("工作面"));
	}

	if( _T("煤尘爆炸性") == field )
	{
		switch(_ttoi(value))
		{
		case 0:
			value = _T("无");
			break;
		case 1:
			value = _T("有");
			break;
		default:
			value = NONRET;
			break;
		}
	}

	if(_T("工作面瓦斯来源") == field)
	{
		switch(_ttoi(value))
		{
		case 0:
			value = _T("本煤层");
			break;
		case 1:
			value = _T("邻近煤层");
			break;
		default:
			value = NONRET;
			break;
		}
	}

	if ( _T("实测残余相对瓦斯压力") == field )
	{
		CString strPress;
		if( value.IsEmpty() || _tstof(value) <= 0 )
		{
			DataHelper::GetPropertyData(objId,_T("残余相对瓦斯压力"),strPress);
		
			if (strPress.IsEmpty() || _tstof(value) <= 0 )
			{
				value = _T("0");
				return;
			}
		}
	}
	if(_T("煤尘爆炸指数") != field)
	{
		DealSpPoints(value);
	}
}

static bool WriteDataToVartxt(AcDbObjectId objId,const AcStringArray& zhName, const AcStringArray& enName)
{
	CString dataDirName = _T( "Datas\\" );
	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T( "vars.txt" ) );
	
	CString dayOutputStr;
	DataHelper::GetPropertyData(objId,_T("拟定日产量"),dayOutputStr);
	if(_tstof(dayOutputStr) <= 0)
	{
		CString msg;
		msg.Format(_T("拟定日产量没有数据"));
		AfxMessageBox(msg);
		return false;
	}
	AcOfstream outFile(fileName);
	if (!outFile) return false;
	//AcDbObjectId objId;
	//SingleDataObjectHelper::GetObjectId(_T("计算参数"),objId);

	int len = zhName.length();
	for ( int i = 0; i < len; i++ )
	{
		CString value;
		DataHelper::GetPropertyData(objId,zhName[i].kACharPtr(),value);
		DealWithSpicFields(objId,zhName[i].kACharPtr(),value);
		outFile << enName[i].kACharPtr() << _T("\t") << (LPWSTR)(LPCTSTR)value << _T("\n");
		//acutPrintf(_T("\n字段:%s\t值:%s\n"),zhName[i].kACharPtr(),enName[i].kACharPtr());
	}

	//评价范围内的可解吸瓦斯含量可解吸瓦斯含量
	CString gasLimStr,actualRGStr,actualAGStr,wCCStr;
	GetGasLitmit(dayOutputStr,gasLimStr);
	if(gasLimStr.IsEmpty() ||  _T("NULL") == gasLimStr) gasLimStr = NONRET;
	DealSpPoints(gasLimStr);
	outFile  << _T("{{Analysis_Gas_Limit}}")  << _T("\t") << (LPWSTR)(LPCTSTR)gasLimStr << _T("\n");
	//评价范围内的可解吸瓦斯含量 
	CString strAnalysis;
	DataHelper::GetPropertyData(objId,_T("可解吸瓦斯含量"),strAnalysis);
	if (strAnalysis.IsEmpty())
	{
		CString msg;
		msg.Format(_T("可解析瓦斯含量没有数据"));
		AfxMessageBox(msg);
		return false;
	}

	if ( _tstof(gasLimStr) - _tstof(strAnalysis) < 0.00001)
	{
		CString msg;
		msg.Format(_T("评价范围内的可解吸瓦斯含量【%.2fm3/t】大于可解吸瓦斯含量【%.2fm3/t】\n不满足瓦斯抽放要求!"), _tstof(strAnalysis),_tstof(gasLimStr));
		AfxMessageBox(msg);
		return false;
	}

	DataHelper::GetPropertyData(objId,_T("实测残余瓦斯含量"),actualRGStr);
	//标准大气压力下的残存瓦斯含量
	DataHelper::GetPropertyData(objId,_T("标准大气压力下的残存瓦斯含量"),wCCStr);

	double actualAG = _tstof(actualRGStr) - _tstof(wCCStr);
	actualAGStr.Format(_T("%lf"),actualAG);
	if( actualAG <= 0 ) actualAGStr = NONRET;
	DealSpPoints(actualAGStr);
	outFile  << _T("{{Actual_Analysis_Gas}}")  << _T("\t") << (LPWSTR)(LPCTSTR)actualAGStr << _T("\n") ;
	//acutPrintf(_T("\n字段:%s\t值:%s\n"),_T("{{Analysis_Gas_Limit}}"),gasLimStr);
	CString locaDesc,numDesc;
	GetMineDesc(locaDesc,numDesc);
	outFile  << _T("{{Location_Desc}}")  << _T("\t") << (LPWSTR)(LPCTSTR)locaDesc << _T("\n") ;
	outFile  << _T("{{Num_Desc}}")  << _T("\t") << (LPWSTR)(LPCTSTR)numDesc << _T("\n") ;

	CString gasRate;
	CaculGasRate(objId,gasRate);
	if (_tstof(gasRate) >= 1 || _tstof(gasRate) <= 0)
	{
		outFile.close();
		CString msg;
		msg.Format(_T("工作面瓦斯浓度为:%.2lf%s>1%s,不满足瓦斯抽放要求!"), _tstof(gasRate),_T("%"),_T("%"));
		AfxMessageBox(msg);
		return false;
	}

	DealSpPoints(gasRate);
	outFile  << _T("{{Face_Gas_Density}}")  << _T("\t") << (LPWSTR)(LPCTSTR)gasRate << _T("\n") ;

	CString areS,speedV;
	CaculFaceSV(objId,areS,speedV);
	if(_tstof(speedV) <= 0 || _tstof(speedV) > 4)
	{
		outFile.close();
		CString msg;
		msg.Format(_T("风速为:%.2lfm/s,不满足瓦斯抽放要求!"), _tstof(speedV));
		AfxMessageBox(msg);
		return false;
	}

	DealSpPoints(areS);
	DealSpPoints(speedV);
	outFile  << _T("{{Face_Equivalent_Area}}")  << _T("\t") << (LPWSTR)(LPCTSTR)areS << _T("\n") ;
	outFile  << _T("{{Face_Wind_Speed}}")  << _T("\t") << (LPWSTR)(LPCTSTR)speedV << _T("\n") ;
	
	outFile.close();

	return true;
}

static bool GetFieldsValue()
{
	AcDbObjectId objId;
	SingleDataObjectHelper::GetObjectId(_T("计算参数"),objId);
	
	CString gasFromStr;
	DataHelper::GetPropertyData(objId,_T("工作面瓦斯来源"),gasFromStr);

	CString fvName;
	if (0 == _ttoi(gasFromStr))
	{
		fvName = _T("fvOwn.txt");
	}
	else
	{
		fvName = _T("fvNearBy.txt");
	}

	CString dataDirName = _T( "Datas\\" );
	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),fvName );

	AcIfstream inFile(fileName);
	AcStringArray zhNames,enNames;
	//acutPrintf(_T("\n%s"),fileName);
	if(!inFile) return false;
	while( !inFile.eof() )
	{
		ACHAR zhName[_MAX_PATH], enName[_MAX_PATH];
		inFile >> zhName >> enName;
		if(inFile.fail()) break;
		CString strZhName,strEnName;
		strZhName.Format(_T("%s"),zhName);
		strEnName.Format(_T("%s"),enName);
		zhNames.append(zhName);
		enNames.append(enName);
	}
	inFile.close();
	return WriteDataToVartxt(objId,zhNames,enNames);
}

void UIHelper::CreatReport()
{
	if(!GetFieldsValue()) return;
	ReportHelper::CreatReportHelper();
	//acedCommand(RTSTR,_T("ARXWORD"),0);
}

void UIHelper::ShowDataListDockBar(const AcDbObjectId& sObjId, const AcDbObjectId& tObjId)
{
	CAcModuleResourceOverride myResources;

	CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();

	if( pDataList_DockBar == 0 )
	{
		pDataList_DockBar = new DataList_DockBar();
		pDataList_DockBar->Create ( pAcadFrame, _T( "路径显示" ) ) ;
		pDataList_DockBar->EnableDocking ( CBRS_ALIGN_ANY ) ;
		pDataList_DockBar->RestoreControlBar () ;

	}
	//设置路径查找的始末点所在分支(代码比较恶心)
	pDataList_DockBar->mChildDlg.sObjId = sObjId;
	pDataList_DockBar->mChildDlg.tObjId = tObjId;
	pAcadFrame->ShowControlBar( pDataList_DockBar, TRUE, FALSE );
}

void UIHelper::DestroyDataListDockBar()
{
	if( pDataList_DockBar != 0 )
	{
		pDataList_DockBar->DestroyWindow();
		delete pDataList_DockBar;
		pDataList_DockBar = 0;
	}
}

void UIHelper::SetGESize()
{

	AcDbObjectId objId = ArxUtilHelper::SelectObject(_T("选择一个设施(瓦斯泵、放水器等)"));
	if (!ArxUtilHelper::IsEqualType( _T( "FacilityGE" ), objId))
	{
		if(!ArxUtilHelper::IsEqualType( _T( "GasPumpGE" ), objId))
		{
			AfxMessageBox(_T("所选不是设施!"));
			return;
		}
	}

	ACHAR tt[100];
	acedGetString( 0, _T("输入大小:"), tt );
	CString lenthStr;
	lenthStr.Format(_T("%s"),tt);
	DataHelper::SetPropertyData(objId,_T("大小"),lenthStr );

}

void UIHelper::GetPumpDB()
{
	int x = 1000;
	CString strX;
	strX.Format(_T("%d"),x);
	LPCTSTR sql = _T("select * from TypeTable where weight <= 1000 ");
	CString dataDirName = _T( "Datas\\" );
	LPCTSTR szDbPath = BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("pump.db") );
	//LPCTSTR szDbPath = _T("D:\\test\\git\\GDES\\x64\\Debug\\Datas\\pump.db");
	std::vector<TypeTable> msg;
	if(!GetPumpMessage(sql,szDbPath,msg))
	{
		AfxMessageBox(_T("数据库打开失败!"));
		return;
	}
	for(int i = 0; i < msg.size(); i++)
	{
		TypeTable tt = msg[i];
		acutPrintf(_T("\n第%d组数据ID:%d"),i+1,tt.id);
	}
}