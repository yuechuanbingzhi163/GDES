#include "stdafx.h"
#include "DataImportDlg.h"

#include "DataInitHelper.h"

IMPLEMENT_DYNAMIC( DataImportDlg, CDialog )

DataImportDlg::DataImportDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( DataImportDlg::IDD, pParent )
{

}

DataImportDlg::~DataImportDlg()
{
}

void DataImportDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
}


BEGIN_MESSAGE_MAP( DataImportDlg, CDialog )
    ON_BN_CLICKED( IDC_DATA_IMPOT_FIELD_BTN, &DataImportDlg::OnBnClickedDataImpotFieldBtn )
    ON_BN_CLICKED( IDC_DATA_IMPOT_STRING_LIST_BTN, &DataImportDlg::OnBnClickedDataImpotStringListBtn )
    ON_BN_CLICKED( IDC_DATA_IMPORT_INT_LIST_BTN, &DataImportDlg::OnBnClickedDataImportIntListBtn )
    ON_BN_CLICKED( IDC_DATA_IMPOT_OBJECT_LIST_BTN, &DataImportDlg::OnBnClickedDataImpotObjectListBtn )
    ON_BN_CLICKED( IDC_DATA_IMPORT_GLOBAL_INFO_BTN, &DataImportDlg::OnBnClickedDataImportGlobalInfoBtn )
END_MESSAGE_MAP()


// DataImportDlg 消息处理程序

void DataImportDlg::OnBnClickedDataImpotFieldBtn()
{
    CFileDialog  openDialog(
        TRUE,
        _T( "txt" ),
        NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
        _T( "txt文件(*.txt)|*.txt||" ) );

    if( IDOK == openDialog.DoModal() )
    {
        CString filePath = openDialog.GetPathName();
        GetDlgItem( IDC_DATA_INPUT_FIELD )->SetWindowText( filePath );

        enableControl( IDC_DATA_IMPOT_FIELD_BTN, _T( "请等待" ), false );
        DataInitHelper::InitDataField( filePath );
        enableControl( IDC_DATA_IMPOT_FIELD_BTN, _T( "导入" ), true );
        MessageBox( _T( "导入完毕!" ) );
    }
}

void DataImportDlg::OnBnClickedDataImpotStringListBtn()
{
    CFileDialog  openDialog(
        TRUE,
        _T( "txt" ),
        NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
        _T( "txt文件(*.txt)|*.txt||" ) );

    if( IDOK == openDialog.DoModal() )
    {
        CString filePath = openDialog.GetPathName();
        GetDlgItem( IDC_DATA_INPUT_STRING_LIST )->SetWindowText( filePath );

        enableControl( IDC_DATA_IMPOT_STRING_LIST_BTN, _T( "请等待" ), false );
        DataInitHelper::InitStrList( filePath );
        enableControl( IDC_DATA_IMPOT_STRING_LIST_BTN,  _T( "导入" ), true );
        MessageBox( _T( "导入完毕!" ) );
    }
}

void DataImportDlg::OnBnClickedDataImportIntListBtn()
{
    CFileDialog  openDialog(
        TRUE,
        _T( "txt" ),
        NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
        _T( "txt文件(*.txt)|*.txt||" ) );

    if( IDOK == openDialog.DoModal() )
    {
        CString filePath = openDialog.GetPathName();
        GetDlgItem( IDC_DATA_INPUT_INT_LIST )->SetWindowText( filePath );

        enableControl( IDC_DATA_IMPORT_INT_LIST_BTN,  _T( "请等待" ), false );
        DataInitHelper::InitIntStrList( filePath );
        enableControl( IDC_DATA_IMPORT_INT_LIST_BTN,  _T( "导入" ), true );
        MessageBox( _T( "导入完毕!" ) );
    }
}

void DataImportDlg::OnBnClickedDataImpotObjectListBtn()
{
    CFileDialog  openDialog(
        TRUE,
        _T( "txt" ),
        NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
        _T( "txt文件(*.txt)|*.txt||" ) );

    if( IDOK == openDialog.DoModal() )
    {
        CString filePath = openDialog.GetPathName();
        GetDlgItem( IDC_DATA_INPUT_OBJECT_LIST )->SetWindowText( filePath );

        enableControl( IDC_DATA_IMPORT_INT_LIST_BTN,  _T( "请等待" ), false );
        DataInitHelper::InitDataObjectList( filePath );
        enableControl( IDC_DATA_IMPORT_INT_LIST_BTN,  _T( "导入" ), true );
        MessageBox( _T( "导入完毕!" ) );
    }
}

void DataImportDlg::OnBnClickedDataImportGlobalInfoBtn()
{
    CFileDialog  openDialog(
        TRUE,
        _T( "txt" ),
        NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
        _T( "txt文件(*.txt)|*.txt||" ) );

    if( IDOK == openDialog.DoModal() )
    {
        CString filePath = openDialog.GetPathName();
        GetDlgItem( IDC_DATA_INPUT_GLOBAL_INFO )->SetWindowText( filePath );

        enableControl( IDC_DATA_IMPORT_GLOBAL_INFO_BTN,  _T( "请等待" ), false );
        DataInitHelper::InitSingleDataObject( filePath );
        enableControl( IDC_DATA_IMPORT_GLOBAL_INFO_BTN,  _T( "导入" ), true );
        MessageBox( _T( "导入完毕!" ) );
    }
}

void DataImportDlg::enableControl( long id, const CString& text, bool bEnable )
{
    CWnd* pWnd = GetDlgItem( id );
    if( pWnd != 0 )
    {
        pWnd->SetWindowText( text );
        pWnd->EnableWindow( bEnable );
        EnableWindow( bEnable ); // 禁用/启动对话框本身
    }
}