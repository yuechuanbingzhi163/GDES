#include "stdafx.h"
#include "RcuNewDlg.h"

// RcuDlg 对话框

IMPLEMENT_DYNAMIC(RcuNewDlg, CAcUiDialog)

RcuNewDlg::RcuNewDlg(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(RcuNewDlg::IDD, pParent)
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
	CAcUiDialog::DoDataExchange(pDX);
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
}


BEGIN_MESSAGE_MAP(RcuNewDlg, CAcUiDialog)
	ON_BN_CLICKED(IDOK, &RcuNewDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// RcuDlg 消息处理程序

void RcuNewDlg::OnBnClickedOk()
{
	//BeginEditorCommand();
	//AcGePoint3d pt;
	//ArxUtilHelper::PromptPt( _T( "\n请选择插入点坐标: " ), pt );
	//CompleteEditorCommand();

	UpdateData( FALSE );

	OnOK();
}

BOOL RcuNewDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}