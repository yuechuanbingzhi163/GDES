#include "stdafx.h"
#include "InputDlg.h"

IMPLEMENT_DYNAMIC( InputDlg, CDialog )

InputDlg::InputDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( InputDlg::IDD, pParent ), m_str( _T( "" ) )
{

}

InputDlg::~InputDlg()
{
}

void InputDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_INPUT_EDIT, m_str );
}

BEGIN_MESSAGE_MAP( InputDlg, CDialog )
    ON_BN_CLICKED( IDOK, &InputDlg::OnBnClickedOk )
END_MESSAGE_MAP()

void InputDlg::OnBnClickedOk()
{
    UpdateData( TRUE ); // 更新控件内容
    m_str.Trim(); // 去除空白
    OnOK();
}
BOOL InputDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    GetDlgItem( IDC_INPUT_EDIT )->SetFocus();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
