// GasPumpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GasPumpDlg.h"
#include "CGridColumnTraitDateTime.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridColumnTraitHyperLink.h"
#include "CGridRowTraitXP.h"
#include "ViewConfigSection.h"
#include <sqliteHelper/SqliteHelper.h>

IMPLEMENT_DYNAMIC(GasPumpDlg, GasBaseAcesDlg)

GasPumpDlg::GasPumpDlg(CWnd* pParent /*=NULL*/)
	: GasBaseAcesDlg(GasPumpDlg::IDD, pParent)
{

}

GasPumpDlg::~GasPumpDlg()
{
}

void GasPumpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAS_PUMP_LIST, m_gasPumpCListPump);
}


BEGIN_MESSAGE_MAP(GasPumpDlg, CDialog)
	ON_BN_CLICKED(IDOK, &GasPumpDlg::OnBnClickedOk)
END_MESSAGE_MAP()


static BOOL ShowRetDlg()
{
	ResultDlg retDlg;
	AcStringArray datas;
	datas.append(_T("泵站的装机能力和管网能力满足瓦斯抽采达标的要求"));
	datas.append(_T("部分备用泵的抽采能力小于运行单泵的抽采能力，不能满足瓦斯抽采达标的要求"));
	datas.append(_T("泵站装机能力和管网能力不足，不能满足瓦斯抽采达标的要求"));
	datas.append(_T("其他"));
	retDlg.setItemDatas(datas);

	IntArray trueIndxs;
	trueIndxs.push_back(0);
	retDlg.setTrueIndex(trueIndxs,RET_GASPUMP_OBJCT_NAME,DESC_GASPUMP_OBJCT_NAME);

	AcStringArray bookMks;
	bookMks.append(_T("GasPump_Ret"));
	bookMks.append(_T("GasPump_Method"));
	bookMks.append(_T("GasPump_Conclusion"));
	bookMks.append(_T("GasPump_YesOrNo"));
	retDlg.setBookMarks(bookMks);

	if(IDOK == retDlg.DoModal()) 
	{
		return TRUE;
	}
	else return FALSE;
}

static void SetItems(vector<GasPumpDataRecord>& records,GasPumpDataModel& dataModel,CGridListCtrlGroups& listCtrl,int num)
{
	if(!records.empty())
	{
		dataModel.SetRecords(records);
	}
	dataModel.SetRowCount(num);
	listCtrl.DeleteAllItems();
	int nItem = 0;
	for(size_t rowId = 0; rowId < dataModel.GetRowIds() ; ++rowId)
	{
		listCtrl.InsertItem(nItem, dataModel.GetCellText(rowId,0));
		//acutPrintf(_T("\n新行:%d"),nItem);
		listCtrl.SetItemData(nItem, rowId);
		for(int col = 0; col < dataModel.GetColCount() ; ++col)
		{
			int nCellCol = col+1;	// +1 because of hidden column
			const CString& strCellText = dataModel.GetCellText(rowId, col);
			//acutPrintf(_T("\n数据:%s"),strCellText);
			listCtrl.SetItemText(nItem, nCellCol, strCellText);
		}
		nItem++;
	}	

}

static void GetDatasFromList(const CGridListCtrlGroups& listCtrl,ArrayVector& datas)
{
	AcStringArray strName, strState, strType, strAbility;
	for(size_t i = 0; i < listCtrl.GetItemCount(); i++)
	{
		CString	name = listCtrl.GetItemText(i,1);
		if (name.IsEmpty())
		{
			name = ISNULL;
		}
		strName.append(name);

		CString state = listCtrl.GetItemText(i,2);
		if (state.IsEmpty())
		{
			state = ISNULL;
		}
		strState.append(state);

		CString type = listCtrl.GetItemText(i,3);
		if (type.IsEmpty())
		{
			type = ISNULL;
		}
		strType.append(type);

		CString ability = listCtrl.GetItemText(i,4);
		if (ability.IsEmpty())
		{
			ability = ISNULL;
		}
		strAbility.append(ability);
	}
	datas.push_back(strName);
	datas.push_back(strState);
	datas.push_back(strType);
	datas.push_back(strAbility);
}

void GasPumpDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	AcStringArray strName,strState,strType,strAbility;
	ArrayVector datas;
	GetDatasFromList(m_gasPumpCListPump,datas);
	if(!datas.empty())
	{
		ReportDataHelper::WriteDatas(GAS_PUMP_OBJCT_NAME,datas);
	}
	if(!ShowRetDlg()) return;
	OnOK();
}

BOOL GasPumpDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();
	InitListCtrl();
	int itemNum;
	if(readDataFromFiles(m_records,itemNum))
	{
		SetItems(m_records,m_dataModel,m_gasPumpCListPump,itemNum);
	}
	UpdateData(FALSE);
	return TRUE;
}
static void GetPumpTypes(const CString& szDbPath,AcStringArray& types)
{
	TypeTableVector ttV;
	CString sql = _T("select * from TypeTable");
	GetPumpTypeTable(sql,szDbPath,ttV);
	for(int i = 0; i < ttV.size(); i++)
	{
		TypeTable tt = ttV[i];
		types.append(tt.type);
	}
}

void GasPumpDlg::InitListCtrl()
{
	m_gasPumpCListPump.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;

	CRect rect;  
	m_gasPumpCListPump.GetClientRect(rect); //获得当前客户区信息  
	
	m_gasPumpCListPump.InsertHiddenLabelColumn();	// Requires one never uses column 0
	int colCount = m_dataModel.GetColCount();
	for(int col = 0; col <  colCount; ++col)
	{
		const CString& title = m_dataModel.GetColTitle(col);
		CGridColumnTrait* pTrait = NULL;
		if(0 != col && 1 != col)
		{
			pTrait = new CGridColumnTraitEdit;
		}
		if ( 2 == col)
		{
			CGridColumnTraitCombo* pComboTrait = new CGridColumnTraitCombo;
			CString dataDirName = _T( "Datas\\" );
			CString szDbPath = BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("pump.db") );
			AcStringArray types;
			GetPumpTypes(szDbPath,types);
			for(int i = 0; i < types.length(); i++)
			{
				pComboTrait->AddItem((DWORD_PTR)i,types[i].kACharPtr());
			}
			pTrait = pComboTrait;
		}

		if( 3 == col )
		{
			m_gasPumpCListPump.InsertColumnTrait(col+1, title, LVCFMT_LEFT, 2*rect.Width()/5, col, pTrait);

		}
		else
		{
			m_gasPumpCListPump.InsertColumnTrait(col+1, title, LVCFMT_LEFT, rect.Width()/5, col, pTrait);
		}
	}
}

BOOL GasPumpDlg::readPumpDatas( ArrayVector& datavector)
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

	datavector.push_back(sysNames);
	datavector.push_back(pumpStates);
	datavector.push_back(pumpTypes);
	datavector.push_back(pumpAbilities);

	return TRUE;
}

static void RemoveExtraItems(ArrayVector& orinDatas,const AcStringArray& sysdlgNames,int pNum)
{
	if(orinDatas.empty()) return;
	AcStringArray pumpdlgNamesTmp = orinDatas[0];
	AcStringArray pumpStateArrayTmp = orinDatas[1];
	AcStringArray pumpTypeArrayTmp = orinDatas[2];
	AcStringArray pumpAbilityArrayTmp = orinDatas[3];

	for (int i = 0; i < pNum; i++)
	{
		if(!sysdlgNames.contains(pumpdlgNamesTmp[i]))
		{
			orinDatas[0].remove(pumpdlgNamesTmp[i]);
			orinDatas[1].remove(pumpStateArrayTmp[i]);
			orinDatas[2].remove(pumpTypeArrayTmp[i]);
			orinDatas[3].remove(pumpAbilityArrayTmp[i]);
		}
	}

}

static void AppenOrinDatas(ArrayVector& newDatas,const ArrayVector& orinDatas,int pNum)
{
	newDatas.resize(4);
	if(orinDatas.empty()) return;
	pNum = orinDatas[0].length();
	for (int i = 0; i < pNum; i++)
	{
		newDatas[0].append(orinDatas[0][i]);
		newDatas[1].append(orinDatas[1][i]);
		newDatas[2].append(orinDatas[2][i]);
		newDatas[3].append(orinDatas[3][i]);
	}

}

static void AddNewDatas(ArrayVector& newDatas,const AcString& sysdlgName)
{
	if(!newDatas[0].contains(sysdlgName))
	{
		for(int j = 0; j < 2; j++)
		{
			newDatas[0].append(sysdlgName);
			if(j%2 == 0)
			{
				newDatas[1].append(_T("运行泵"));
			}
			else
			{
				newDatas[1].append(_T("备用泵"));
			}
			newDatas[2].append(_T(""));
			newDatas[3].append(_T(""));
		}
	}

}

BOOL GasPumpDlg::readDataFromFiles( vector<GasPumpDataRecord>& records,int& num )
{
	//从瓦斯泵站能力对话框中保存的数据
	ArrayVector pumpVector;
	AcStringArray sysdlgNames;
	bool retp = readPumpDatas(pumpVector);
	//从系统对话框中获取数据
	bool rets = readDataFromSysDlg(sysdlgNames);
	if((!retp) && (!rets)) return FALSE;
	AcStringArray pumpdlgNames, pumpStateArray, pumpTypeArray,pumpAbilityArray;
	if(!pumpVector.empty())
	{
		pumpdlgNames = pumpVector[0];
		pumpStateArray = pumpVector[1];
		pumpTypeArray = pumpVector[2];
		pumpAbilityArray = pumpVector[3];
	}

	int sNum = sysdlgNames.length();
	int pNum = pumpdlgNames.length();
	
	ArrayVector datas;
	for(int i = 0; i < pumpVector.size(); i++)
	{
		datas.push_back(pumpVector[i]);
	}

	//排除多余项
	RemoveExtraItems(datas,sysdlgNames,pNum);

	ArrayVector newDatas;
	AppenOrinDatas(newDatas,datas,pNum);
	for (int i = 0; i < sNum; i++)
	{
		AddNewDatas(newDatas,sysdlgNames[i]);
	}
	num = newDatas[0].length();

	for(int i = 0; i < num; i++)
	{
		records.push_back(GasPumpDataRecord(newDatas[0][i].kACharPtr(),newDatas[1][i].kACharPtr(),newDatas[2][i].kACharPtr(),newDatas[3][i].kACharPtr()));
	}
	return TRUE;
}

//BOOL GasPumpDlg::WriteDataToFile(const CString& txtName,const ArrayVector& datas)
//{
//	CString dataDirName = _T( "Datas\\BaseTemp\\" );
//	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),txtName );
//
//	AcOfstream outFile(fileName);
//	if (!outFile) return FALSE;
//
//	for(int i = 0; i < datas[0].length(); i++)
//	{
//		CString value;
//		outFile << datas[0][i].kACharPtr() << _T("\t") << datas[1][i].kACharPtr()
//			<< _T("\t") << datas[2][i].kACharPtr() << _T("\t") << datas[3][i].kACharPtr()
//			<< _T("\n");
//	}
//	outFile.close();
//	return TRUE;
//}

BOOL GasPumpDlg::readDataFromSysDlg( AcStringArray& sysNames )
{
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(RUNNING_SYS_OBJCT_NAME,datasVector,1);
	for(int i = 0; i < datasVector.size(); i++)
	{
		CString strName;
		strName.Format(_T("%s"),datasVector[i][0].kACharPtr());
		if(ISNULL == strName) strName = _T("");
		sysNames.append(strName);
	}
	if(sysNames.isEmpty()) return FALSE;
	return TRUE;

}