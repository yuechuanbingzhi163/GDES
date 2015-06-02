#include "stdafx.h"
#include "PumpCapacityDlg.h"
#include "Caculate.h"
#include "config.h"

#include "CGridListCtrlEx/CGridColumnTraitDateTime.h"
#include "CGridListCtrlEx/CGridColumnTraitEdit.h"
#include "CGridListCtrlEx/CGridColumnTraitCombo.h"
#include "CGridListCtrlEx/CGridColumnTraitHyperLink.h"
#include "CGridListCtrlEx/CGridRowTraitXP.h"
#include "CGridListCtrlEx/ViewConfigSection.h"

// PumpCapacityDlg 对话框

IMPLEMENT_DYNAMIC(PumpCapacityDlg, GasBaseAcesDlg)

PumpCapacityDlg::PumpCapacityDlg(CWnd* pParent /*=NULL*/)
	: GasBaseAcesDlg(PumpCapacityDlg::IDD, pParent)
	, m_absGas(_T(""))
	, m_gasConcentration(_T(""))
	, m_maxQ(_T(""))
	, m_localP(_T(""))
	, m_workCondiction(_T(""))
	, m_numPump(_T(""))
	, m_pumpRet(_T(""))
	, m_sysRet(_T(""))
{

}

PumpCapacityDlg::~PumpCapacityDlg()
{
}

void PumpCapacityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ABS_GAS_EDIT, m_absGas);
	DDX_Text(pDX, IDC_CONCENTRA_GAS_EDIT, m_gasConcentration);
	DDX_Text(pDX, IDC_MINE_MAXQ_EDIT, m_maxQ);
	DDX_Text(pDX, IDC_LOCALAP_EDIT, m_localP);
	DDX_Text(pDX, IDC_SYS_CON_EDIT, m_workCondiction);
	DDX_Text(pDX, IDC_PUMP_NUM_EDIT, m_numPump);
	DDX_Control(pDX, IDC_PUMP_LIST, m_pumpListCtrl);
	DDX_Text(pDX, IDC_PUMP_RET_EDIT, m_pumpRet);
	DDX_Text(pDX, IDC_SYS_RET_EDIT, m_sysRet);
}


BEGIN_MESSAGE_MAP(PumpCapacityDlg, CDialog)
	ON_BN_CLICKED(IDOK, &PumpCapacityDlg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_PUMP_NUM_EDIT, &PumpCapacityDlg::OnEnKillfocusPumpNumEdit)
END_MESSAGE_MAP()

static void setRecord(vector<GasCapacity_DataRecord>& records,const ArrayVector& datasVector,CString& num)
{
	for (int i = 0; i < datasVector.size(); i++)
	{
		CString strIndx = datasVector[i][0].kACharPtr();
		CString strQ = datasVector[i][1].kACharPtr();
		CString strCon =  datasVector[i][2].kACharPtr();
		CString strP = datasVector[i][3].kACharPtr();
		CString strKP = datasVector[i][4].kACharPtr();
		CString strD = datasVector[i][5].kACharPtr();
		CString strV =  datasVector[i][6].kACharPtr();
		CString strKS = datasVector[i][7].kACharPtr();

		records.push_back(GasCapacity_DataRecord(strIndx,strQ,strCon,strP,strKP,strD,strV,strKS));
	}
	num.Format(_T("%d"),datasVector.size());

}

static BOOL IsNum(CString &str)
{
	int n=str.GetLength();
	for(int i=0;i<n;i++)
		if ((str[i]<'0'||str[i]>'9') && str[i] != '.') 
			return FALSE;
	return TRUE;
}

static BOOL IsInt(CString &str)
{
	int n=str.GetLength();
	for(int i=0;i<n;i++)
		if (str[i]<'0'||str[i]>'9') 
			return FALSE;
	return TRUE;
}

static void SetItems(vector<GasCapacity_DataRecord>& records,GasCapacity_DataModel&dataModel,CGridListCtrlGroups& listCtrl,int num)
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

void PumpCapacityDlg::OnInitList()
{
	m_pumpListCtrl.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;

	CRect rect;  
	m_pumpListCtrl.GetClientRect(rect); //获得当前客户区信息  

	m_pumpListCtrl.InsertHiddenLabelColumn();	// Requires one never uses column 0
	int colCount = m_dataModel.GetColCount();
	for(int col = 0; col <  colCount; ++col)
	{
		const CString& title = m_dataModel.GetColTitle(col);
		CGridColumnTrait* pTrait = NULL;
		if(0 != col)
		{
			pTrait = new CGridColumnTraitEdit;
		}
		if(1 == col)
		{
			m_pumpListCtrl.InsertColumnTrait(col+1, title, LVCFMT_LEFT, 10*rect.Width()/56, col, pTrait);
		}
		else if(0 == col)
		{
			m_pumpListCtrl.InsertColumnTrait(col+1, title, LVCFMT_CENTER, 3*rect.Width()/56, col, pTrait);
		}

		else if(2 == col)
		{
			m_pumpListCtrl.InsertColumnTrait(col+1, title, LVCFMT_LEFT, 8*rect.Width()/56, col, pTrait);
		}

		else
		{
			m_pumpListCtrl.InsertColumnTrait(col+1, title, LVCFMT_LEFT, rect.Width()/8, col, pTrait);
		}
	}

	ArrayVector datasVector;

	//获取瓦斯泵数据
	ReportDataHelper::ReadDatas(PUMP_CAPACITY_LIST,datasVector,8);
	//vector<CListCtrl_DataRecord> records;
	if(datasVector.size() > 0) 
	{
		setRecord(m_records,datasVector,m_numPump);
	}

	UpdateData(FALSE);
	if(IsInt(m_numPump))
	{ 
		if(_ttoi(m_numPump) > 0)
		{
			SetItems(m_records,m_dataModel,m_pumpListCtrl,_ttoi(m_numPump));
		}

	}

}

BOOL PumpCapacityDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();

	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(MINE_GAS_CAPACITY,datasVector,1);
	m_workCondiction = _T("0.75");
	if(!datasVector.empty())
	{
		m_absGas = datasVector[0][0].kACharPtr();
		m_gasConcentration = datasVector[1][0].kACharPtr();
		m_localP = datasVector[2][0].kACharPtr();
		m_maxQ = datasVector[3][0].kACharPtr();
		m_workCondiction = datasVector[4][0].kACharPtr();
		m_numPump = datasVector[5][0].kACharPtr();
		m_pumpRet = datasVector[6][0].kACharPtr();
		m_sysRet = datasVector[7][0].kACharPtr();
	}

	UpdateData(FALSE);

	OnInitList();
	return TRUE;
}

static void GetDatasFromList(const CGridListCtrlGroups& listCtrl,ArrayVector& datas,AcStringArray& dataArray)
{
	AcStringArray strIndx, strQ, strCon, strP,strKP, strD, strV, strKS;
	for(size_t i = 0; i < listCtrl.GetItemCount(); i++)
	{
		CString	indx = listCtrl.GetItemText(i,1);
		if (indx.IsEmpty())
		{
			indx = ISNULL;
		}
		strIndx.append(indx);

		CString q = listCtrl.GetItemText(i,2);
		if (q.IsEmpty())
		{
			q = ISNULL;
		}
		strQ.append(q);
		dataArray.append(q);

		CString concentration = listCtrl.GetItemText(i,3);
		if (concentration.IsEmpty())
		{
			concentration = ISNULL;
		}
		strCon.append(concentration);
		dataArray.append(concentration);

		CString p = listCtrl.GetItemText(i,4);
		if (p.IsEmpty())
		{
			p = ISNULL;
		}
		strP.append(p);
		dataArray.append(p);

		CString kp = listCtrl.GetItemText(i,5);
		if (kp.IsEmpty())
		{
			kp = ISNULL;
		}
		strKP.append(kp);
		dataArray.append(kp);

		CString d = listCtrl.GetItemText(i,6);
		if (d.IsEmpty())
		{
			d = ISNULL;
		}
		strD.append(d);
		dataArray.append(d);

		CString v = listCtrl.GetItemText(i,7);
		if (v.IsEmpty())
		{
			v = ISNULL;
		}
		strV.append(v);
		dataArray.append(v);

		CString ks = listCtrl.GetItemText(i,8);
		if (ks.IsEmpty())
		{
			ks = ISNULL;
		}
		strKS.append(ks);
		dataArray.append(ks);

	}
	datas.push_back(strIndx);
	datas.push_back(strQ);
	datas.push_back(strCon);
	datas.push_back(strP);
	datas.push_back(strKP);
	datas.push_back(strD);
	datas.push_back(strV);
	datas.push_back(strKS);

}

void PumpCapacityDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	ArrayVector dataVector;
	AcStringArray dataArray;
	dataArray.append(m_absGas);
	dataArray.append(m_gasConcentration);
	dataArray.append(m_localP);
	dataArray.append(m_maxQ);
	dataArray.append(m_workCondiction);
	dataArray.append(m_numPump);

	ArrayVector datas;
	AcStringArray pumpListArray;
	GetDatasFromList(m_pumpListCtrl,datas,pumpListArray);

	CString strPumpRet,strSysRet;
	if(!Calculate::MineGasCapacityCacul(dataArray,pumpListArray,strPumpRet,strSysRet))
	{
		AfxMessageBox(_T("数据错误!"));
	}

	dataArray.append(strPumpRet);
	dataArray.append(strSysRet);
	dataVector.push_back(dataArray);

	ReportDataHelper::WriteDatas(MINE_GAS_CAPACITY,dataVector);
	if(!datas.empty())
	{
		ReportDataHelper::WriteDatas(PUMP_CAPACITY_LIST,datas);
	}

	m_pumpRet = strPumpRet;
	m_sysRet = strSysRet;
	UpdateData(FALSE);
}

void PumpCapacityDlg::OnEnKillfocusPumpNumEdit()
{
	UpdateData(TRUE);
	if(IsInt(m_numPump))
	{ 
		if(_ttoi(m_numPump) > 0)
		{
			SetItems(m_records,m_dataModel,m_pumpListCtrl,_ttoi(m_numPump));
		}

	}
}
