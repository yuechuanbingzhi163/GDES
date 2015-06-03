#include "stdafx.h"
#include "GasBaseAcesDlg.h"
#include "Resource.h"

CString GasBaseAcesDlg::GetAppPathDir()
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

CString GasBaseAcesDlg::BuildPath( const CString& dir, const CString& fileName )
{
	CString path;
	path.Format( _T( "%s%s" ), dir, fileName );
	return path;
}

IMPLEMENT_DYNAMIC(GasBaseAcesDlg, CDialog)

GasBaseAcesDlg::GasBaseAcesDlg( UINT nIDTemplate,CWnd* pParent /*= NULL*/ )
: CDialog(nIDTemplate, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_GASSYS_ICON);
}

GasBaseAcesDlg::~GasBaseAcesDlg()
{
}

void GasBaseAcesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GasBaseAcesDlg, CDialog)
END_MESSAGE_MAP()

BOOL GasBaseAcesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	EnableToolTips(TRUE);
	m_tt.Create(this);
	m_tt.Activate(TRUE);

	return TRUE;
}

BOOL GasBaseAcesDlg::PreTranslateMessage( MSG* pMsg )
{
	if (NULL != m_tt.GetSafeHwnd())
	{
		m_tt.RelayEvent(pMsg);
	}

	return CDialog::PreTranslateMessage(pMsg);

}

void GasBaseAcesDlg::SetToolTip( int itemID,const CString& tooltip )
{
	CWnd* pW=GetDlgItem(itemID);//得到控件的指针
	m_tt.AddTool(pW,tooltip);//为此控件添加tip
}
//BOOL GasBaseAcesDlg::readDataFromFile( const CString& txtName, AcStringArray& datasArray )
//{
//	CString dataDirName = _T( "Datas\\BaseTemp\\" );
//	CString vfileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),txtName );
//
//	AcIfstream inFile(vfileName);
//	if(!inFile) return FALSE;
//	datasArray.removeAll();
//	while( !inFile.eof() )
//	{
//		ACHAR name[_MAX_PATH], data[_MAX_PATH];
//		inFile >> name >> data;
//		if(inFile.fail()) break;
//		CString strFName,strData;
//		strFName.Format(_T("%s"),name);
//		strData.Format(_T("%s"),data);
//		datasArray.append(data);
//	}
//	inFile.close();
//	return TRUE;
//}
//
//BOOL GasBaseAcesDlg::WriteDataToFile( const CString& txtName, const AcStringArray& nameArray, const AcStringArray& datasArray )
//{
//	CString dataDirName = _T( "Datas\\BaseTemp\\" );
//	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),txtName );
//
//	AcOfstream outFile(fileName);
//	if (!outFile) return FALSE;
//
//	for(int i = 0; i < nameArray.length(); i++)
//	{
//		outFile << nameArray[i] << _T("\t")  << datasArray[i].kACharPtr() << _T("\n");
//	}
//	outFile.close();
//	return TRUE;
//}

