#include "stdafx.h"
#include "RcuEditDrillDlg.h"


// RcuEditDrillDlg 对话框

IMPLEMENT_DYNAMIC(RcuEditDrillDlg, RcuAcUiBaseDlg)

RcuEditDrillDlg::RcuEditDrillDlg(CWnd* pParent /*=NULL*/)
	: RcuAcUiBaseDlg(RcuEditDrillDlg::IDD, pParent)
	, m_name(_T(""))
	, m_leftOrRight(0)
	, m_width(0)
	, m_height(0)
	, m_dist(0)
	, m_index(0)
{

}

RcuEditDrillDlg::~RcuEditDrillDlg()
{
}

void RcuEditDrillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DRIL_NAME_EDIT, m_name);
	DDX_Text(pDX, IDC_DRILL_WIDTH_EDIT, m_width);
	DDX_Text(pDX, IDC_DRILL_HEIGHT_EDIT, m_height);
	DDX_Text(pDX, IDC_DRILL_DIS_EDIT, m_dist);
	DDX_Text(pDX, IDC_DRILL_INDEX_EDIT, m_index);
	DDX_Control(pDX, IDC_LOCA_COMBO, m_locaCombBox);
}


BEGIN_MESSAGE_MAP(RcuEditDrillDlg, RcuAcUiBaseDlg)
	ON_BN_CLICKED(IDOK, &RcuEditDrillDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL RcuEditDrillDlg::OnInitDialog()
{
	RcuAcUiBaseDlg::OnInitDialog();
	m_locaCombBox.AddString(_T("左帮钻场"));
	m_locaCombBox.AddString(_T("右帮钻场"));
	m_locaCombBox.SetCurSel(m_leftOrRight);
	UpdateData(FALSE);

	SetToolTip(IDC_DRILL_WIDTH_EDIT,_T("单位:m"));
	SetToolTip(IDC_DRILL_HEIGHT_EDIT,_T("单位:m"));
	SetToolTip(IDC_DRILL_DIS_EDIT,_T("单位:m"));

	return TRUE; 
}

void RcuEditDrillDlg::OnBnClickedOk()
{
	UpdateData( TRUE );
	m_leftOrRight = m_locaCombBox.GetCurSel();
	OnOK();
}

void RcuEditDrillDlg::writeToDataLink( DrillSiteLink& ds_link )
{
	ds_link.m_name = m_name;
	ds_link.m_leftOrRight = m_leftOrRight;
	ds_link.m_dist = m_dist;
	ds_link.m_width = m_width;
	ds_link.m_height = m_height;
	ds_link.m_start = m_index;
}

void RcuEditDrillDlg::readFromDataLink( DrillSiteLink& ds_link )
{
	m_name = ds_link.m_name;
	m_leftOrRight = ds_link.m_leftOrRight;
	m_dist = ds_link.m_dist;
	m_width = ds_link.m_width;
	m_height = ds_link.m_height;
	m_index = ds_link.m_start;
}