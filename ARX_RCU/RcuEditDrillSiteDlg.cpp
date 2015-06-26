#include "stdafx.h"
#include "RcuEditDrillSiteDlg.h"

IMPLEMENT_DYNAMIC(RcuEditDrillSiteDlg, RcuAcUiBaseDlg)

RcuEditDrillSiteDlg::RcuEditDrillSiteDlg(CWnd* pParent /*=NULL*/)
	: RcuAcUiBaseDlg(RcuEditDrillSiteDlg::IDD, pParent)
	, m_name(_T(""))
	, m_width(3)
	, m_height(4)
	, m_dist(50)
	, m_index(1)
	, m_pore_size(0.09)
{

}

RcuEditDrillSiteDlg::~RcuEditDrillSiteDlg()
{
}

void RcuEditDrillSiteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DRIL_NAME_EDIT, m_name);
	DDX_Text(pDX, IDC_DRILL_WIDTH_EDIT, m_width);
	DDX_Text(pDX, IDC_DRILL_HEIGHT_EDIT, m_height);
	DDX_Text(pDX, IDC_DRILL_DIS_EDIT, m_dist);
	DDX_Text(pDX, IDC_DRILL_INDEX_EDIT, m_index);
	DDX_Text(pDX, IDC_DRILL_RADIUS_EDIT, m_pore_size);
}


BEGIN_MESSAGE_MAP(RcuEditDrillSiteDlg, RcuAcUiBaseDlg)
	ON_BN_CLICKED(IDOK, &RcuEditDrillSiteDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL RcuEditDrillSiteDlg::OnInitDialog()
{
	RcuAcUiBaseDlg::OnInitDialog();

	SetToolTip(IDC_DRILL_WIDTH_EDIT,_T("单位:m"));
	SetToolTip(IDC_DRILL_HEIGHT_EDIT,_T("单位:m"));
	SetToolTip(IDC_DRILL_DIS_EDIT,_T("单位:m"));

	if(m_drill_site.isNull())
	{
		SetDlgItemText(IDOK, _T("新设计钻场"));
	}
	else
	{
		SetDlgItemText(IDOK, _T("更新钻场参数"));
	}

	//更新数据到界面
	UpdateData(FALSE);

	return TRUE; 
}

void RcuEditDrillSiteDlg::OnBnClickedOk()
{
	UpdateData( TRUE );

	OnOK();
}

void RcuEditDrillSiteDlg::writeToDataLink( DrillSiteLink& ds_link )
{
	ds_link.m_name = m_name;
	ds_link.m_dist = m_dist;
	ds_link.m_width = m_width;
	ds_link.m_height = m_height;
	ds_link.m_start = m_index;
	ds_link.m_pore_size = m_pore_size;
}

void RcuEditDrillSiteDlg::readFromDataLink( DrillSiteLink& ds_link )
{
	if(m_drill_site.isNull()) return;

	m_name = ds_link.m_name;
	m_dist = ds_link.m_dist;
	m_width = ds_link.m_width;
	m_height = ds_link.m_height;
	m_index = ds_link.m_start;
	m_pore_size = ds_link.m_pore_size;
}