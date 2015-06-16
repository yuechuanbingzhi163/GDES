// RcuAddDrillDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RcuAddDrillDlg.h"


// RcuAddDrillDlg 对话框

IMPLEMENT_DYNAMIC(RcuAddDrillDlg, RcuAcUiBaseDlg)

RcuAddDrillDlg::RcuAddDrillDlg(CWnd* pParent /*=NULL*/)
	: RcuAcUiBaseDlg(RcuAddDrillDlg::IDD, pParent)
	, m_name(_T(""))
	, m_loca(0)
	, m_width(0)
	, m_height(0)
	, m_dist(0)
	, m_index(0)
{

}

RcuAddDrillDlg::~RcuAddDrillDlg()
{
}

void RcuAddDrillDlg::DoDataExchange(CDataExchange* pDX)
{
	RcuAcUiBaseDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DRIL_NAME_EDIT, m_name);
	DDX_Text(pDX, IDC_DRILL_LOCA_EDIT, m_loca);
	DDV_MinMaxInt(pDX, m_loca, 0, 2);
	DDX_Text(pDX, IDC_DRILL_WIDTH_EDIT, m_width);
	DDX_Text(pDX, IDC_DRILL_HEIGHT_EDIT, m_height);
	DDX_Text(pDX, IDC_DRILL_DIS_EDIT, m_dist);
	DDX_Text(pDX, IDC_DRILL_INDEX_EDIT, m_index);
}


BEGIN_MESSAGE_MAP(RcuAddDrillDlg, RcuAcUiBaseDlg)
	ON_BN_CLICKED(IDOK, &RcuAddDrillDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// RcuAddDrillDlg 消息处理程序

void RcuAddDrillDlg::OnBnClickedOk()
{
	UpdateData( TRUE );
	OnOK();
}

void RcuAddDrillDlg::writeToDataLink( DrillSiteLink& ds_link )
{
	ds_link.m_name = m_name;
	ds_link.m_leftOrRight = m_loca;
	ds_link.m_dist = m_dist;
	ds_link.m_width = m_width;
	ds_link.m_height = m_height;
	ds_link.m_start = m_index;
}

BOOL RcuAddDrillDlg::OnInitDialog()
{
	RcuAcUiBaseDlg::OnInitDialog();

	return TRUE; 
}