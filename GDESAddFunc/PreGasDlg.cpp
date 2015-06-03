#include "stdafx.h"
#include "PreGasDlg.h"
#include "Caculate.h"
#include "config.h"

#include "CGridListCtrlEx/CGridColumnTraitDateTime.h"
#include "CGridListCtrlEx/CGridColumnTraitEdit.h"
#include "CGridListCtrlEx/CGridColumnTraitCombo.h"
#include "CGridListCtrlEx/CGridColumnTraitHyperLink.h"
#include "CGridListCtrlEx/CGridRowTraitXP.h"
#include "CGridListCtrlEx/ViewConfigSection.h"


IMPLEMENT_DYNAMIC(PreGasDlg, GasBaseAcesDlg)

PreGasDlg::PreGasDlg(CWnd* pParent /*=NULL*/)
	: GasBaseAcesDlg(PreGasDlg::IDD, pParent)
	, m_preMaxGas(_T(""))
	, m_leafGas(_T(""))
	, m_mineReRate(_T(""))
	, m_forGas(_T(""))
	, m_gasY(_T(""))
	, m_TTJ(_T(""))
	, m_faceNum(_T(""))
	, m_ret(_T(""))
{

}

PreGasDlg::~PreGasDlg()
{
}

void PreGasDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PRE_MAX_GAS_EDIT, m_preMaxGas);
	DDX_Text(pDX, IDC_PRE_LEAFT_GAS_EDIT, m_leafGas);
	DDX_Text(pDX, IDC_PRE_MINE_RERATE_EDIT, m_mineReRate);
	DDX_Text(pDX, IDC_PRE_FOR_GAS_EDIT, m_forGas);
	DDX_Text(pDX, IDC_PRE_GAS_EDIT, m_gasY);
	DDX_Text(pDX, IDC_PRE_TT_MINE_EDIT, m_TTJ);
	DDX_Text(pDX, IDC_FACE_NUM_EDIT, m_faceNum);
	DDX_Control(pDX, IDC_MINE_FACE_LIST, m_mineFaceList);
	DDX_Text(pDX, IDC_PRE_RET_EDIT, m_ret);
}


BEGIN_MESSAGE_MAP(PreGasDlg, CDialog)
	ON_EN_KILLFOCUS(IDC_FACE_NUM_EDIT, &PreGasDlg::OnEnKillfocusFaceNumEdit)
	ON_BN_CLICKED(IDOK, &PreGasDlg::OnBnClickedOk)
END_MESSAGE_MAP()

static void setRecord(vector<PreGas_DataRecord>& records,const ArrayVector& datasVector,CString& num)
{
	for (int i = 0; i < datasVector.size(); i++)
	{
		CString strIndx = datasVector[i][0].kACharPtr();
		CString strLenth = datasVector[i][1].kACharPtr();
		CString strHeight =  datasVector[i][2].kACharPtr();
		CString strDencity = datasVector[i][3].kACharPtr();
		CString strDayAdvance = datasVector[i][4].kACharPtr();
		CString strRecovRate = datasVector[i][5].kACharPtr();
		records.push_back(PreGas_DataRecord(strIndx,strLenth,strHeight,strDencity,strDayAdvance,strRecovRate));
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

static void SetItems(vector<PreGas_DataRecord>& records,PreGas_DataModel& dataModel,CGridListCtrlGroups& listCtrl,int num)
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

/*数据保存顺序
 * 核定区域内煤层的最大瓦斯含量
 * 抽采后满足防突要求的残余瓦斯含量
 * 矿井回采率
 * 矿井年实际预抽瓦斯量
 * 邻近层和围岩瓦斯储量系数
 * 矿井掘进出煤系数
 * 工作面数
*/

BOOL PreGasDlg::OnInitDialog()
{
	GasBaseAcesDlg::OnInitDialog();
	ArrayVector datasVector;
	ReportDataHelper::ReadDatas(PRE_GAS,datasVector,1);
	m_gasY = _T("1.2");
	m_TTJ = _T("1.2");
	if(!datasVector.empty())
	{
		m_preMaxGas = datasVector[0][0].kACharPtr();
		m_leafGas = datasVector[1][0].kACharPtr();
		m_mineReRate = datasVector[2][0].kACharPtr();
		m_forGas = datasVector[3][0].kACharPtr();
		m_gasY = datasVector[4][0].kACharPtr();
		m_TTJ = datasVector[5][0].kACharPtr();
		m_faceNum = datasVector[6][0].kACharPtr();
		m_ret = datasVector[7][0].kACharPtr();
	}

	UpdateData(FALSE);

	SetToolTip(IDC_PRE_MAX_GAS_EDIT,_T("核定区域内煤层的最大瓦斯含量"));
	SetToolTip(IDC_PRE_LEAFT_GAS_EDIT,_T("抽采后满足防突要求的残余瓦斯含量"));
	SetToolTip(IDC_PRE_FOR_GAS_EDIT,_T("矿井年实际预抽瓦斯量"));
	SetToolTip(IDC_PRE_GAS_EDIT,_T("邻近层和围岩瓦斯储量系数"));
	OnInitList();
	return TRUE;
}

void PreGasDlg::OnInitList()
{
	m_mineFaceList.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;

	CRect rect;  
	m_mineFaceList.GetClientRect(rect); //获得当前客户区信息  

	m_mineFaceList.InsertHiddenLabelColumn();	// Requires one never uses column 0
	int colCount = m_dataModel.GetColCount();
	for(int col = 0; col <  colCount; ++col)
	{
		const CString& title = m_dataModel.GetColTitle(col);
		CGridColumnTrait* pTrait = NULL;
		if(0 != col)
		{
			pTrait = new CGridColumnTraitEdit;
		}
		if(0 == col)
		{
			m_mineFaceList.InsertColumnTrait(col+1, title, LVCFMT_CENTER, rect.Width()/11, col, pTrait);
		}
		else if (1 == col || 2 == col)
		{
			m_mineFaceList.InsertColumnTrait(col+1, title, LVCFMT_LEFT, 1.5*rect.Width()/11, col, pTrait);
		}

		else
		{
			m_mineFaceList.InsertColumnTrait(col+1, title, LVCFMT_LEFT, 7*rect.Width()/33, col, pTrait);
		}
	}

	ArrayVector datasVector;

	//获取瓦斯泵数据
	ReportDataHelper::ReadDatas(PRE_GAS_LIST,datasVector,6);
	//vector<CListCtrl_DataRecord> records;
	if(datasVector.size() > 0) 
	{
		setRecord(m_records,datasVector,m_faceNum);
	}

	UpdateData(FALSE);
	if(IsInt(m_faceNum))
	{ 
		if(_ttoi(m_faceNum) > 0)
		{
			SetItems(m_records,m_dataModel,m_mineFaceList,_ttoi(m_faceNum));
		}

	}


}
// PreGasDlg 消息处理程序

void PreGasDlg::OnEnKillfocusFaceNumEdit()
{
	UpdateData(TRUE);
	if(IsInt(m_faceNum))
	{ 
		if(_ttoi(m_faceNum) > 0)
		{
			SetItems(m_records,m_dataModel,m_mineFaceList,_ttoi(m_faceNum));
		}

	}
}

static void GetDatasFromList(const CGridListCtrlGroups& listCtrl,ArrayVector& datas,AcStringArray& dataArray)
{
	AcStringArray strIndx, strLength, strHeight, strDencity,strDayAdvance, strRecovRate;
	for(size_t i = 0; i < listCtrl.GetItemCount(); i++)
	{
		CString	indx = listCtrl.GetItemText(i,1);
		if (indx.IsEmpty())
		{
			indx = ISNULL;
		}
		strIndx.append(indx);

		CString length = listCtrl.GetItemText(i,2);
		if (length.IsEmpty())
		{
			length = ISNULL;
		}
		strLength.append(length);
		dataArray.append(length);

		CString height = listCtrl.GetItemText(i,3);
		if (height.IsEmpty())
		{
			height = ISNULL;
		}
		strHeight.append(height);
		dataArray.append(height);

		CString dencity = listCtrl.GetItemText(i,4);
		if (dencity.IsEmpty())
		{
			dencity = ISNULL;
		}
		strDencity.append(dencity);
		dataArray.append(dencity);

		CString dayAd = listCtrl.GetItemText(i,5);
		if (dayAd.IsEmpty())
		{
			dayAd = ISNULL;
		}
		strDayAdvance.append(dayAd);
		dataArray.append(dayAd);

		CString recovRate = listCtrl.GetItemText(i,6);
		if (recovRate.IsEmpty())
		{
			recovRate = ISNULL;
		}
		strRecovRate.append(recovRate);
		dataArray.append(recovRate);
	}
	datas.push_back(strIndx);
	datas.push_back(strLength);
	datas.push_back(strHeight);
	datas.push_back(strDencity);
	datas.push_back(strDayAdvance);
	datas.push_back(strRecovRate);
}

void PreGasDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	ArrayVector dataVector;
	AcStringArray dataArray;
	dataArray.append(m_preMaxGas);
	dataArray.append(m_leafGas);
	dataArray.append(m_mineReRate);
	dataArray.append(m_forGas);
	dataArray.append(m_gasY);
	dataArray.append(m_TTJ);
	dataArray.append(m_faceNum);

	ArrayVector datas;
	AcStringArray faceListArray;
	GetDatasFromList(m_mineFaceList,datas,faceListArray);

	CString strRet;
	if(!Calculate::MinePreGasCacul(dataArray,faceListArray,strRet))
	{
		AfxMessageBox(_T("数据错误!"));
	}

	dataArray.append(strRet);
	dataVector.push_back(dataArray);

	ReportDataHelper::WriteDatas(PRE_GAS,dataVector);
	if(!datas.empty())
	{
		ReportDataHelper::WriteDatas(PRE_GAS_LIST,datas);
	}

	m_ret = strRet;
	UpdateData(FALSE);
}
