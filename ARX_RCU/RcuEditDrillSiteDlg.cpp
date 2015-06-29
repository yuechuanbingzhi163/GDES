#include "stdafx.h"
#include "RcuHelper.h"
#include "RcuDataLink.h"
#include "RcuEditDrillSiteDlg.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

IMPLEMENT_DYNAMIC(RcuEditDrillSiteDlg, RcuAcUiBaseDlg)

RcuEditDrillSiteDlg::RcuEditDrillSiteDlg(CWnd* pParent /*=NULL*/)
	: RcuAcUiBaseDlg(RcuEditDrillSiteDlg::IDD, pParent)
	, m_x(0)
	, m_y(0)
	, m_z(0)
	, m_width(4)
	, m_height(5)
	, m_left(15)
	, m_right(15)
	, m_top(10)
	, m_bottom(10)
	, m_gas_radius(3)
	, m_thick(3)
	, m_angle(8)
	, m_dist(50)
	, m_pore_size(0.09)
	, m_index(1)
{

}

RcuEditDrillSiteDlg::~RcuEditDrillSiteDlg()
{
}

void RcuEditDrillSiteDlg::DoDataExchange(CDataExchange* pDX)
{
	RcuAcUiBaseDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, m_x);
	DDX_Text(pDX, IDC_EDIT7, m_y);
	DDX_Text(pDX, IDC_EDIT9, m_z);
	DDX_Text(pDX, IDC_EDIT2, m_width);
	DDX_Text(pDX, IDC_EDIT3, m_height);
	DDX_Text(pDX, IDC_EDIT10, m_left);
	DDX_Text(pDX, IDC_EDIT11, m_right);
	DDX_Text(pDX, IDC_EDIT12, m_top);
	DDX_Text(pDX, IDC_EDIT13, m_bottom);
	DDX_Text(pDX, IDC_EDIT17, m_gas_radius);
	DDX_Text(pDX, IDC_EDIT14, m_thick);
	DDX_Text(pDX, IDC_EDIT15, m_angle);
	DDX_Text(pDX, IDC_EDIT16, m_dist);
	DDX_Text(pDX, IDC_EDIT18, m_name);
	DDX_Text(pDX, IDC_PORE_SIZE_EDIT, m_pore_size);
	DDX_Text(pDX, IDC_INDEX_EDIT, m_index);
}


BEGIN_MESSAGE_MAP(RcuEditDrillSiteDlg, RcuAcUiBaseDlg)
	ON_BN_CLICKED(IDOK, &RcuEditDrillSiteDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/**
在OnInitDialog中退出程序的方法
	1. 调用EndDialog函数:EndDialog(0);
	2. 调用SendMessage（WM_CLOSE）:SendMessage(WM_CLOSE);
*/
BOOL RcuEditDrillSiteDlg::OnInitDialog()
{
	RcuAcUiBaseDlg::OnInitDialog();

	SetToolTip(IDC_EDIT2,_T("迎头和钻场（耳洞）总宽度:m"));
	SetToolTip(IDC_EDIT3,_T("单位:m"));
	SetToolTip(IDC_EDIT10,_T("单位:m"));
	SetToolTip(IDC_EDIT11,_T("单位:m"));
	SetToolTip(IDC_EDIT12,_T("单位:m"));
	SetToolTip(IDC_EDIT13,_T("单位:m"));
	SetToolTip(IDC_EDIT14,_T("单位:m"));
	SetToolTip(IDC_EDIT16,_T("单位:m"));
	SetToolTip(IDC_EDIT15,_T("单位:度"));
	SetToolTip(IDC_EDIT17,_T("单位:m"));
	SetToolTip(IDC_PORE_SIZE_EDIT, _T("钻孔孔径:m"));
	SetToolTip(IDC_PORE_GAP_EDIT,  _T("钻孔之间间距:m"));
	SetToolTip(IDC_INDEX_EDIT,  _T("钻孔指定起始编号"));

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

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void RcuEditDrillSiteDlg::OnBnClickedOk()
{
	//从界面提取数据
	UpdateData( TRUE );

	OnOK();
}

void RcuEditDrillSiteDlg::exchangeDrillSiteData( DrillSiteLink& ds_link, bool save)
{
	if(save)
	{
		//从对话框界面提取数据
		ds_link.m_name = m_name;
		ds_link.m_width = m_width;
		ds_link.m_height = m_height;
		ds_link.m_left = m_left;
		ds_link.m_right = m_right;
		ds_link.m_top = m_top;
		ds_link.m_bottom = m_bottom;
		ds_link.m_pt = AcGePoint3d(m_x, m_y, m_z);
		ds_link.m_dist = m_dist;
		ds_link.m_pore_size = m_pore_size;
		ds_link.m_start = m_index;
	}
	else
	{
		//给对话框界面赋值
		m_name = ds_link.m_name;
		m_width = ds_link.m_width;
		m_height = ds_link.m_height;
		m_left = ds_link.m_left;
		m_right = ds_link.m_right;
		m_top = ds_link.m_top;
		m_bottom = ds_link.m_bottom;
		AcGePoint3d pt = ds_link.m_pt;
		m_x = pt.x; m_y = pt.y; m_z = pt.z;
		m_dist = ds_link.m_dist;
		m_pore_size = ds_link.m_pore_size;
		m_index = ds_link.m_start;
	}
}

void RcuEditDrillSiteDlg::exchangeCoalSurfaceData(CoalSurfaceLink& cs_link, bool save)
{
	if(save)
	{
		//从对话框界面提取数据
		cs_link.m_thick = m_thick;
		cs_link.m_angle = m_angle;
		cs_link.m_gas_radius = m_gas_radius;
	}
	else
	{
		//给对话框界面赋值
		m_thick = cs_link.m_thick;
		m_angle = cs_link.m_angle;
		m_gas_radius = cs_link.m_gas_radius;
	}
}

void RcuEditDrillSiteDlg::readFromDataLink( DrillSiteLink& ds_link,CoalSurfaceLink& cs_link)
{
	//如果是新建钻场,则不读入数据
	if(m_drill_site.isNull()) return;

	//交换钻场数据到对话框中
	exchangeDrillSiteData(ds_link, false);
	//交换煤层数据到对话框中
	exchangeCoalSurfaceData(cs_link, false);
}

void RcuEditDrillSiteDlg::writeToDataLink( DrillSiteLink& ds_link, CoalSurfaceLink& cs_link)
{
	//从对话框中提取数据给钻场
	exchangeDrillSiteData(ds_link, true);
	//从对话框中提取数据给煤层
	exchangeCoalSurfaceData(cs_link, true);
}