#include "stdafx.h"
#include "GPPDlg.h"

#include "Tool.h"

#include "../MineGE/DataLink.h"
#include "../ArxHelper/HelperClass.h"
#include "../GasGE/GPP.h"

class GPPDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "瓦斯压力值" ), &m_pressure );
        linkDoubleData( _T( "测点标高" ), &m_level );
        linkDoubleData( _T( "埋深" ), &m_height );
    }

public:
    double m_pressure;
    double m_level;
    double m_height;
};

static GPP* CreateGPP( const AcGePoint3d& pt )
{
    return new GPP( pt );
}

IMPLEMENT_DYNAMIC( GPPDlg, CAcUiDialog )

GPPDlg::GPPDlg( CWnd* pParent /*=NULL*/ )
    : CAcUiDialog( GPPDlg::IDD, pParent )
    , m_xPos( 0 )
    , m_yPos( 0 )
    , m_zPos( 0 )
    , m_pressure( 2.3 )
    , m_level( -600 )
    , m_height( 620 )
{

}

GPPDlg::~GPPDlg()
{
}

void GPPDlg::DoDataExchange( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_GPP_PT_BTN, m_pickBtn );
    DDX_Text( pDX, IDC_GPP_X_POS, m_xPos );
    DDX_Text( pDX, IDC_GPP_Y_POS, m_yPos );
    DDX_Text( pDX, IDC_GPP_Z_POS, m_zPos );
    DDX_Text( pDX, IDC_GPP_PRESSURE, m_pressure );
    DDX_Text( pDX, IDC_GPP_LEVEL, m_level );
    DDX_Text( pDX, IDC_GPP_HEIGHT, m_height );
}


BEGIN_MESSAGE_MAP( GPPDlg, CAcUiDialog )
    ON_BN_CLICKED( IDOK, &GPPDlg::OnBnClickedOk )
    ON_BN_CLICKED( IDC_GPP_PT_BTN, &GPPDlg::OnBnClickedGppPtBtn )
END_MESSAGE_MAP()


// GPPDlg 消息处理程序

BOOL GPPDlg::OnInitDialog()
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

void GPPDlg::OnBnClickedOk()
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

void GPPDlg::OnBnClickedGppPtBtn()
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

void GPPDlg::readPropertyData()
{
    AcGePoint3d pt;
    GetInsertPoint( m_objId, pt );

    m_xPos = pt.x;
    m_yPos = pt.y;
    m_zPos = pt.z;

    GPPDataLink dl;
    dl.setDataSource( m_objId );
    dl.updateData( false );

    m_pressure = dl.m_pressure;
    m_level = dl.m_level;
    m_height = dl.m_height;
}

void GPPDlg::writePropertyData()
{
    AcGePoint3d pt( m_xPos, m_yPos, m_zPos );
    SetInsertPoint( m_objId, pt );

    GPPDataLink dl;
    dl.setDataSource( m_objId );

    dl.m_pressure = m_pressure;
    dl.m_level = m_level;
    dl.m_height = m_height;
    dl.updateData( true );
}

AcDbObjectId GPPDlg::createObject()
{
    AcDbObjectId objId;

    GPP* pGPP = CreateGPP( AcGePoint3d( m_xPos, m_yPos, m_zPos ) );
    if( ArxUtilHelper::PostToModelSpace( pGPP ) )
    {
        objId = pGPP->objectId();
        pGPP->close();
    }
    else
    {
        delete pGPP;
    }
    return objId;
}