#include "stdafx.h"
#include "CDHDlg.h"

#include "Tool.h"

#include "../MineGE/DataLink.h"
#include "../ArxHelper/HelperClass.h"
#include "../GasGE/CDH.h"

class CDHDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkIntData( _T( "孔号" ), &m_num );
        linkDoubleData( _T( "地面标高" ), &m_h1 );
        linkDoubleData( _T( "煤层底板标高" ), &m_h2 );
        linkDoubleData( _T( "煤厚" ), &m_thick );
    }

public:
    int m_num;
    double m_h1;
    double m_h2;
    double m_thick;
};

static CDH* CreateCDH( const AcGePoint3d& pt )
{
    return new CDH( pt );
}

IMPLEMENT_DYNAMIC( CDHDlg, CAcUiDialog )

CDHDlg::CDHDlg( CWnd* pParent /*=NULL*/ )
    : CAcUiDialog( CDHDlg::IDD, pParent )
    , m_xPos( 0 )
    , m_yPos( 0 )
    , m_zPos( 0 )
    , m_num( 27 )
    , m_h1( 125.16 )
    , m_h2( -449.10 )
    , m_thick( 1.46 )
{

}

CDHDlg::~CDHDlg()
{
}

void CDHDlg::DoDataExchange( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_CDH_PT_BTN, m_pickBtn );
    DDX_Text( pDX, IDC_CDH_X_POS, m_xPos );
    DDX_Text( pDX, IDC_CDH_Y_POS, m_yPos );
    DDX_Text( pDX, IDC_CDH_Z_POS, m_zPos );

    DDX_Text( pDX, IDC_CDH_NUM, m_num );
    DDX_Text( pDX, IDC_CDH_H1, m_h1 );
    DDX_Text( pDX, IDC_CDH_H2, m_h2 );
    DDX_Text( pDX, IDC_CDH_THICKNESS, m_thick );
}


BEGIN_MESSAGE_MAP( CDHDlg, CAcUiDialog )
    ON_BN_CLICKED( IDOK, &CDHDlg::OnBnClickedOk )
    ON_BN_CLICKED( IDC_CDH_PT_BTN, &CDHDlg::OnBnClickedGopPtBtn )
END_MESSAGE_MAP()


BOOL CDHDlg::OnInitDialog()
{
    CAcUiDialog::OnInitDialog();

    m_pickBtn.AutoLoad();

    if( !m_objId.isNull() )
    {
        readPropertyData();
        UpdateData( FALSE );

        GetDlgItem( IDOK )->SetWindowText( _T( "修改" ) );
    }
    else
    {
        GetDlgItem( IDOK )->SetWindowText( _T( "添加" ) );
    }

    return TRUE;
}

void CDHDlg::OnBnClickedOk()
{
    if( !UpdateData( TRUE ) ) return;

    if( m_objId.isNull() )
    {
        m_objId = createObject();
        if( m_objId.isNull() )
        {
            MessageBox( _T( "添加失败" ) );
            OnOK();
            return;
        }
    }

    writePropertyData();
    OnOK();
}

void CDHDlg::OnBnClickedGopPtBtn()
{
    BeginEditorCommand();
    AcGePoint3d pt;
    ArxUtilHelper::PromptPt( _T( "请选择插入点坐标: " ), pt );
    CompleteEditorCommand();

    m_xPos = pt.x;
    m_yPos = pt.y;
    m_zPos = pt.z;
    UpdateData( FALSE );
}

void CDHDlg::readPropertyData()
{
    AcGePoint3d pt;
    GetInsertPoint( m_objId, pt );

    m_xPos = pt.x;
    m_yPos = pt.y;
    m_zPos = pt.z;

    CDHDataLink dl;
    dl.setDataSource( m_objId );
    dl.updateData( false );

    m_num = dl.m_num;
    m_h1 = dl.m_h1;
    m_h2 = dl.m_h2;
    m_thick = dl.m_thick;
}

void CDHDlg::writePropertyData()
{
    AcGePoint3d pt( m_xPos, m_yPos, m_zPos );
    SetInsertPoint( m_objId, pt );

    CDHDataLink dl;
    dl.setDataSource( m_objId );

    dl.m_num = m_num;
    dl.m_h1 = m_h1;
    dl.m_h2 = m_h2;
    dl.m_thick = m_thick;

    dl.updateData( true );
}

AcDbObjectId CDHDlg::createObject()
{
    AcDbObjectId objId;

    CDH* pCDH = CreateCDH( AcGePoint3d( m_xPos, m_yPos, m_zPos ) );
    if( ArxUtilHelper::PostToModelSpace( pCDH ) )
    {
        objId = pCDH->objectId();
        pCDH->close();
    }
    else
    {
        delete pCDH;
    }
    return objId;
}

