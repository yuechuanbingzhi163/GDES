#include "stdafx.h"
#include "PumpCapacityDlg.h"

#include "CGridListCtrlEx/CGridColumnTraitDateTime.h"
#include "CGridListCtrlEx/CGridColumnTraitEdit.h"
#include "CGridListCtrlEx/CGridColumnTraitCombo.h"
#include "CGridListCtrlEx/CGridColumnTraitHyperLink.h"
#include "CGridListCtrlEx/CGridRowTraitXP.h"
#include "CGridListCtrlEx/ViewConfigSection.h"

#include "config.h"
// PumpCapacityDlg 对话框

IMPLEMENT_DYNAMIC(PumpCapacityDlg, GasBaseAcesDlg)

PumpCapacityDlg::PumpCapacityDlg(CWnd* pParent /*=NULL*/)
	: GasBaseAcesDlg(PumpCapacityDlg::IDD, pParent)
	, m_absGas(_T(""))
	, m_gasConcentration(_T(""))
	, m_maxQ(_T(""))
	, m_localP(_T(""))
	, m_surplus(_T(""))
	, m_workCondiction(_T(""))
	, m_numPump(_T(""))
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
	DDX_Text(pDX, IDC_OVERFLOWER_EDIT, m_surplus);
	DDX_Text(pDX, IDC_SYS_CON_EDIT, m_workCondiction);
	DDX_Text(pDX, IDC_PUMP_NUM_EDIT, m_numPump);
	DDX_Control(pDX, IDC_PUMP_LIST, m_pumpListCtrl);
}


BEGIN_MESSAGE_MAP(PumpCapacityDlg, CDialog)
	ON_BN_CLICKED(IDOK, &PumpCapacityDlg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_PUMP_NUM_EDIT, &PumpCapacityDlg::OnEnKillfocusPumpNumEdit)
END_MESSAGE_MAP()

static void setRecord(vector<CListCtrl_DataRecord>& records,const ArrayVector& datasVector,CString& num)
{
	for (int i = 0; i < datasVector.size(); i++)
	{
		CString strIndx = datasVector[i][0].kACharPtr();
		CString strQ = datasVector[i][1].kACharPtr();
		CString strCon =  datasVector[i][2].kACharPtr();
		CString strP = datasVector[i][3].kACharPtr();
		records.push_back(CListCtrl_DataRecord(strIndx,strQ,strCon,strP));
	}
	num.Format(_T("%d"),datasVector.size());

}

static BOOL IsNum(CString &str)
{
	int n=str.GetLength();
	for(int i=0;i<n;i++)
		if (str[i]<'0'||str[i]>'9') 
			return FALSE;
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
			m_pumpListCtrl.InsertColumnTrait(col+1, title, LVCFMT_LEFT, 4.5*rect.Width()/13, col, pTrait);
		}
		else if(0 == col)
		{
			m_pumpListCtrl.InsertColumnTrait(col+1, title, LVCFMT_CENTER, rect.Width()/13, col, pTrait);
		}
		else
		{
			m_pumpListCtrl.InsertColumnTrait(col+1, title, LVCFMT_LEFT, 3.5*rect.Width()/13, col, pTrait);
		}
	}

	ArrayVector datasVector;

	//获取瓦斯泵数据
	ReportDataHelper::ReadDatas(PUMP_CAPACITY_LIST,datasVector,4);
	//vector<CListCtrl_DataRecord> records;
	if(datasVector.size() > 0) 
	{
		setRecord(m_records,datasVector,m_numPump);
	}

	UpdateData(FALSE);
	if(IsNum(m_numPump))
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
	ReportDataHelper::ReadDatas(MINE_GAS_PUMP_CAPACITY,datasVector,1);
	if(!datasVector.empty())
	{
		m_absGas = datasVector[0][0].kACharPtr();
		m_gasConcentration = datasVector[1][0].kACharPtr();
		m_maxQ = datasVector[2][0].kACharPtr();
		m_localP = datasVector[3][0].kACharPtr();
		m_surplus = datasVector[4][0].kACharPtr();
		m_workCondiction = datasVector[5][0].kACharPtr();
		m_numPump = datasVector[6][0].kACharPtr();
	}

	UpdateData(FALSE);

	OnInitList();
	return TRUE;
}

static void GetDatasFromList(const CGridListCtrlGroups& listCtrl,ArrayVector& datas)
{
	AcStringArray strIndx, strQ, strCon, strP;
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

		CString concentration = listCtrl.GetItemText(i,3);
		if (concentration.IsEmpty())
		{
			concentration = ISNULL;
		}
		strCon.append(concentration);

		CString p = listCtrl.GetItemText(i,4);
		if (p.IsEmpty())
		{
			p = ISNULL;
		}
		strP.append(p);
	}
	datas.push_back(strIndx);
	datas.push_back(strQ);
	datas.push_back(strCon);
	datas.push_back(strP);
}

void PumpCapacityDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	ArrayVector dataVector;
	AcStringArray dataArray;
	dataArray.append(m_absGas);
	dataArray.append(m_gasConcentration);
	dataArray.append(m_maxQ);
	dataArray.append(m_localP);
	dataArray.append(m_surplus);
	dataArray.append(m_workCondiction);
	dataArray.append(m_numPump);

	dataVector.push_back(dataArray);

	ReportDataHelper::WriteDatas(MINE_GAS_PUMP_CAPACITY,dataVector);

	ArrayVector datas;
	GetDatasFromList(m_pumpListCtrl,datas);
	if(!datas.empty())
	{
		ReportDataHelper::WriteDatas(PUMP_CAPACITY_LIST,datas);
	}

}

void PumpCapacityDlg::OnEnKillfocusPumpNumEdit()
{
	UpdateData(TRUE);
	if(IsNum(m_numPump))
	{ 
		if(_ttoi(m_numPump) > 0)
		{
			SetItems(m_records,m_dataModel,m_pumpListCtrl,_ttoi(m_numPump));
		}

	}
}
