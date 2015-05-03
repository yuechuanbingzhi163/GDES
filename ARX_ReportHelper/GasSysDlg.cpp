#include "stdafx.h"
#include "GasSysDlg.h"

#include "CGridColumnTraitDateTime.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridColumnTraitHyperLink.h"
#include "CGridRowTraitXP.h"
#include "ViewConfigSection.h"

IMPLEMENT_DYNAMIC(GasSysDlg, GasBaseAcesDlg)

GasSysDlg::GasSysDlg(CWnd* pParent /*=NULL*/)
	: GasBaseAcesDlg(GasSysDlg::IDD, pParent)
	, m_numPermSys(_T(""))
	, m_numTempSys(_T(""))
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDI_GASSYS_ICON);
}

GasSysDlg::~GasSysDlg()
{
}

void GasSysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, ID_PERMSYS_NUM_EDIT, m_numPermSys);
	DDX_Control(pDX, IDC_PERMSYS_INPUT_LIST, m_permCListCtrl);
	DDX_Text(pDX, ID_TEMPESYS_NUM_EDIT, m_numTempSys);
	DDX_Control(pDX, IDC_TEMPSYS_INPUT_LIST, m_tempCListCtrl);
}


BEGIN_MESSAGE_MAP(GasSysDlg, CDialog)
	ON_EN_KILLFOCUS(ID_PERMSYS_NUM_EDIT, &GasSysDlg::OnEnKillfocusPermsysNumEdit)
	ON_EN_SETFOCUS(ID_PERMSYS_NUM_EDIT, &GasSysDlg::OnEnSetfocusPermsysNumEdit)
	ON_BN_CLICKED(IDC_PERM_CLEAR_BUTTON, &GasSysDlg::OnBnClickedPermClearButton)
	ON_NOTIFY(NM_RCLICK, IDC_PERMSYS_INPUT_LIST, &GasSysDlg::OnNMRClickPermsysInputList)
	ON_COMMAND(ID_DELETE_ITEM, &GasSysDlg::OnDeleteItem)
	ON_BN_CLICKED(IDC_TEMP_CLEAR_BUTTON, &GasSysDlg::OnBnClickedTempClearButton)
	ON_BN_CLICKED(IDC_EXIT_BUTTON, &GasSysDlg::OnBnClickedExitButton)
	ON_NOTIFY(NM_RCLICK, IDC_TEMPSYS_INPUT_LIST, &GasSysDlg::OnNMRClickTempsysInputList)
	ON_BN_CLICKED(ID_ENSURE, &GasSysDlg::OnBnClickedEnsure)
	ON_EN_KILLFOCUS(ID_TEMPESYS_NUM_EDIT, &GasSysDlg::OnEnKillfocusTempesysNumEdit)
	ON_EN_SETFOCUS(ID_TEMPESYS_NUM_EDIT, &GasSysDlg::OnEnSetfocusTempesysNumEdit)
END_MESSAGE_MAP()


// GasSysDlg 消息处理程序

static BOOL IsNum(CString &str)
{
	int n=str.GetLength();
	for(int i=0;i<n;i++)
		if (str[i]<'0'||str[i]>'9') 
			return FALSE;
	return TRUE;
}

static void initList(CGridListCtrlGroups& listCtrl,const CListCtrl_DataModel& dataModal)
{
	listCtrl.SetCellMargin(1.2);

	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;
	listCtrl.SetDefaultRowTrait(pRowTrait);
	listCtrl.EnableVisualStyles(true);

	// Create Columns
	listCtrl.InsertHiddenLabelColumn();	// Requires one never uses column 0

	for(int col = 0; col < dataModal.GetColCount() ; ++col)
	{
		const CString& title = dataModal.GetColTitle(col);
		CGridColumnTrait* pTrait = NULL;
		if (col==1)
		{
			CGridColumnTraitCombo* pComboTrait = new CGridColumnTraitCombo;
			pComboTrait->AddItem((DWORD_PTR)0,_T("是"));
			pComboTrait->AddItem((DWORD_PTR)1,_T("否"));
			pTrait = pComboTrait;
		}
		else if (col==2)
		{
			CGridColumnTraitCombo* pComboTrait = new CGridColumnTraitCombo;
			pComboTrait->AddItem((DWORD_PTR)0,_T("正在维修"));
			pComboTrait->AddItem((DWORD_PTR)1,_T("已经撤出"));
			pComboTrait->AddItem((DWORD_PTR)1,_T("正在建设"));
			pTrait = pComboTrait;
		}

		else
		{
			pTrait = new CGridColumnTraitEdit;
		}
		listCtrl.InsertColumnTrait(col+1, title, LVCFMT_LEFT, 100, col, pTrait);
	}


	CViewConfigSectionWinApp* pColumnProfile = new CViewConfigSectionWinApp(_T("Sample List"));
	pColumnProfile->AddProfile(_T("默认"));
	pColumnProfile->AddProfile(_T("自定义"));
	//pColumnProfile->DeleteProfile(_T("Special"));
	listCtrl.SetupColumnConfig(pColumnProfile);
}
static void setRecord(vector<CListCtrl_DataRecord>& records,const ArrayVector& datasVector,CString& num)
{
	for (int i = 0; i < datasVector.size(); i++)
	{
		CString name = datasVector[i][0];
		CString running =  datasVector[i][1];
		CString reason = datasVector[i][2];
		records.push_back(CListCtrl_DataRecord(name,running,reason));
	}
	num.Format(_T("%d"),datasVector.size());

}

//datasVector存储方式
/*
名称|运转|备注
名称|运转|备注
*/
void GasSysDlg::OnInitList()
{
	initList(m_permCListCtrl,m_permDataModel);
	initList(m_tempCListCtrl,m_tempDataModel);
	ArrayVector datasVector;

	//获取地面系统数据
	ReportDataHelper::ReadDatas(PERMENT_OBJCT_NAME,datasVector,3);
	m_numPermSys = _T("");
	m_permRecords.clear();
	if(datasVector.size() > 0) 
	{
		setRecord(m_permRecords,datasVector,m_numPermSys);
	}

	datasVector.clear();
	//获取井下系统数据
	ReportDataHelper::ReadDatas(TEMPGAS_OBJCT_NAME,datasVector,3);
	m_numTempSys = _T("");
	m_tempRecords.clear();
	if(datasVector.size() > 0) 
	{
		setRecord(m_tempRecords,datasVector,m_numTempSys);
	}

	UpdateData(FALSE);
	if(IsNum(m_numPermSys))
	{ 
		SetPermItems(_ttoi(m_numPermSys));
	}
	if(IsNum(m_numTempSys))
	{ 
		SetTempItems(_ttoi(m_numTempSys));
	}

}

void GasSysDlg::OnEnKillfocusPermsysNumEdit()
{
	//int oldNum = _ttoi(m_numPermSys);
	UpdateData(TRUE);
	//if(m_numPermSys.IsEmpty()) return;
	//if(oldNum == _ttoi(m_numPermSys)) return;
	if(IsNum(m_numPermSys))
		SetPermItems(_ttoi(m_numPermSys));
	//m_permRecords = m_permDataModel.GetRecords();
}

static void GetDatasFromList(const CGridListCtrlGroups& listCtrl,AcStringArray& strName,AcStringArray& strIsRunning,AcStringArray& strReason,int num)
{
	for(size_t i = 0; i < num; i++)
	{
		CString	name = listCtrl.GetItemText(i,1);
		if (name.IsEmpty())
		{
			name = ISNULL;
		}
		strName.append(name);
		CString isRunning = listCtrl.GetItemText(i,2);
		if (isRunning.IsEmpty())
		{
			isRunning = ISNULL;
		}
		strIsRunning.append(isRunning);
		CString reason = listCtrl.GetItemText(i,3);
		if (reason.IsEmpty())
		{
			reason = ISNULL;
		}

		strReason.append(reason);
		//acutPrintf(_T("\n第%d行数据:%s\t%s\t%s"),i+1,strName,isRunning,strReason);
	}
}

BOOL GasSysDlg::RecordRunningSysName(const AcStringArray& gasSysNams,const AcStringArray& gasSysRunning)
{
	//CString dataDirName = _T( "Datas\\BaseTemp\\" );
	//CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("bgaspumpvars0.txt") );

	//AcOfstream outFile(fileName);
	//if (!outFile) return FALSE;

	ArrayVector namesVector;
	AcStringArray names;
	for(int i = 0; i < gasSysNams.length(); i++)
	{
		CString isRunning = gasSysRunning[i].kACharPtr();
		if(isRunning != _T("是")) continue;
		names.append(gasSysNams[i]);
		//outFile << gasSysNams[i].kACharPtr() << _T("\n");
	}
	namesVector.push_back(names);
	ReportDataHelper::WriteDatas(RUNNING_SYS_OBJCT_NAME,namesVector);
	if(namesVector.empty()) return FALSE;
	return TRUE;
}

static BOOL ShowRetDlg()
{
	ResultDlg retDlg;
	AcStringArray datas;
	datas.append(_T("目前矿井瓦斯抽采系统正常运转，能保证生产接续"));
	datas.append(_T("建立了部分部分瓦斯抽采系统，无法保证生产接续"));
	datas.append(_T("没有建立地面固定瓦斯抽采系统"));
	datas.append(_T("其他"));
	retDlg.setItemDatas(datas);

	IntArray trueIndxs;
	trueIndxs.push_back(0);
	trueIndxs.push_back(1);
	retDlg.setTrueIndex(trueIndxs,RET_GASSYS_OBJCT_NAME,DESC_GASSYS_OBJCT_NAME);

	AcStringArray bookMks;
	bookMks.append(_T("GasSys_Ret"));
	bookMks.append(_T("GasSys_Method"));
	bookMks.append(_T("GasSys_Conclusion"));
	bookMks.append(_T("GasSys_YesOrNo"));
	retDlg.setBookMarks(bookMks);

	if(IDOK == retDlg.DoModal()) 
	{
		return TRUE;
	}
	else return FALSE;
}

void GasSysDlg::OnBnClickedEnsure()
{
	UpdateData(TRUE);
	AcStringArray strName,strIsRunning,strReason;
	//从列表中获取数据
	GetDatasFromList(m_permCListCtrl,strName,strIsRunning,strReason,_ttoi(m_numPermSys));
	ArrayVector datas;
	datas.push_back(strName);
	datas.push_back(strIsRunning);
	datas.push_back(strReason);
	//把数据写到数据词典中
	ReportDataHelper::WriteDatas(PERMENT_OBJCT_NAME,datas);

	AcStringArray gasSysNams;
	gasSysNams.append(strName);
	AcStringArray gasSysRunning;
	gasSysRunning.append(strIsRunning);

	strName.removeAll();
	strIsRunning.removeAll();
	strReason.removeAll();
	GetDatasFromList(m_tempCListCtrl,strName,strIsRunning,strReason,_ttoi(m_numTempSys));
	datas.clear();
	datas.push_back(strName);
	datas.push_back(strIsRunning);
	datas.push_back(strReason);
	ReportDataHelper::WriteDatas(TEMPGAS_OBJCT_NAME,datas);
	//WriteDataToFile(_T("btvars.txt"),strName,strIsRunning,strReason);
	//for (int i = 0; i < strName.length(); i++)
	//{
		gasSysNams.append(strName);
		gasSysRunning.append(strIsRunning);
	//}
	RecordRunningSysName(gasSysNams,gasSysRunning);

	if(ShowRetDlg())
	{
		OnOK();
	}
}

BOOL GasSysDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();
	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon
	OnInitList();
	return TRUE;
}

static void SetItems(vector<CListCtrl_DataRecord>& records,CListCtrl_DataModel&dataModel,CGridListCtrlGroups& listCtrl,int num)
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
void GasSysDlg::SetPermItems(int num)
{
	if( num <= 0 ) return;
	SetItems(m_permRecords,m_permDataModel,m_permCListCtrl,num);
}

void GasSysDlg::OnEnSetfocusPermsysNumEdit()
{
	UpdateData(TRUE);
	m_permRecords.clear();
	for(size_t i = 0; i < _ttoi(m_numPermSys); i++)
	{
		CString	strName = m_permCListCtrl.GetItemText(i,1);
		CString isRunning = m_permCListCtrl.GetItemText(i,2);
		CString strReason = m_permCListCtrl.GetItemText(i,3);
		m_permRecords.push_back(CListCtrl_DataRecord(strName,isRunning,strReason));
	}
}

void GasSysDlg::OnBnClickedPermClearButton()
{
	//删除所有项
	m_permCListCtrl.DeleteAllItems();

	//清除所有数据
	m_permRecords.clear();
	m_permDataModel.SetRecords(m_permRecords);

	//设置个数为空
	m_numPermSys = _T("");
	UpdateData(FALSE);
}

void GasSysDlg::OnNMRClickPermsysInputList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem == -1) return; 
	CPoint point;
	::GetCursorPos(&point);

	CMenu menu;
	menu.LoadMenu(IDR_LIST_DEL_MENU);
	CMenu* popup=menu.GetSubMenu(0);
	if(popup==NULL ) return;

	CBitmap m_bitmap1;//位图
	m_bitmap1.LoadBitmap(IDB_DELETE_BITMAP);//加载ID为IDB_BITMAP1的位图
	
	popup->SetMenuItemBitmaps(0,MF_BYPOSITION,&m_bitmap1,&m_bitmap1); 

	popup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this );

	*pResult = 0;
}

void GasSysDlg::OnDeleteItem()
{
	//删除所选项
	int nIndex;
	POSITION pos = m_permCListCtrl.GetFirstSelectedItemPosition();
	nIndex = m_permCListCtrl.GetNextSelectedItem(pos);  // 获取引索项。m_nIndex的值为用户在列表框中选中的行号

	if(nIndex !=-1)
	{
		m_permCListCtrl.DeleteItem(nIndex);
		int num = _ttoi(m_numPermSys);
		if(IsNum(m_numPermSys))
		{
			num = num -1;
		}
		m_numPermSys.Format(_T("%d"),num);
		UpdateData(FALSE);
	}

	pos = m_tempCListCtrl.GetFirstSelectedItemPosition();
	nIndex = m_tempCListCtrl.GetNextSelectedItem(pos);  // 获取引索项。m_nIndex的值为用户在列表框中选中的行号

	if(nIndex!=-1)
	{
		m_tempCListCtrl.DeleteItem(nIndex);
		int num = _ttoi(m_numTempSys);
		if(IsNum(m_numTempSys))
		{
			num = num -1;
		}
		m_numTempSys.Format(_T("%d"),num);
		UpdateData(FALSE);
	}
}

void GasSysDlg::OnBnClickedTempClearButton()
{
	//删除所有项
	m_tempCListCtrl.DeleteAllItems();

	//清除所有数据
	m_tempRecords.clear();
	m_tempDataModel.SetRecords(m_tempRecords);

	//设置个数为空
	m_numTempSys = _T("");
	UpdateData(FALSE);

}

void GasSysDlg::OnBnClickedExitButton()
{
	CDialog::OnCancel();
}

void GasSysDlg::OnNMRClickTempsysInputList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem == -1) return; 
	CPoint point;
	::GetCursorPos(&point);

	CMenu menu;
	menu.LoadMenu(IDR_LIST_DEL_MENU);
	CMenu* popup=menu.GetSubMenu(0);
	if(popup==NULL ) return;

	CBitmap m_bitmap1;//位图
	m_bitmap1.LoadBitmap(IDB_DELETE_BITMAP);//加载ID为IDB_BITMAP1的位图

	popup->SetMenuItemBitmaps(0,MF_BYPOSITION,&m_bitmap1,&m_bitmap1); 

	popup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this );

	*pResult = 0;
}
void GasSysDlg::OnEnKillfocusTempesysNumEdit()
{
	//int oldNum = _ttoi(m_numTempSys);
	UpdateData(TRUE);
	//if(m_numPermSys.IsEmpty()) return;
	//if(oldNum == _ttoi(m_numTempSys)) return;
	if(IsNum(m_numTempSys))
		SetTempItems(_ttoi(m_numTempSys));
}

void GasSysDlg::OnEnSetfocusTempesysNumEdit()
{
	UpdateData(TRUE);
	m_tempRecords.clear();
	for(size_t i = 0; i < _ttoi(m_numTempSys); i++)
	{
		CString	strName = m_tempCListCtrl.GetItemText(i,1);
		CString isRunning = m_tempCListCtrl.GetItemText(i,2);
		CString strReason = m_tempCListCtrl.GetItemText(i,3);
		m_tempRecords.push_back(CListCtrl_DataRecord(strName,isRunning,strReason));
		//acutPrintf(_T("\n第%d行数据:%s"),i+1,strName);
	}
}

void GasSysDlg::SetTempItems( int num )
{
	if( num <= 0 ) return;
	SetItems(m_tempRecords,m_tempDataModel,m_tempCListCtrl,num);
}