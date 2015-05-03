// regMachineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "regMachine.h"
#include "regMachineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegMachineDlg dialog

CRegMachineDlg::CRegMachineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegMachineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegMachineDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CRegMachineDlg::~CRegMachineDlg()
{
}

void CRegMachineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegMachineDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRegMachineDlg, CDialog)
	//{{AFX_MSG_MAP(CRegMachineDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GET_ID, OnGetId)
	ON_BN_CLICKED(IDC_GET_REG, OnGetReg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegMachineDlg message handlers

BOOL CRegMachineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRegMachineDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRegMachineDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRegMachineDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRegMachineDlg::OnGetId() 
{
	// 获取硬件信息
	if ( !m_hWare.GetHDNum( &m_dInfo ) )
	{
		m_hWare.GetBiosNum( &m_dInfo );
	}
	
	// 把硬编码转换成数字ID
	m_strUID = m_reg.DisPoseID( m_dInfo.szSystemInfo );
	
	// 设置用户ID
	SetDlgItemText( IDC_EDIT_USERID, m_strUID );
}

void CRegMachineDlg::OnGetReg() 
{
	UpdateData();

	CString userID;
	CString useDate;

	// 存放分解出来后的注册码.
	CString strNum1 = "";  
	CString strNum2 = "";
	CString strNum3 = "";
	CString strNum4 = "";

	GetDlgItemText(IDC_EDIT_USERID,userID);

	if ( userID.IsEmpty() )
	{
		AfxMessageBox( "请输入用户名!" );
		return;
	}
	else if ( userID.GetLength() < 8 )
	{
		AfxMessageBox( "用户名应该为8位数字" );
		return;
	}

	useDate = "4";
	userID = userID + useDate;

	m_strReg = m_reg.GetRegNum( userID );

	strNum1 = m_strReg.Left( 4 );
	strNum2 = m_strReg.Mid( 4, m_strReg.GetLength() );
    strNum2 = strNum2.Left( 4 );
	strNum3 = m_strReg.Mid( 8, m_strReg.GetLength() );
	strNum3 = strNum3.Left( 4 );
	strNum4 = m_strReg.Mid( 12, m_strReg.GetLength() );

	SetDlgItemText(IDC_EDIT_REGNUM1,strNum1+strNum2+strNum3+strNum4);
}
