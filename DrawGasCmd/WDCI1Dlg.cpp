#include "stdafx.h"
#include "WDCI1Dlg.h"

#include "Tool.h"

#include "../MineGE/DataLink.h"
#include "../ArxHelper/HelperClass.h"
#include "../GasGE/DCI.h"

class WDCI1DataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "钻屑解吸指标" ), &m_delta_h2 );
        linkDoubleData( _T( "钻孔最大钻屑量" ), &m_smax );
    }

public:
    double m_delta_h2;
    double m_smax;
};

static WDCI1* CreateWDCI1( const AcGePoint3d& pt )
{
    return new WDCI1( pt );
}

IMPLEMENT_DYNAMIC( WDCI1Dlg, CAcUiDialog )

WDCI1Dlg::WDCI1Dlg( CWnd* pParent /*=NULL*/ )
    : CAcUiDialog( WDCI1Dlg::IDD, pParent )
    , m_xPos( 0 )
    , m_yPos( 0 )
    , m_zPos( 0 )
    , m_delta_h2( 120 )
    , m_smax( 2.3 )
{

}

WDCI1Dlg::~WDCI1Dlg()
{
}

void WDCI1Dlg::DoDataExchange( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_WDCI1_PT_BTN, m_pickBtn );
    DDX_Text( pDX, IDC_WDCI1_X_POS, m_xPos );
    DDX_Text( pDX, IDC_WDCI1_Y_POS, m_yPos );
    DDX_Text( pDX, IDC_WDCI1_Z_POS, m_zPos );
    DDX_Text( pDX, IDC_WDCI1_DELTA_H2, m_delta_h2 );
    DDX_Text( pDX, IDC_WDCI1_S_MAX, m_smax );
}


BEGIN_MESSAGE_MAP( WDCI1Dlg, CAcUiDialog )
    ON_BN_CLICKED( IDC_WDCI1_PT_BTN, &WDCI1Dlg::OnBnClickedWdci1PtBtn )
    ON_BN_CLICKED( IDOK, &WDCI1Dlg::OnBnClickedOk )
END_MESSAGE_MAP()

BOOL WDCI1Dlg::OnInitDialog()
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

void WDCI1Dlg::OnBnClickedOk()
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

void WDCI1Dlg::OnBnClickedWdci1PtBtn()
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

void WDCI1Dlg::readPropertyData()
{
    AcGePoint3d pt;
    GetInsertPoint( m_objId, pt );

    m_xPos = pt.x;
    m_yPos = pt.y;
    m_zPos = pt.z;

    WDCI1DataLink dl;
    dl.setDataSource( m_objId );
    dl.updateData( false );

    m_delta_h2 = dl.m_delta_h2;
    m_smax = dl.m_smax;
}

void WDCI1Dlg::writePropertyData()
{
    AcGePoint3d pt( m_xPos, m_yPos, m_zPos );
    SetInsertPoint( m_objId, pt );

    WDCI1DataLink dl;
    dl.setDataSource( m_objId );

    dl.m_delta_h2 = m_delta_h2;
    dl.m_smax = m_smax;

    dl.updateData( true );
}

AcDbObjectId WDCI1Dlg::createObject()
{
    AcDbObjectId objId;

    WDCI1* pWDCI1 = CreateWDCI1( AcGePoint3d( m_xPos, m_yPos, m_zPos ) );
    if( ArxUtilHelper::PostToModelSpace( pWDCI1 ) )
    {
        objId = pWDCI1->objectId();
        pWDCI1->close();
    }
    else
    {
        delete pWDCI1;
    }
    return objId;
}