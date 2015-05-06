// EditPumpDB.cpp : 实现文件
//

#include "stdafx.h"
#include "EditPumpDBDlg.h"
#include "EditPumpDBDlg.h"


IMPLEMENT_DYNAMIC(EditPumpDBDlg, CDialog)

EditPumpDBDlg::EditPumpDBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(EditPumpDBDlg::IDD, pParent)
	, m_btype(FALSE)
	, m_bweight(FALSE)
	, m_bspeed(FALSE)
	, m_bmaxp(FALSE)
	, m_bfactory(FALSE)
	, m_blength(FALSE)
	, m_bheight(FALSE)
	, m_bweidth(FALSE)
	, m_bminabsp(FALSE)
	, m_babsp(FALSE)
	, m_bpower(FALSE)
	, m_bmaxq(FALSE)
	, m_ball(FALSE)
	, m_type(_T(""))
	, m_weight(_T(""))
	, m_speed(_T(""))
	, m_maxp(_T(""))
	, m_factory(_T(""))
	, m_length(_T(""))
	, m_height(_T(""))
	, m_weidth(_T(""))
	, m_absp(_T(""))
	, m_power(_T(""))
	, m_maxq(_T(""))
	, m_minabsp(_T(""))
	, m_itemsNum(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_GASSYS_ICON);
}

EditPumpDBDlg::~EditPumpDBDlg()
{
}

void EditPumpDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_PUMP_NAME_CHECK, m_btype);
	DDX_Check(pDX, IDC_PUMP_WEIGHT_CHECK, m_bweight);
	DDX_Check(pDX, IDC_PUMP_SPEED_CHECK, m_bspeed);
	DDX_Check(pDX, IDC_PUMP_MAXP_CHECK, m_bmaxp);
	DDX_Check(pDX, IDC_PUMP_FACTORY_CHECK, m_bfactory);
	DDX_Check(pDX, IDC_PUMP_LENGTH_CHECK, m_blength);
	DDX_Check(pDX, IDC_PUMP_HEIGHT_CHECK, m_bheight);
	DDX_Check(pDX, IDC_PUMP_WEIDTH_CHECK, m_bweidth);
	DDX_Check(pDX, IDC_PUMP_MINABSP_CHECK, m_bminabsp);
	DDX_Check(pDX, IDC_PUMP_ABSP_CHECK, m_babsp);
	DDX_Check(pDX, IDC_PUMP_POWER_CHECK, m_bpower);
	DDX_Check(pDX, IDC_PUMP_MAXQ_CHECK, m_bmaxq);
	DDX_Check(pDX, IDC_ALL_PUMP_CHECK, m_ball);
	DDX_Text(pDX, IDC_FIND_PUMP_NAME_EDIT, m_type);
	DDX_Text(pDX, IDC_FIND_PUMP_WEIGHT_EDIT, m_weight);
	DDX_Text(pDX, IDC_FIND_PUMP_SPEED_EDIT, m_speed);
	DDX_Text(pDX, IDC_FIND_PUMP_MAXP_EDIT, m_maxp);
	DDX_Text(pDX, IDC_FIND_PUMP_FACTORY_EDIT, m_factory);
	DDX_Text(pDX, IDC_FIND_PUMP_LENGTH_EDIT, m_length);
	DDX_Text(pDX, IDC_FIND_PUMP_HEIGHT_EDIT, m_height);
	DDX_Text(pDX, IDC_FIND_PUMP_WEIDTH_EDIT, m_weidth);
	DDX_Text(pDX, IDC_FIND_PUMP_ABSP_EDIT, m_absp);
	DDX_Text(pDX, IDC_FIND_PUMP_POWER_EDIT, m_power);
	DDX_Text(pDX, IDC_FIND_PUMP_MAXQ_EDIT, m_maxq);
	DDX_Text(pDX, IDC_FIND_PUMP_MINABSP_EDIT, m_minabsp);
	DDX_Text(pDX, IDC_PUMP_ITEMSNUM_EDIT, m_itemsNum);
	DDX_Control(pDX, IDC_FIND_PUMP_RET_LIST, m_listCtrl);
}


BEGIN_MESSAGE_MAP(EditPumpDBDlg, CDialog)
	ON_BN_CLICKED(IDC_ALL_PUMP_CHECK, &EditPumpDBDlg::OnBnClickedAllPumpCheck)
	ON_BN_CLICKED(IDC_PUMP_NAME_CHECK, &EditPumpDBDlg::OnBnClickedPumpNameCheck)
	ON_BN_CLICKED(IDC_PUMP_LENGTH_CHECK, &EditPumpDBDlg::OnBnClickedPumpLengthCheck)
	ON_BN_CLICKED(IDC_PUMP_ABSP_CHECK, &EditPumpDBDlg::OnBnClickedPumpAbspCheck)
	ON_BN_CLICKED(IDC_PUMP_WEIGHT_CHECK, &EditPumpDBDlg::OnBnClickedPumpWeightCheck)
	ON_BN_CLICKED(IDC_PUMP_HEIGHT_CHECK, &EditPumpDBDlg::OnBnClickedPumpHeightCheck)
	ON_BN_CLICKED(IDC_PUMP_POWER_CHECK, &EditPumpDBDlg::OnBnClickedPumpPowerCheck)
	ON_BN_CLICKED(IDC_PUMP_SPEED_CHECK, &EditPumpDBDlg::OnBnClickedPumpSpeedCheck)
	ON_BN_CLICKED(IDC_PUMP_WEIDTH_CHECK, &EditPumpDBDlg::OnBnClickedPumpWeidthCheck)
	ON_BN_CLICKED(IDC_PUMP_MAXQ_CHECK, &EditPumpDBDlg::OnBnClickedPumpMaxqCheck)
	ON_BN_CLICKED(IDC_PUMP_MAXP_CHECK, &EditPumpDBDlg::OnBnClickedPumpMaxpCheck)
	ON_BN_CLICKED(IDC_PUMP_MINABSP_CHECK, &EditPumpDBDlg::OnBnClickedPumpMinabspCheck)
	ON_BN_CLICKED(IDC_PUMP_FACTORY_CHECK, &EditPumpDBDlg::OnBnClickedPumpFactoryCheck)
	ON_BN_CLICKED(IDC_EXIT_BUTTON, &EditPumpDBDlg::OnBnClickedExitButton)
	ON_BN_CLICKED(IDC_UPDATE_PUMPDB_BUTTON, &EditPumpDBDlg::OnBnClickedUpdatePumpdbButton)
	ON_BN_CLICKED(ID_FIND_PUMP, &EditPumpDBDlg::OnBnClickedFindPump)
END_MESSAGE_MAP()

BOOL EditPumpDBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	OnInitListCtrl();
	return TRUE;
}


void EditPumpDBDlg::OnBnClickedAllPumpCheck()
{
	UpdateData(TRUE);
	m_btype = m_bweight = m_bspeed = m_bmaxp
		= m_bfactory = m_blength =m_bheight
		= m_bweidth = m_bminabsp = m_babsp
		= m_bpower = m_bmaxq = m_ball;
	UpdateData(FALSE);
}

void EditPumpDBDlg::SetAllCheckBox()
{
	UpdateData(TRUE);
	m_ball = m_btype && m_bweight && m_bspeed && m_bmaxp
		&& m_bfactory && m_blength && m_bheight
		&& m_bweidth && m_bminabsp && m_babsp
		&& m_bpower && m_bmaxq;
	UpdateData(FALSE);
}

void EditPumpDBDlg::OnBnClickedPumpNameCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpLengthCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpAbspCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpWeightCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpHeightCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpPowerCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpSpeedCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpWeidthCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpMaxqCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpMaxpCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpMinabspCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnBnClickedPumpFactoryCheck()
{
	SetAllCheckBox();
}

void EditPumpDBDlg::OnInitListCtrl()
{
	//间隔线和行选中
	m_listCtrl.SetExtendedStyle(m_listCtrl.GetExtendedStyle() |
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	//表头
	m_listCtrl.InsertColumn(1,_T("类型"),LVCFMT_CENTER);  
	m_listCtrl.InsertColumn(2,_T("泵重"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(3,_T("泵长"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(4,_T("泵宽"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(5,_T("泵高"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(6,_T("转速"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(7,_T("吸入绝压"),LVCFMT_CENTER);  
	m_listCtrl.InsertColumn(8,_T("极限压力"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(9,_T("最大气量"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(10,_T("电机功率"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(11,_T("最低吸入绝压"),LVCFMT_CENTER);
	m_listCtrl.InsertColumn(12,_T("生产厂家"),LVCFMT_CENTER);

	CRect rect;  
	m_listCtrl.GetClientRect(rect); //获得当前客户区信息  
	m_listCtrl.SetColumnWidth(0, 2.5*rect.Width() / 23); //设置列的宽度。  
	m_listCtrl.SetColumnWidth(1, 1.2*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(2, 1.2*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(3, 1.2*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(4, 1.2*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(5, 1.2*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(6, 1.5*rect.Width() / 23);   
	m_listCtrl.SetColumnWidth(7, 1.5*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(8, 1.5*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(9, 1.5*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(10,2.5*rect.Width() / 23);  
	m_listCtrl.SetColumnWidth(11, 6*rect.Width() / 23);  

	CFont *cFont = new CFont;  

	cFont->CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("宋体")); 
	m_listCtrl.SetFont(cFont,true);//设置字体大小
	//m_listCtrl.SetTextBkColor(RGB(185,193,181));//设置背景颜色

	//m_listCtrl.SetTextColor(RGB(80,80,80));//设置字体颜色
	//COLORREF color = RGB(185,193,181); 
	//m_listCtrl.SetBkColor(color); 
}
void EditPumpDBDlg::OnBnClickedExitButton()
{
	CDialog::OnCancel();
}

void EditPumpDBDlg::OnBnClickedUpdatePumpdbButton()
{
	// TODO: 在此添加控件通知处理程序代码
}

void EditPumpDBDlg::OnBnClickedFindPump()
{
	// TODO: 在此添加控件通知处理程序代码
}
