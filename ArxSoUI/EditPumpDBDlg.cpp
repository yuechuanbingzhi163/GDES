#include "stdafx.h"
using namespace SOUI;

#include "MainDlg.h"
#include "PathBuild.h"
#include "SListCtrl2.h"

#include <controls.extend/FileHelper.h>
#include <controls.extend/SChatEdit.h>
#include <controls.extend/gif/SGifPlayer.h>
#include <controls.extend/reole/richeditole.h>

static BOOL IsNum(const CString &str)
{
	int n=str.GetLength();
	for(int i=0;i<n;i++)
		if ((str[i]<'0'||str[i]>'9') && str[i] != '.') 
			return FALSE;
	return TRUE;
}

static BOOL IsInt(const CString &str)
{
	int n=str.GetLength();
	for(int i=0;i<n;i++)
		if (str[i]<'0'||str[i]>'9') 
			return FALSE;
	return TRUE;
}

static bool DeletePumpFromDB(CString& ttsql,CString& ptsql)
{
	CString dataDirName = _T( "Datas\\" );
	CString szDbPath = BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("pump.db") );
	if(!DeletePumpFronTable(szDbPath,ttsql)) return false;
	if(!DeletePumpFronTable(szDbPath,ptsql)) return false;
	return true;
}

static void DealStringType(CString& ttsql,CString& msg, const CString& contents,const CString& condition)
{
	ttsql.Append(condition);
	if(contents.IsEmpty())
	{
		msg = _T("条件为空!");
		return;
	}
	if(contents.Find(_T("<")) != -1 || contents.Find(_T(">")) != -1 || contents.Find(_T("=")) != -1) 
		msg = _T("类型中包含非法符号【>】【<】【=】");
	ttsql.Append(_T(" = \'"));
	ttsql.Append(contents);
	ttsql.Append(_T("\'"));
}

static void DealOtherType(CString& ttsql,CString& msg,const CString& editString,const CString& condition)
{
	ttsql.Append(condition);
	if(editString.IsEmpty())
	{
		msg = _T("条件为空!");
		return;
	}
	if(editString.Find(_T("<")) == -1 && editString.Find(_T(">")) == -1 && editString.Find(_T("=")) == -1)
		ttsql.Append(_T(" = "));
	ttsql.Append(editString);
}

static void DealProperty(CString& ptsql,CString& msg,const CString& editString,const CString& condition)
{
	ptsql.Append(condition);
	if(editString.IsEmpty())
	{
		msg = _T("条件为空!");
		return;
	}
	if(editString.Find(_T("<")) == -1 && editString.Find(_T(">")) == -1 && editString.Find(_T("=")) == -1)
		ptsql.Append(_T(" = "));
	ptsql.Append(editString);

}

static void PrintPropertyTable( const PropertyTable& pt)
{
	//acutPrintf(_T("\n***************************"));
	acutPrintf(_T("\n**********属性表**********"));
	acutPrintf(_T("\nID:%d,转速:%d,电机功率:%.1f,最大气量:%.2f,极限压力:%d,吸入绝压:%d"),
		pt.id,pt.speed,pt.power,pt.maxQ,pt.maxP,pt.absP);
	acutPrintf(_T("\n"));
}

static void PrintTypeTable( const TypeTable& tt)
{
	//acutPrintf(_T("\n***************************"));
	acutPrintf(_T("\n**********类型表**********"));
	acutPrintf(_T("\nID:%d,类型%s,最低吸入绝压:%d,泵重:%d,长:%d,宽:%d,高:%d,厂家:%s"),
		tt.id,tt.type,tt.absP,tt.weight,tt.length,tt.weidth,tt.heigth,tt.factName);
	acutPrintf(_T("\n"));
}

static void GetTypeTableDatas(const CString& sql,TypeTableVector& ttV)
{
	CString dataDirName = _T( "Datas\\" );
	CString szDbPath = BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("pump.db") );
	GetPumpTypeTable(sql,szDbPath,ttV);
}

static void GetPropertyTableDatas(const CString& sql,PropertyTableVector& ptV)
{
	CString dataDirName = _T( "Datas\\" );
	CString szDbPath = BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("pump.db") );
	GetPumpPropertyTable(sql,szDbPath,ptV);
}

static void FindTypeFirst(const CString& ttsql,DBDatasVector& datasV)
{
	TypeTableVector ttV;
	GetTypeTableDatas(ttsql,ttV);
	for(int i = 0; i < ttV.size(); i++)
	{
		TypeTable tt = ttV[i];
		DBDatas datas;
		datas.CopyFromType(tt);
		CString strID;
		strID.Format(_T("%d"),tt.id);
		CString ptsql = _T("select * from PropertyTable where catagory_id = ") + strID;
		PropertyTableVector ptV;
		GetPropertyTableDatas(ptsql,ptV);
		for(int j = 0; j < ptV.size(); j++)
		{
			datas.CopyFromProperty(ptV[j]);
			datasV.push_back(datas);
		}
	}
}

static void FindPropertyFirst(const CString& ptsql,DBDatasVector& datasV)
{
	PropertyTableVector ptV;
	GetPropertyTableDatas(ptsql,ptV);
	for(int i = 0; i < ptV.size(); i++)
	{
		PropertyTable pt = ptV[i];
		DBDatas datas;
		datas.CopyFromProperty(pt);
		CString strID;
		strID.Format(_T("%d"),pt.id);
		CString ttsql = _T("select * from TypeTable where catagory_id = ") + strID;
		TypeTableVector ttV;
		GetTypeTableDatas(ttsql,ttV);
		for(int j = 0; j < ttV.size(); j++)
		{
			datas.CopyFromType(ttV[j]);
			datasV.push_back(datas);
		}
	}
}

static void FindTypeProperty(const CString& ttsql,const CString& ptsql,DBDatasVector& datasV)
{
	PropertyTableVector ptV;
	GetPropertyTableDatas(ptsql,ptV);
	TypeTableVector ttV;
	GetTypeTableDatas(ttsql,ttV);
	for(int i = 0; i < ttV.size(); i++)
	{
		DBDatas datas;
		datas.CopyFromType(ttV[i]);
		for(int j = 0; j < ptV.size(); j++)
		{
			PropertyTable pt = ptV[j];
			if(pt.id != ttV[i].id) continue;
			datas.CopyFromProperty(pt);
			datasV.push_back(datas);
		}
	}
}

static void FindAllPumps(DBDatasVector& datasV)
{
	CString ttsql;
	ttsql = _T("select * from TypeTable");
	FindTypeFirst(ttsql,datasV);
}

void CMainDlg::InitPumpCheckBox()
{
	pTypeCheck = FindChildByName2<SCheckBox>(L"typeCheckBox");
	pFactoryCheck = FindChildByName2<SCheckBox>(L"factoryCheckBox");
	pSpeedCheck = FindChildByName2<SCheckBox>(L"speedCheckBox");
	pPowerCheck = FindChildByName2<SCheckBox>(L"powerCheckBox");
	pLengthCheck = FindChildByName2<SCheckBox>(L"lengthCheckBox");
	pWidthCheck = FindChildByName2<SCheckBox>(L"widthCheckBox");
	pHeightCheck = FindChildByName2<SCheckBox>(L"heightCheckBox");
	pWeightCheck = FindChildByName2<SCheckBox>(L"weightCheckBox");
	pAbsPCheck = FindChildByName2<SCheckBox>(L"absPCheckBox");
	pMaxPCheck = FindChildByName2<SCheckBox>(L"maxPCheckBox");
	pMinPCheck = FindChildByName2<SCheckBox>(L"minPCheckBox");
	pMaxQCheck = FindChildByName2<SCheckBox>(L"maxQCheckBox");
}

bool CMainDlg::EditsHasEmpty()
{
	LPCTSTR names[] = {
		L"typeEdit",
		L"factoryEdit",
		L"weightEdit", 
		L"speedEdit",
		L"powerEdit",
		L"lengthEdit",
		L"widthEdit",
		L"heightEdit",
		L"absPEdit",		
		L"maxPEdit",
		L"minPEdit"
		L"maxQEdit"
	}; 

	LPCTSTR errorMsgs[] = {
		_T("型号为空!"),
		_T("生产厂家为空!"),
		_T("泵重为空!"),
		_T("转速为空!"),
		_T("电机功率为空!"),
		_T("长度为空!"),
		_T("宽度为空!"),
		_T("高度为空!"),
		_T("吸入绝压为空!"),
		_T("极限压力为空!"),
		_T("最低吸入绝压为空!"),
		_T("最大气量为空!")
	};

	LPCTSTR errorMsgs2[] = {
		_T("型号非法!"),
		_T("生产厂家非法!"),
		_T("泵重非法[请输入整数]!"),
		_T("转速非法[请输入整数]!"),
		_T("电机功率非法[请输入整数]!"),
		_T("长度非法[请输入整数]!"),
		_T("宽度非法[请输入整数]!"),
		_T("高度非法[请输入整数]!"),
		_T("吸入绝压非法[请输入整数]!"),
		_T("极限压力非法[请输入整数]!"),
		_T("最低吸入绝压非法[请输入整数]!"),
		_T("最大气量非法[请输入整数]!")
	};

	const int typeFlags[] = {
		-1,
		-1,
		0,
		0,
		1,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		1
	};

	bool ret = true;
	int n = sizeof(names)/sizeof(names[0]);
	for(int i=0;i<n;i++)
	{
		SWindow *pWnd = FindChildByName(names[i]);
		if(pWnd == 0) continue;

		if(pWnd->GetWindowText().IsEmpty())
		{
			AfxMessageBox(errorMsgs[i]);
			ret = false; break;
		}
	}
	if(!ret) return false;

	for(int i=0;i<n;i++)
	{
		SWindow *pWnd = FindChildByName(names[i]);
		CString str = pWnd->GetWindowText();
		if(typeFlags[i] == -1)
		{
			continue;
		}
		else if(typeFlags[i] == 0)
		{
			ret = IsInt(str);
		}
		else
		{
			ret = IsNum(str);
		}

		if(!ret)
		{
			AfxMessageBox(errorMsgs2[i]);
			break;
		}
	}
	return ret;
}

bool CMainDlg::GetEditContents(TypeTable& tt,PropertyTable& pt)
{
	if(EditsHasEmpty()) return false;

	tt.absP = _ttoi((LPCTSTR)FindChildByName(L"absPEdit")->GetWindowText());
	tt.factName = (LPCTSTR)FindChildByName(L"factoryEdit")->GetWindowText();
	tt.heigth = _ttoi((LPCTSTR)FindChildByName(L"heightEdit")->GetWindowText());
	tt.length = _ttoi((LPCTSTR)FindChildByName(L"lengthEdit")->GetWindowText());
	tt.type = (LPCTSTR)FindChildByName(L"typeEdit")->GetWindowText();
	tt.weidth = _ttoi((LPCTSTR)FindChildByName(L"widthEdit")->GetWindowText());
	tt.weight = _ttoi((LPCTSTR)FindChildByName(L"weightEdit")->GetWindowText());

	pt.speed = _ttoi((LPCTSTR)FindChildByName(L"speedEdit")->GetWindowText());
	pt.power = _tstof((LPCTSTR)FindChildByName(L"powerEdit")->GetWindowText());
	pt.absP = _ttoi((LPCTSTR)FindChildByName(L"absPEdit")->GetWindowText());
	pt.maxP = _ttoi((LPCTSTR)FindChildByName(L"maxPEdit")->GetWindowText());
	pt.maxQ = _tstof((LPCTSTR)FindChildByName(L"maxQEdit")->GetWindowText());
	return true;
}

void CMainDlg::OnBtnFindPump()
{
	////查找泵列表控件
	//SListCtrl2 *pumpList = FindChildByName2<SListCtrl2>(L"pumpDbList");
	//if(pumpList)
	//{
	//	SMessageBox(NULL,_T("this is a message box"),_T("haha"),MB_OK|MB_ICONEXCLAMATION);
	//}
	////查找"型号"编辑框
	//SEdit *pType = FindChildByName2<SEdit>(L"typeEdit");
	//if(pType)
	//{
	//	AfxMessageBox((LPCTSTR)pType->GetWindowText());
	//}

	DBDatasVector datasV;
	//查看所有瓦斯泵
	if(GetCheckBoxNum() == 0)
	{
		FindAllPumps(datasV);
	}
	else if(!FindPumpsByCondition(datasV))
	{
		datasV.clear();
	}
	UpdateList(datasV);
}

void CMainDlg::OnBtnUpdatePump()
{
	CString dataDirName = _T( "Datas\\" );
	CString szDbPath = BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("pump.db") );

	TypeTable tt;
	PropertyTable pt;
	if(!GetEditContents(tt,pt)) return;
	int newId;
	InsertPumpToTypeTable(szDbPath,tt,newId);
	pt.id = newId;

	if(!InsertPumpToPropertyTable(szDbPath,pt))
	{
		AfxMessageBox(_T("数据插入失败!"),MB_OK|MB_ICONSTOP);
		return;
	}
	AfxMessageBox(_T("数据插入成功!"));

	//更新列表控件
	pTypeCheck->SetCheck(TRUE);
	pWeightCheck->SetCheck(TRUE);
	pSpeedCheck->SetCheck(TRUE);
	pMaxPCheck->SetCheck(TRUE);
	pFactoryCheck->SetCheck(TRUE);
	pLengthCheck->SetCheck(TRUE);
	pHeightCheck->SetCheck(TRUE);
	pWidthCheck->SetCheck(TRUE);
	pMinPCheck->SetCheck(TRUE);
	pAbsPCheck->SetCheck(TRUE);
	pPowerCheck->SetCheck(TRUE);
	pMaxPCheck->SetCheck(TRUE);

	DBDatasVector datasV;
	if(!FindPumpsByCondition(datasV)) datasV.clear();
	UpdateList(datasV);
}

void CMainDlg::UpdateList( const DBDatasVector& datasV )
{
	SListCtrl2 *pList=FindChildByName2<SListCtrl2>(L"pumpDbList");
	pList->DeleteAllItems();

	for (int i = 0; i < datasV.size(); i++ )
	{
		DBDatas datas = datasV[i];
		int iItem=pList->InsertItem(i,datas.type);
		//TCHAR szBuf[10];
		//_stprintf(szBuf,_T("%d"),pst->score);
		//pList->SetSubItemText(iItem,1,szBuf);

		pList->SetSubItemText(iItem, 1, datas.weight);  
		pList->SetSubItemText(iItem, 2, datas.length);  
		pList->SetSubItemText(iItem, 3, datas.weidth);  
		pList->SetSubItemText(iItem, 4, datas.height);
		pList->SetSubItemText(iItem, 5, datas.speed);  
		pList->SetSubItemText(iItem, 6, datas.absp);  
		pList->SetSubItemText(iItem, 7, datas.maxp);  
		pList->SetSubItemText(iItem, 8, datas.maxq);  
		pList->SetSubItemText(iItem, 9, datas.power);  
		pList->SetSubItemText(iItem, 10, datas.minabsp);  
		pList->SetSubItemText(iItem, 11, datas.factory);
	}
}

int CMainDlg::CheckBoxTable()
{
	if((pTypeCheck->IsChecked() || pMinPCheck->IsChecked() || pWidthCheck->IsChecked() || 
		pLengthCheck->IsChecked() || pWidthCheck->IsChecked() || pHeightCheck->IsChecked() ||
		pFactoryCheck->IsChecked())	&& (!pSpeedCheck->IsChecked() && !pPowerCheck->IsChecked() && 
		!pMaxQCheck->IsChecked() && !pMaxPCheck->IsChecked() && !pAbsPCheck->IsChecked())) 
		return 1;

	if((!pTypeCheck->IsChecked() && !pMinPCheck->IsChecked() && !pWidthCheck->IsChecked() && 
		!pLengthCheck->IsChecked() && !pWeightCheck->IsChecked() && !pHeightCheck->IsChecked() && 
		!pFactoryCheck->IsChecked()) && (pSpeedCheck->IsChecked() || pPowerCheck->IsChecked() || 
		pMaxQCheck->IsChecked() || pMaxPCheck->IsChecked() || pAbsPCheck->IsChecked())) 
		return 2;

	if((pTypeCheck->IsChecked() || pMinPCheck->IsChecked() || pWidthCheck->IsChecked() || 
		pLengthCheck->IsChecked() || pWidthCheck->IsChecked() || pHeightCheck->IsChecked() || 
		pFactoryCheck->IsChecked())	&& (pSpeedCheck->IsChecked() || pPowerCheck->IsChecked() ||
		pMaxQCheck->IsChecked() || pMaxPCheck->IsChecked() || pAbsPCheck->IsChecked()))
		return 3;

	return 0;
}

bool CMainDlg::FindPumpsByCondition( DBDatasVector& datasV )
{
	int indx = CheckBoxTable();
	CString msg;
	CString ttsql = _T("select * from TypeTable where ");
	CString ptsql = _T("select * from PropertyTable where ");
	switch(indx)
	{
	case 0:
		msg = _T("一个都没有选中!");
		break;
	case 1:
		OnlyTypesql(ttsql,msg);
		FindTypeFirst(ttsql,datasV);
		break;
	case 2:
		OnlyPropertysql(ptsql,msg);
		FindPropertyFirst(ptsql,datasV);
		break;
	case 3:
		OnlyTypesql(ttsql,msg);
		OnlyPropertysql(ptsql,msg);
		FindTypeProperty(ttsql,ptsql,datasV);
		break;
	default:
		msg = _T("选择错误!");
		break;
	}
	if(!msg.IsEmpty()) 
	{
		AfxMessageBox(msg);
		return false;
	}
	//acutPrintf(_T("\n%s"),msg);
	return true;
}

bool CMainDlg::DeletePump( const DBDatas& datas )
{
	CString ttsql = _T("delete from TypeTable where type = \'") + datas.type + _T("\' and absP = ")
		+ datas.minabsp + _T(" and weight = ") + datas.weight + _T(" and length = ")
		+ datas.length + _T(" and weidth = ") + datas.weidth + _T(" and heigth = ") + datas.height
		+ _T(" and factoryName = \'") + datas.factory + _T("\'");
	CString ptsql = _T("delete from PropertyTable where speed = ") + datas.speed + _T(" and power = ")
		+ datas.power + _T(" and maxQ = ") + datas.maxq + _T(" and maxP = ") + datas.maxp + _T(" and absP = ")
		+ datas.absp;
	bool ret = DeletePumpFromDB(ttsql,ptsql);
	return ret;
}

void CMainDlg::OnlyTypesql(CString& ttsql,CString& msg)
{
	int checkNum = GetCheckBoxNum();
	int currentNum = 0;
	if(pTypeCheck->IsChecked())
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and type ");
		}
		else
		{
			condition = _T(" type ");
		}
		DealStringType(ttsql,msg,(LPCTSTR)FindChildByName(L"typeEdit")->GetWindowText(),condition);
		currentNum += 1;
	}

	if(pMinPCheck->IsChecked())
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and absP ");
		}
		else
		{
			condition = _T(" absP ");
		}

		DealOtherType(ttsql,msg,(LPCTSTR)FindChildByName(L"absPEdit")->GetWindowText(),condition);
		currentNum += 1;
	}

	if(pWeightCheck->IsChecked())
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and weight ");
		}
		else
		{
			condition = _T(" weight ");
		}

		DealOtherType(ttsql,msg,(LPCTSTR)FindChildByName(L"weightEdit")->GetWindowText(),condition);
		currentNum += 1;
	}

	if(pLengthCheck->IsChecked())
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and length ");
		}
		else
		{
			condition = _T(" length ");
		}

		DealOtherType(ttsql,msg,(LPCTSTR)FindChildByName(L"lengthEdit")->GetWindowText(),condition);
		currentNum += 1;
	}

	if(pWidthCheck->IsChecked())
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and weidth ");
		}
		else
		{
			condition = _T(" weidth ");
		}

		DealOtherType(ttsql,msg,(LPCTSTR)FindChildByName(L"widthEdit")->GetWindowText(),condition);
		currentNum += 1;
	}

	if(pHeightCheck->IsChecked())
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and heigth ");
		}
		else
		{
			condition = _T(" heigth ");
		}

		DealOtherType(ttsql,msg,(LPCTSTR)FindChildByName(L"heightEdit")->GetWindowText(),condition);
		currentNum += 1;
	}

	if(pFactoryCheck->IsChecked())
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and factoryName ");
		}
		else
		{
			condition = _T(" factoryName ");
		}

		DealStringType(ttsql,msg,(LPCTSTR)FindChildByName(L"factoryEdit")->GetWindowText(),condition);
		currentNum += 1;
	}
}

void CMainDlg::OnlyPropertysql( CString& ptsql,CString& msg )
{
	int checkNum = GetCheckBoxNum();
	int currentNum = 0;
	if(pSpeedCheck->IsChecked())
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and speed ");
		}
		else
		{
			condition = _T(" speed ");
		}
		DealProperty(ptsql,msg,(LPCTSTR)FindChildByName(L"speedEdit")->GetWindowText(),condition);
		currentNum += 1;
	}

	if(pPowerCheck->IsChecked())
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and power ");
		}
		else
		{
			condition = _T(" power ");
		}

		DealProperty(ptsql,msg,(LPCTSTR)FindChildByName(L"powerEdit")->GetWindowText(),condition);
		currentNum += 1;
	}

	if(pMaxQCheck->IsChecked())
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and maxQ ");
		}
		else
		{
			condition = _T(" maxQ ");
		}

		DealProperty(ptsql,msg,(LPCTSTR)FindChildByName(L"maxQEdit")->GetWindowText(),condition);
		currentNum += 1;
	}

	if(pMaxPCheck->IsChecked())
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and maxP ");
		}
		else
		{
			condition = _T(" maxP ");
		}

		DealProperty(ptsql,msg,(LPCTSTR)FindChildByName(L"maxPEdit")->GetWindowText(),condition);
		currentNum += 1;
	}

	if(pAbsPCheck->IsChecked())
	{
		CString condition;
		if(currentNum >= 1 && checkNum > 1)
		{
			condition = _T(" and absP ");
		}
		else
		{
			condition = _T(" absP ");
		}

		DealProperty(ptsql,msg,(LPCTSTR)FindChildByName(L"absPEdit")->GetWindowText(),condition);
		currentNum += 1;
	}
}

int CMainDlg::GetCheckBoxNum()
{
	int ret = 0;
	if(pTypeCheck->IsChecked()) ret += 1;
	if(pWidthCheck->IsChecked()) ret += 1;
	if(pSpeedCheck->IsChecked()) ret += 1;
	if(pMaxPCheck->IsChecked()) ret += 1;
	if(pFactoryCheck->IsChecked()) ret += 1;
	if(pLengthCheck->IsChecked()) ret += 1;
	if(pHeightCheck->IsChecked()) ret += 1;
	if(pWidthCheck->IsChecked()) ret += 1;
	if(pMinPCheck->IsChecked()) ret += 1;
	if(pAbsPCheck->IsChecked()) ret += 1;
	if(pPowerCheck->IsChecked()) ret += 1;
	if(pMaxQCheck->IsChecked()) ret += 1;
	return ret;
}

void CMainDlg::OnListCtrlDblClickEvent(SOUI::EventArgs *pEvt)
{
	EventLCDblClick *pEvtLCDblClick = (EventLCDblClick*)pEvt;
	int nCurSel = pEvtLCDblClick->nCurSel;
	SListCtrl2 *pList = (SListCtrl2*)pEvt->sender;
	if(nCurSel < 0 || nCurSel >=pList->GetColumnCount()) return;

	FindChildByName(L"typeEdit")->SetWindowText(pList->GetSubItemText(nCurSel, 0));
	FindChildByName(L"factoryEdit")->SetWindowText(pList->GetSubItemText(nCurSel, 1));
	FindChildByName(L"speedEdit")->SetWindowText(pList->GetSubItemText(nCurSel, 2));
	FindChildByName(L"powerEdit")->SetWindowText(pList->GetSubItemText(nCurSel, 3));
	FindChildByName(L"lengthEdit")->SetWindowText(pList->GetSubItemText(nCurSel, 4));
	FindChildByName(L"widthEdit")->SetWindowText(pList->GetSubItemText(nCurSel, 5));
	FindChildByName(L"heightEdit")->SetWindowText(pList->GetSubItemText(nCurSel, 6));
	FindChildByName(L"weightEdit")->SetWindowText(pList->GetSubItemText(nCurSel, 7));
	FindChildByName(L"absPEdit")->SetWindowText(pList->GetSubItemText(nCurSel, 8));
	FindChildByName(L"maxPEdit")->SetWindowText(pList->GetSubItemText(nCurSel, 9));
	FindChildByName(L"minPEdit")->SetWindowText(pList->GetSubItemText(nCurSel, 10));
	FindChildByName(L"maxQEdit")->SetWindowText(pList->GetSubItemText(nCurSel, 11));
}

void CMainDlg::OnBtnDelPump()
{
	SListCtrl2 *pList=FindChildByName2<SListCtrl2>(L"pumpDbList");
	pList->DeleteItem(pList->GetSelectedItem());
}