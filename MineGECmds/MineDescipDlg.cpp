#include "stdafx.h"
#include "MineDescripDlg.h"

#include <iostream>
#include <fstream>
using namespace std;

#include "AcFStream.h"

IMPLEMENT_DYNAMIC(MineMainDescipDlg, CDialog)

static CString GetAppPathDir()
{
	TCHAR szModulePath[_MAX_PATH];
	GetModuleFileName( _hdllInstance, szModulePath, _MAX_PATH );

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	_tsplitpath( szModulePath, drive, dir, NULL, NULL );

	TCHAR szPath[_MAX_PATH] = {0};
	_tmakepath( szPath, drive, dir, NULL, NULL );

	return CString( szPath );
}

static CString BuildPath( const CString& dir, const CString& fileName )
{
	CString path;
	path.Format( _T( "%s%s" ), dir, fileName );
	return path;
}

MineMainDescipDlg::MineMainDescipDlg(CWnd* pParent /*=NULL*/)
: CDialog(MineMainDescipDlg::IDD, pParent)
{

}

MineMainDescipDlg::~MineMainDescipDlg()
{
}

void MineMainDescipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT2_LOCATION, m_EditCtrl);
	DDX_Control(pDX, IDC_RICHEDIT2_NUM, m_numEditCtrl);
}


BEGIN_MESSAGE_MAP(MineMainDescipDlg, CDialog)
	ON_BN_CLICKED(IDOK, &MineMainDescipDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// MineMainDescipDlg 消息处理程序

void MineMainDescipDlg::OnBnClickedOk()
{
	CString dataDirName = _T( "Datas\\" );
	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T( "locaDesc.txt" ) );


	AcOfstream outFile(fileName);
	if (!outFile) return;

	CString locaDesc;
	m_EditCtrl.GetWindowText(locaDesc);
	locaDesc.Replace(_T(" "), NULL);
	outFile <<  (LPWSTR)(LPCTSTR)locaDesc;

	outFile.close();

	dataDirName = _T( "Datas\\" );
	fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T( "numDesc.txt" ) );


	AcOfstream outFile2(fileName);
	if (!outFile2) return;

	CString numDesc;
	m_numEditCtrl.GetWindowText(numDesc);
	numDesc.Replace(_T(" "), NULL);
	outFile2 <<  (LPWSTR)(LPCTSTR)numDesc;

	outFile2.close();

	OnOK();
}

BOOL MineMainDescipDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CFont font;
	font.CreatePointFont(130,_T("宋体"),NULL);
	GetDlgItem(IDC_RICHEDIT2_LOCATION)->SetFont(&font);
	GetDlgItem(IDC_RICHEDIT2_NUM)->SetFont(&font);
	////不能输入太多字，否则在读写word的时候会崩溃
	m_EditCtrl.LimitText(_MAX_PATH / 2);
	m_numEditCtrl.LimitText(_MAX_PATH / 2);

	CString dataDirName = _T( "Datas\\" );
	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("locaDesc.txt") );

	AcIfstream inFile(fileName);
	CString locaDesc;
	if(!inFile) return FALSE;
	while( !inFile.eof() )
	{
		ACHAR /*charDesName[_MAX_PATH],*/ charLoca[_MAX_PATH];
		//ACHAR *charLoca = new ACHAR;
		inFile /*>> charDesName*/ >> charLoca;
		CString chDesc;
		chDesc.Format(_T("%s"),charLoca);
		locaDesc.Append(chDesc);
	}
	inFile.close();
	m_EditCtrl.SetWindowText(locaDesc);

	dataDirName = _T( "Datas\\" );
	fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("numDesc.txt") );

	AcIfstream inFile2(fileName);
	CString numDesc;
	if(!inFile2) return FALSE;
	while( !inFile2.eof() )
	{
		ACHAR /*charDesName[_MAX_PATH],*/ charLoca[_MAX_PATH];
		//ACHAR *charLoca = new ACHAR;
		inFile2 /*>> charDesName*/ >> charLoca;
		CString chDesc;
		chDesc.Format(_T("%s"),charLoca);
		numDesc.Append(chDesc);
	}
	inFile2.close();

	m_numEditCtrl.SetWindowText(numDesc);

	return TRUE;
}