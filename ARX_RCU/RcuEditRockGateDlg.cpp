#include "stdafx.h"
#include "RcuEditRockGateDlg.h"
#include "RcuHelper.h"
#include "RcuDataLink.h"
#include "RcuEditDrillSiteDlg.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

IMPLEMENT_DYNAMIC(RcuEditRockGateDlg, RcuAcUiBaseDlg)

RcuEditRockGateDlg::RcuEditRockGateDlg(CWnd* pParent /*=NULL*/)
	: RcuAcUiBaseDlg(RcuEditRockGateDlg::IDD, pParent)
	, m_x(0)
	, m_y(0)
	, m_z(0)
	, m_length(500)
	, m_width(4)
	, m_height(5)
	, m_left(8)
	, m_right(8)
	, m_top(8)
	, m_bottom(8)
	, m_radius(0.08)
	, m_thick(3)
	, m_angle(3)
	, m_dist(50)
{

}

RcuEditRockGateDlg::~RcuEditRockGateDlg()
{
}

void RcuEditRockGateDlg::DoDataExchange(CDataExchange* pDX)
{
	RcuAcUiBaseDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, m_x);
	DDX_Text(pDX, IDC_EDIT7, m_y);
	DDX_Text(pDX, IDC_EDIT9, m_z);
	DDX_Text(pDX, IDC_EDIT1, m_length);
	DDX_Text(pDX, IDC_EDIT2, m_width);
	DDX_Text(pDX, IDC_EDIT3, m_height);
	DDX_Text(pDX, IDC_EDIT10, m_left);
	DDX_Text(pDX, IDC_EDIT11, m_right);
	DDX_Text(pDX, IDC_EDIT12, m_top);
	DDX_Text(pDX, IDC_EDIT13, m_bottom);
	DDX_Text(pDX, IDC_EDIT17, m_radius);
	DDX_Text(pDX, IDC_EDIT14, m_thick);
	DDX_Text(pDX, IDC_EDIT15, m_angle);
	DDX_Text(pDX, IDC_EDIT16, m_dist);
	DDX_Text(pDX, IDC_EDIT18, m_name);
}


BEGIN_MESSAGE_MAP(RcuEditRockGateDlg, RcuAcUiBaseDlg)
	ON_BN_CLICKED(IDOK, &RcuEditRockGateDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/**
在OnInitDialog中退出程序的方法
	1. 调用EndDialog函数:EndDialog(0);
	2. 调用SendMessage（WM_CLOSE）:SendMessage(WM_CLOSE);
*/
BOOL RcuEditRockGateDlg::OnInitDialog()
{
	RcuAcUiBaseDlg::OnInitDialog();

	SetToolTip(IDC_EDIT1,_T("单位:m"));
	SetToolTip(IDC_EDIT2,_T("单位:m"));
	SetToolTip(IDC_EDIT3,_T("单位:m"));
	SetToolTip(IDC_EDIT10,_T("单位:m"));
	SetToolTip(IDC_EDIT11,_T("单位:m"));
	SetToolTip(IDC_EDIT12,_T("单位:m"));
	SetToolTip(IDC_EDIT13,_T("单位:m"));
	SetToolTip(IDC_EDIT14,_T("单位:m"));
	SetToolTip(IDC_EDIT16,_T("单位:m"));
	SetToolTip(IDC_EDIT15,_T("单位:度"));
	SetToolTip(IDC_EDIT17,_T("单位:m"));

	if(m_rock_gate.isNull())
	{
		SetDlgItemText(IDOK, _T("新设计石门"));
	}
	else
	{
		SetDlgItemText(IDOK, _T("更新石门参数"));
	}

	//更新数据到界面
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void RcuEditRockGateDlg::OnBnClickedOk()
{
	//从界面提取数据
	UpdateData( TRUE );

	OnOK();
}

void RcuEditRockGateDlg::exchangeRockGateData(RockGateLink& rg_link, bool save)
{
	if(save)
	{
		//从对话框界面提取数据
		rg_link.m_name = m_name;
		rg_link.m_radius = m_radius;
		rg_link.m_length = m_length;
		rg_link.m_width = m_width;
		rg_link.m_height = m_height;
		rg_link.m_left = m_left;
		rg_link.m_right = m_right;
		rg_link.m_top = m_top;
		rg_link.m_bottom = m_bottom;
		rg_link.m_pt = ArxUtilHelper::Point3dToString(AcGePoint3d(m_x, m_y, m_z));
		rg_link.m_dist = m_dist;
	}
	else
	{
		//给对话框界面赋值
		m_name = rg_link.m_name;
		m_radius = rg_link.m_radius;
		m_length = rg_link.m_length;
		m_width = rg_link.m_width;
		m_height = rg_link.m_height;
		m_left = rg_link.m_left;
		m_right = rg_link.m_right;
		m_top = rg_link.m_top;
		m_bottom = rg_link.m_bottom;
		AcGePoint3d pt;
		ArxUtilHelper::StringToPoint3d(rg_link.m_pt, pt);
		m_x = pt.x; m_y = pt.y; m_z = pt.z;
		m_dist = rg_link.m_dist;
	}
}

void RcuEditRockGateDlg::exchangeCoalSurfaceData(CoalSurfaceLink& cs_link, bool save)
{
	if(save)
	{
		//从对话框界面提取数据
		cs_link.m_thick = m_thick;
		cs_link.m_angle = m_angle;
	}
	else
	{
		//给对话框界面赋值
		m_thick = cs_link.m_thick;
		m_angle = cs_link.m_angle;
	}
}

void RcuEditRockGateDlg::readFromDataLink(RockGateLink& rg_link, CoalSurfaceLink& cs_link)
{
	//如果是新建石门,则不读入数据
	if(m_rock_gate.isNull()) return;

	//交换石门数据到对话框中
	exchangeRockGateData(rg_link, false);
	//交换煤层数据到对话框中
	exchangeCoalSurfaceData(cs_link, false);
}

void RcuEditRockGateDlg::writeToDataLink(RockGateLink& rg_link, CoalSurfaceLink& cs_link)
{
	//从对话框中提取数据给石门
	exchangeRockGateData(rg_link, true);
	//从对话框中提取数据给煤层
	exchangeCoalSurfaceData(cs_link, true);
}