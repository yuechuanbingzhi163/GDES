#include "stdafx.h"
#include "RcuNewDlg.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

IMPLEMENT_DYNAMIC(RcuNewDlg, RcuAcUiBaseDlg)

RcuNewDlg::RcuNewDlg(CWnd* pParent /*=NULL*/)
	: RcuAcUiBaseDlg(RcuNewDlg::IDD, pParent)
	, m_x(0)
	, m_y(0)
	, m_z(0)
	, m_length(0)
	, m_width(0)
	, m_height(0)
	, m_left(0)
	, m_right(0)
	, m_top(0)
	, m_bottom(0)
	, m_thick(0)
	, m_angle(0)
	, m_dist(0)
	, m_radius(0)
{

}

RcuNewDlg::~RcuNewDlg()
{
}

void RcuNewDlg::DoDataExchange(CDataExchange* pDX)
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
	DDX_Text(pDX, IDC_EDIT14, m_thick);
	DDX_Text(pDX, IDC_EDIT15, m_angle);
	DDX_Text(pDX, IDC_EDIT16, m_dist);
	DDX_Text(pDX, IDC_EDIT17, m_radius);
	DDX_Text(pDX, IDC_EDIT18, m_name);
}


BEGIN_MESSAGE_MAP(RcuNewDlg, RcuAcUiBaseDlg)
	ON_BN_CLICKED(IDOK, &RcuNewDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// RcuDlg 消息处理程序

void RcuNewDlg::OnBnClickedOk()
{
	//BeginEditorCommand();
	//AcGePoint3d pt;
	//ArxUtilHelper::PromptPt( _T( "\n请选择插入点坐标: " ), pt );
	//CompleteEditorCommand();
	UpdateData( TRUE );
	OnOK();
}

BOOL RcuNewDlg::OnInitDialog()
{
	RcuAcUiBaseDlg::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void RcuNewDlg::writeToDataLink(RockGateLink& rg_link, CoalSurfaceLink& cs_link)
{
	//提取石门数据
	rg_link.m_name = m_name;
	rg_link.m_pt = ArxUtilHelper::Point3dToString(AcGePoint3d(m_x, m_y, m_z));
	rg_link.m_length = m_length;
	rg_link.m_width = m_width;
	rg_link.m_height = m_height;
	rg_link.m_left = m_left;
	rg_link.m_right = m_right;
	rg_link.m_top = m_top;
	rg_link.m_bottom = m_bottom;
	rg_link.m_radius = m_radius;
	rg_link.m_dist = m_dist;

	//提取煤层数据
	cs_link.m_thick = m_thick;
	cs_link.m_angle = m_angle;
}
