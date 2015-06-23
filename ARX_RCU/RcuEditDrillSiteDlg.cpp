#include "stdafx.h"
#include "RcuEditDrillSiteDlg.h"

IMPLEMENT_DYNAMIC(RcuEditDrillSiteDlg, RcuAcUiBaseDlg)

RcuEditDrillSiteDlg::RcuEditDrillSiteDlg(CWnd* pParent /*=NULL*/)
	: RcuAcUiBaseDlg(RcuEditDrillSiteDlg::IDD, pParent)
	, m_name(_T(""))
	, m_leftOrRight(0)
	, m_width(0)
	, m_height(0)
	, m_dist(0)
	, m_index(0)
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
	DDX_Control(pDX, IDC_LOCA_COMBO, m_locaCombBox);
}


BEGIN_MESSAGE_MAP(RcuEditDrillSiteDlg, RcuAcUiBaseDlg)
	ON_BN_CLICKED(IDOK, &RcuEditDrillSiteDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL RcuEditDrillSiteDlg::OnInitDialog()
{
	RcuAcUiBaseDlg::OnInitDialog();

	m_locaCombBox.AddString(_T("左帮钻场"));
	m_locaCombBox.AddString(_T("右帮钻场"));

	SetToolTip(IDC_DRILL_WIDTH_EDIT,_T("单位:m"));
	SetToolTip(IDC_DRILL_HEIGHT_EDIT,_T("单位:m"));
	SetToolTip(IDC_DRILL_DIS_EDIT,_T("单位:m"));

	if(m_drill_site.isNull())
	{
		SetDlgItemText(IDOK, _T("新设计钻场"));
		m_locaCombBox.SetCurSel(0);
	}
	else
	{
		SetDlgItemText(IDOK, _T("更新钻场参数"));
		m_locaCombBox.SetCurSel(m_leftOrRight);
	}

	//更新数据到界面
	UpdateData(FALSE);

	return TRUE; 
}

void RcuEditDrillSiteDlg::OnBnClickedOk()
{
	UpdateData( TRUE );

	m_leftOrRight = m_locaCombBox.GetCurSel();

	OnOK();
}

void RcuEditDrillSiteDlg::writeToDataLink( DrillSiteLink& ds_link )
{
	ds_link.m_name = m_name;
	ds_link.m_leftOrRight = m_leftOrRight;
	ds_link.m_dist = m_dist;
	ds_link.m_width = m_width;
	ds_link.m_height = m_height;
	ds_link.m_start = m_index;
	ds_link.m_leftOrRight = m_leftOrRight;
}

void RcuEditDrillSiteDlg::readFromDataLink( DrillSiteLink& ds_link )
{
	if(m_drill_site.isNull()) return;

	m_name = ds_link.m_name;
	m_leftOrRight = ds_link.m_leftOrRight;
	m_dist = ds_link.m_dist;
	m_width = ds_link.m_width;
	m_height = ds_link.m_height;
	m_index = ds_link.m_start;

	if(m_leftOrRight > 1 || m_leftOrRight < 0)
	{
		m_leftOrRight = 0;
	}
	m_locaCombBox.SetCurSel(m_leftOrRight);
}