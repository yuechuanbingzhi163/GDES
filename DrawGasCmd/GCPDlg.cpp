#include "stdafx.h"
#include "GCPDlg.h"

#include "Tool.h"

#include "../MineGE/DataLink.h"
#include "../ArxHelper/HelperClass.h"
#include "../GasGE/GCP.h"

class GCPDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "瓦斯含量" ), &m_gas );
        linkDoubleData( _T( "测点标高" ), &m_level );
        linkDoubleData( _T( "埋深" ), &m_height );
    }

public:
    double m_gas;
    double m_level;
    double m_height;
};

static GCP* CreateGCP( const AcGePoint3d& pt )
{
    return new GCP( pt );
}

IMPLEMENT_DYNAMIC( GCPDlg, CAcUiDialog )

GCPDlg::GCPDlg( CWnd* pParent /*=NULL*/ )
    : CAcUiDialog( GCPDlg::IDD, pParent )
    , m_xPos( 0 )
    , m_yPos( 0 )
    , m_zPos( 0 )
    , m_gas( 12.30 )
    , m_level( -610.24 )
    , m_height( 713.85 )
{

}

GCPDlg::~GCPDlg()
{
}

void GCPDlg::DoDataExchange( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_GCP_PT_BTN, m_pickBtn );
    DDX_Text( pDX, IDC_GCP_X_POS, m_xPos );
    DDX_Text( pDX, IDC_GCP_Y_POS, m_yPos );
    DDX_Text( pDX, IDC_GCP_Z_POS, m_zPos );
    DDX_Text( pDX, IDC_GCP_GAS, m_gas );
    DDX_Text( pDX, IDC_GCP_LEVEL, m_level );
    DDX_Text( pDX, IDC_GCP_HEIGHT, m_height );
}


BEGIN_MESSAGE_MAP( GCPDlg, CAcUiDialog )
    ON_BN_CLICKED( IDOK, &GCPDlg::OnBnClickedOk )
    ON_BN_CLICKED( IDC_GCP_PT_BTN, &GCPDlg::OnBnClickedGcpPtBtn )
END_MESSAGE_MAP()

BOOL GCPDlg::OnInitDialog()
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

void GCPDlg::OnBnClickedOk()
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

void GCPDlg::readPropertyData()
{
    AcGePoint3d pt;
    GetInsertPoint( m_objId, pt );

    m_xPos = pt.x;
    m_yPos = pt.y;
    m_zPos = pt.z;

    GCPDataLink dl;
    dl.setDataSource( m_objId );
    dl.updateData( false );

    m_gas = dl.m_gas;
    m_level = dl.m_level;
    m_height = dl.m_height;
}

void GCPDlg::writePropertyData()
{
    AcGePoint3d pt( m_xPos, m_yPos, m_zPos );
    SetInsertPoint( m_objId, pt );

    GCPDataLink dl;
    dl.setDataSource( m_objId );

    dl.m_gas = m_gas;
    dl.m_level = m_level;
    dl.m_height = m_height;
    dl.updateData( true );
}

AcDbObjectId GCPDlg::createObject()
{
    AcDbObjectId objId;

    GCP* pGCP = CreateGCP( AcGePoint3d( m_xPos, m_yPos, m_zPos ) );
    if( ArxUtilHelper::PostToModelSpace( pGCP ) )
    {
        objId = pGCP->objectId();
        pGCP->close();
    }
    else
    {
        delete pGCP;
    }
    return objId;
}

void GCPDlg::OnBnClickedGcpPtBtn()
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
