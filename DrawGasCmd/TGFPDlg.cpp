#include "stdafx.h"
#include "TGFPDlg.h"

#include "Tool.h"

#include "../MineGE/DataLink.h"
#include "../ArxHelper/HelperClass.h"
#include "../GasGE/GFP.h"

class TGFPDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "绝对瓦斯涌出量" ), &m_aq );
        linkStringData( _T( "回采年月" ), &m_dt );
    }

public:
    double m_aq;
    CString m_dt;
};

static TGFP* CreateTGFP( const AcGePoint3d& pt )
{
    return new TGFP( pt );
}

IMPLEMENT_DYNAMIC( TGFPDlg, CAcUiDialog )

TGFPDlg::TGFPDlg( CWnd* pParent /*=NULL*/ )
    : CAcUiDialog( TGFPDlg::IDD, pParent )
    , m_xPos( 0 )
    , m_yPos( 0 )
    , m_zPos( 0 )
    , m_aq( 1.8 )
    , m_dt( _T( "03.02" ) )
{

}

TGFPDlg::~TGFPDlg()
{
}

void TGFPDlg::DoDataExchange( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_TGFP_PT_BTN, m_pickBtn );
    DDX_Text( pDX, IDC_TGFP_X_POS, m_xPos );
    DDX_Text( pDX, IDC_TGFP_Y_POS, m_yPos );
    DDX_Text( pDX, IDC_TGFP_Z_POS, m_zPos );

    DDX_Text( pDX, IDC_TGFP_AQ, m_aq );
    DDX_Text( pDX, IDC_TGFP_DATE, m_dt );
}


BEGIN_MESSAGE_MAP( TGFPDlg, CAcUiDialog )
    ON_BN_CLICKED( IDOK, &TGFPDlg::OnBnClickedOk )
    ON_BN_CLICKED( IDC_TGFP_PT_BTN, &TGFPDlg::OnBnClickedGopPtBtn )
END_MESSAGE_MAP()


BOOL TGFPDlg::OnInitDialog()
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

void TGFPDlg::OnBnClickedOk()
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

void TGFPDlg::OnBnClickedGopPtBtn()
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

void TGFPDlg::readPropertyData()
{
    AcGePoint3d pt;
    GetInsertPoint( m_objId, pt );

    m_xPos = pt.x;
    m_yPos = pt.y;
    m_zPos = pt.z;

    TGFPDataLink dl;
    dl.setDataSource( m_objId );
    dl.updateData( false );

    m_aq = dl.m_aq;
    m_dt = dl.m_dt;
}

void TGFPDlg::writePropertyData()
{
    AcGePoint3d pt( m_xPos, m_yPos, m_zPos );
    SetInsertPoint( m_objId, pt );

    TGFPDataLink dl;
    dl.setDataSource( m_objId );

    dl.m_aq = m_aq;
    dl.m_dt = m_dt;

    dl.updateData( true );
}

AcDbObjectId TGFPDlg::createObject()
{
    AcDbObjectId objId;

    TGFP* pTGFP = CreateTGFP( AcGePoint3d( m_xPos, m_yPos, m_zPos ) );
    if( ArxUtilHelper::PostToModelSpace( pTGFP ) )
    {
        objId = pTGFP->objectId();
        pTGFP->close();
    }
    else
    {
        delete pTGFP;
    }
    return objId;
}

