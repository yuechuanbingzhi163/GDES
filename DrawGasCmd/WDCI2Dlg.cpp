#include "stdafx.h"
#include "WDCI2Dlg.h"

#include "Tool.h"

#include "../MineGE/DataLink.h"
#include "../ArxHelper/HelperClass.h"
#include "../GasGE/DCI.h"

class WDCI2DataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "钻孔最大瓦斯涌出初速度" ), &m_velocity );
        linkDoubleData( _T( "钻孔最大钻屑量" ), &m_smax );
        linkDoubleData( _T( "R值指标" ), &m_r );
    }

public:
    double m_velocity;
    double m_smax;
    double m_r;
};

static WDCI2* CreateWDCI2( const AcGePoint3d& pt )
{
    return new WDCI2( pt );
}

IMPLEMENT_DYNAMIC( WDCI2Dlg, CAcUiDialog )

WDCI2Dlg::WDCI2Dlg( CWnd* pParent /*=NULL*/ )
    : CAcUiDialog( WDCI2Dlg::IDD, pParent )
    , m_xPos( 0 )
    , m_yPos( 0 )
    , m_zPos( 0 )
    , m_velocity( 5 )
    , m_smax( 2.3 )
    , m_r( 0.5 )
{

}

WDCI2Dlg::~WDCI2Dlg()
{
}

void WDCI2Dlg::DoDataExchange( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_WDCI2_PT_BTN, m_pickBtn );
    DDX_Text( pDX, IDC_WDCI2_X_POS, m_xPos );
    DDX_Text( pDX, IDC_WDCI2_Y_POS, m_yPos );
    DDX_Text( pDX, IDC_WDCI2_Z_POS, m_zPos );
    DDX_Text( pDX, IDC_WDCI2_VELOCITY, m_velocity );
    DDX_Text( pDX, IDC_WDCI2_S_MAX, m_smax );
    DDX_Text( pDX, IDC_WDCI2_R, m_r );
}


BEGIN_MESSAGE_MAP( WDCI2Dlg, CAcUiDialog )
    ON_BN_CLICKED( IDC_WDCI2_PT_BTN, &WDCI2Dlg::OnBnClickedWdci1PtBtn )
    ON_BN_CLICKED( IDOK, &WDCI2Dlg::OnBnClickedOk )
END_MESSAGE_MAP()

BOOL WDCI2Dlg::OnInitDialog()
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

void WDCI2Dlg::OnBnClickedOk()
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

void WDCI2Dlg::OnBnClickedWdci1PtBtn()
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

void WDCI2Dlg::readPropertyData()
{
    AcGePoint3d pt;
    GetInsertPoint( m_objId, pt );

    m_xPos = pt.x;
    m_yPos = pt.y;
    m_zPos = pt.z;

    WDCI2DataLink dl;
    dl.setDataSource( m_objId );
    dl.updateData( false );

    m_velocity = dl.m_velocity;
    m_smax = dl.m_smax;
    m_r = dl.m_r;
}

void WDCI2Dlg::writePropertyData()
{
    AcGePoint3d pt( m_xPos, m_yPos, m_zPos );
    SetInsertPoint( m_objId, pt );

    WDCI2DataLink dl;
    dl.setDataSource( m_objId );

    dl.m_velocity = m_velocity;
    dl.m_smax = m_smax;
    dl.m_r = m_r;

    dl.updateData( true );
}

AcDbObjectId WDCI2Dlg::createObject()
{
    AcDbObjectId objId;

    WDCI2* pWDCI2 = CreateWDCI2( AcGePoint3d( m_xPos, m_yPos, m_zPos ) );
    if( ArxUtilHelper::PostToModelSpace( pWDCI2 ) )
    {
        objId = pWDCI2->objectId();
        pWDCI2->close();
    }
    else
    {
        delete pWDCI2;
    }
    return objId;
}