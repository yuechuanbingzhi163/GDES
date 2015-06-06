#include "stdafx.h"
#include "CSDCIDlg.h"
#include "Tool.h"

#include "../MineGE/DataLink.h"
#include "../ArxHelper/HelperClass.h"
#include "../GasGE/DCI.h"

class CSDCIDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "瓦斯放散初速度" ), &m_velocity );
        linkDoubleData( _T( "煤的坚固性系数" ), &m_coeff );
    }

public:
    double m_velocity;
    double m_coeff;
};

static CSDCI* CreateCSDCI( const AcGePoint3d& pt )
{
    return new CSDCI( pt );
}

IMPLEMENT_DYNAMIC( CSDCIDlg, CAcUiDialog )

CSDCIDlg::CSDCIDlg( CWnd* pParent /*=NULL*/ )
    : CAcUiDialog( CSDCIDlg::IDD, pParent )
    , m_xPos( 0 )
    , m_yPos( 0 )
    , m_zPos( 0 )
    , m_velocity( 0 )
    , m_coeff( 0 )
{

}

CSDCIDlg::~CSDCIDlg()
{
}

void CSDCIDlg::DoDataExchange( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_CSDCI_PT_BTN, m_pickBtn );
    DDX_Text( pDX, IDC_CSDCI_X_POS, m_xPos );
    DDX_Text( pDX, IDC_CSDCI_Y_POS, m_yPos );
    DDX_Text( pDX, IDC_CSDCI_Z_POS, m_zPos );
    DDX_Text( pDX, IDC_CSDCI_VELOCITY, m_velocity );
    DDX_Text( pDX, IDC_CSDCI_COEFF, m_coeff );
}


BEGIN_MESSAGE_MAP( CSDCIDlg, CAcUiDialog )
    ON_BN_CLICKED( IDOK, &CSDCIDlg::OnBnClickedOk )
    ON_BN_CLICKED( IDC_CSDCI_PT_BTN, &CSDCIDlg::OnBnClickedCsdciPtBtn )
END_MESSAGE_MAP()


BOOL CSDCIDlg::OnInitDialog()
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

void CSDCIDlg::OnBnClickedOk()
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

void CSDCIDlg::OnBnClickedCsdciPtBtn()
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

void CSDCIDlg::readPropertyData()
{
    AcGePoint3d pt;
    GetInsertPoint( m_objId, pt );

    m_xPos = pt.x;
    m_yPos = pt.y;
    m_zPos = pt.z;

    CSDCIDataLink dl;
    dl.setDataSource( m_objId );
    dl.updateData( false );

    m_velocity = dl.m_velocity;
    m_coeff = dl.m_coeff;
}

void CSDCIDlg::writePropertyData()
{
    AcGePoint3d pt( m_xPos, m_yPos, m_zPos );
    SetInsertPoint( m_objId, pt );

    CSDCIDataLink dl;
    dl.setDataSource( m_objId );

    dl.m_velocity = m_velocity;
    dl.m_coeff = m_coeff;

    dl.updateData( true );
}

AcDbObjectId CSDCIDlg::createObject()
{
    AcDbObjectId objId;

    CSDCI* pCSDCI = CreateCSDCI( AcGePoint3d( m_xPos, m_yPos, m_zPos ) );
    if( ArxUtilHelper::PostToModelSpace( pCSDCI ) )
    {
        objId = pCSDCI->objectId();
        pCSDCI->close();
    }
    else
    {
        delete pCSDCI;
    }
    return objId;
}