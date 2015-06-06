#include "stdafx.h"
#include "PPPDlg.h"

#include "Tool.h"

#include "../MineGE/DataLink.h"
#include "../ArxHelper/HelperClass.h"
#include "../GasGE/PPP.h"

class PPPDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "突出煤岩量" ), &m_coal );
        linkDoubleData( _T( "涌出瓦斯量" ), &m_gas );
        linkDoubleData( _T( "标高" ), &m_level );
        linkStringData( _T( "发生年月" ), &m_dt );
    }

public:
    double m_coal;
    double m_gas;
    double m_level;
    CString m_dt;
};

enum PPP_TYPE
{
    PPP_SMALL    = 0,
    PPP_MID      = 1,
    PPP_LARGE    = 2,
    PPP_LARGER   = 3
};

static PPP* CreatePPP( const AcGePoint3d& pt )
{
    return new PPP( pt );
}

IMPLEMENT_DYNAMIC( PPPDlg, CAcUiDialog )

PPPDlg::PPPDlg( CWnd* pParent /*=NULL*/ )
    : CAcUiDialog( PPPDlg::IDD, pParent )
    , m_xPos( 0 )
    , m_yPos( 0 )
    , m_zPos( 0 )
    , m_coal( 20 )
    , m_gas( 2000 )
    , m_level( -568 )
    , m_dt( _T( "02.02" ) )
{

}

PPPDlg::~PPPDlg()
{
}

void PPPDlg::DoDataExchange( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_PPP_PT_BTN, m_pickBtn );
    DDX_Text( pDX, IDC_PPP_X_POS, m_xPos );
    DDX_Text( pDX, IDC_PPP_Y_POS, m_yPos );
    DDX_Text( pDX, IDC_ppP_Z_POS, m_zPos );
    DDX_Text( pDX, IDC_PPP_COAL_MINE, m_coal );
    DDX_Text( pDX, IDC_PPP_GAS, m_gas );
    DDX_Text( pDX, IDC_PPP_LEVEL, m_level );
    DDX_Text( pDX, IDC_PPP_DATE, m_dt );
}


BEGIN_MESSAGE_MAP( PPPDlg, CAcUiDialog )
    ON_BN_CLICKED( IDOK, &PPPDlg::OnBnClickedOk )
    ON_BN_CLICKED( IDC_PPP_PT_BTN, &PPPDlg::OnBnClickedPppPtBtn )
END_MESSAGE_MAP()


BOOL PPPDlg::OnInitDialog()
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

void PPPDlg::OnBnClickedOk()
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

void PPPDlg::OnBnClickedPppPtBtn()
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

void PPPDlg::readPropertyData()
{
    AcGePoint3d pt;
    GetInsertPoint( m_objId, pt );

    m_xPos = pt.x;
    m_yPos = pt.y;
    m_zPos = pt.z;

    PPPDataLink dl;
    dl.setDataSource( m_objId );
    dl.updateData( false );

    m_coal = dl.m_coal;
    m_gas = dl.m_gas;
    m_level = dl.m_level;
    m_dt = dl.m_dt;
}

void PPPDlg::writePropertyData()
{
    AcGePoint3d pt( m_xPos, m_yPos, m_zPos );
    SetInsertPoint( m_objId, pt );

    PPPDataLink dl;
    dl.setDataSource( m_objId );

    dl.m_coal = m_coal;
    dl.m_gas = m_gas;
    dl.m_level = m_level;
    dl.m_dt = m_dt;

    dl.updateData( true );
}

AcDbObjectId PPPDlg::createObject()
{
    AcDbObjectId objId;

    PPP* pPPP = CreatePPP( AcGePoint3d( m_xPos, m_yPos, m_zPos ) );
    if( ArxUtilHelper::PostToModelSpace( pPPP ) )
    {
        objId = pPPP->objectId();
        pPPP->close();
    }
    else
    {
        delete pPPP;
    }
    return objId;
}