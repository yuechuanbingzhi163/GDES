#include "stdafx.h"
#include "GOPDlg.h"

#include "Tool.h"

#include "../MineGE/DataLink.h"
#include "../ArxHelper/HelperClass.h"
#include "../GasGE/GOP.h"

class GOPDataLink : public DataLink
{
protected:
    virtual void regDatas()
    {
        linkDoubleData( _T( "突出煤量" ), &m_coal );
        linkDoubleData( _T( "涌出瓦斯总量" ), &m_gas );
        linkDoubleData( _T( "标高" ), &m_level );
        linkStringData( _T( "突出年月日" ), &m_dt );
    }

public:
    double m_coal;
    double m_gas;
    double m_level;
    CString m_dt;
};

enum GOP_TYPE
{
    GOP_SMALL    = 0,
    GOP_MID      = 1,
    GOP_LARGE    = 2,
    GOP_LARGER   = 3
};

static GOP* CreateGOP( GOP_TYPE type, const AcGePoint3d& pt )
{
    GOP* pGOP = 0;
    switch( type )
    {
    case GOP_SMALL:
        pGOP = new SmallGOP( pt );
        break;

    case GOP_MID:
        pGOP = new MidGOP( pt );
        break;

    case GOP_LARGE:
        pGOP = new LargeGOP( pt );
        break;

    case GOP_LARGER:
        pGOP = new LargerGOP( pt );
        break;
    }
    return pGOP;
}

static int GetGOPType( const AcDbObjectId& objId )
{
    int type = -1;
    if( ArxUtilHelper::IsEqualType( _T( "SmallGOP" ), objId ) )
    {
        type = GOP_SMALL;
    }
    else if( ArxUtilHelper::IsEqualType( _T( "MidGOP" ), objId ) )
    {
        type = GOP_MID;
    }
    else if( ArxUtilHelper::IsEqualType( _T( "LargeGOP" ), objId ) )
    {
        type = GOP_LARGE;
    }
    else if( ArxUtilHelper::IsEqualType( _T( "LargerGOP" ), objId ) )
    {
        type = GOP_LARGER;
    }
    return type;
}

IMPLEMENT_DYNAMIC( GOPDlg, CAcUiDialog )

GOPDlg::GOPDlg( CWnd* pParent /*=NULL*/ )
    : CAcUiDialog( GOPDlg::IDD, pParent )
    , m_xPos( 0 )
    , m_yPos( 0 )
    , m_zPos( 0 )
    , m_coal( 856 )
    , m_gas( 9.87 )
    , m_level( -294 )
    , m_dt( _T( "1990.05.03" ) )
{

}

GOPDlg::~GOPDlg()
{
}

void GOPDlg::DoDataExchange( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_GOP_PT_BTN, m_pickBtn );
    DDX_Text( pDX, IDC_GOP_X_POS, m_xPos );
    DDX_Text( pDX, IDC_GOP_Y_POS, m_yPos );
    DDX_Text( pDX, IDC_GOP_Z_POS, m_zPos );
    DDX_Text( pDX, IDC_GOP_COAL_MINE, m_coal );
    DDX_Text( pDX, IDC_GOP_GAS, m_gas );
    DDX_Text( pDX, IDC_GOP_LEVEL, m_level );
    DDX_Text( pDX, IDC_GOP_DATE, m_dt );
    DDX_Control( pDX, IDC_GOP_TYPE, m_typeCombo );
}


BEGIN_MESSAGE_MAP( GOPDlg, CAcUiDialog )
    ON_BN_CLICKED( IDOK, &GOPDlg::OnBnClickedOk )
    ON_BN_CLICKED( IDC_GOP_PT_BTN, &GOPDlg::OnBnClickedGopPtBtn )
END_MESSAGE_MAP()


BOOL GOPDlg::OnInitDialog()
{
    CAcUiDialog::OnInitDialog();

    m_pickBtn.AutoLoad();
    m_typeCombo.AddString( _T( "小型突出点" ) );
    m_typeCombo.AddString( _T( "中型突出点" ) );
    m_typeCombo.AddString( _T( "大型突出点" ) );
    m_typeCombo.AddString( _T( "特大型突出点" ) );

    if( !m_objId.isNull() )
    {
        readPropertyData();
        UpdateData( FALSE );

        m_typeCombo.EnableWindow( FALSE );
        m_typeCombo.SetCurSel( GetGOPType( m_objId ) );
        GetDlgItem( IDOK )->SetWindowText( _T( "修改" ) );
    }
    else
    {
        m_typeCombo.EnableWindow( TRUE );
        m_typeCombo.SetCurSel( 0 );
        GetDlgItem( IDOK )->SetWindowText( _T( "添加" ) );
    }

    return TRUE;
}

void GOPDlg::OnBnClickedOk()
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

void GOPDlg::OnBnClickedGopPtBtn()
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

void GOPDlg::readPropertyData()
{
    AcGePoint3d pt;
    GetInsertPoint( m_objId, pt );

    m_xPos = pt.x;
    m_yPos = pt.y;
    m_zPos = pt.z;

    GOPDataLink dl;
    dl.setDataSource( m_objId );
    dl.updateData( false );

    m_coal = dl.m_coal;
    m_gas = dl.m_gas;
    m_level = dl.m_level;
    m_dt = dl.m_dt;
}

void GOPDlg::writePropertyData()
{
    AcGePoint3d pt( m_xPos, m_yPos, m_zPos );
    SetInsertPoint( m_objId, pt );

    GOPDataLink dl;
    dl.setDataSource( m_objId );

    dl.m_coal = m_coal;
    dl.m_gas = m_gas;
    dl.m_level = m_level;
    dl.m_dt = m_dt;

    dl.updateData( true );
}

AcDbObjectId GOPDlg::createObject()
{
    AcDbObjectId objId;

    GOP_TYPE type = ( GOP_TYPE )m_typeCombo.GetCurSel();
    GOP* pGOP = CreateGOP( type, AcGePoint3d( m_xPos, m_yPos, m_zPos ) );
    if( ArxUtilHelper::PostToModelSpace( pGOP ) )
    {
        objId = pGOP->objectId();
        pGOP->close();
    }
    else
    {
        delete pGOP;
    }
    return objId;
}

