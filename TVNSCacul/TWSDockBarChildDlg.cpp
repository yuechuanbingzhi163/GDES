#include "stdafx.h"
#include "TWSDockBarChildDlg.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"


IMPLEMENT_DYNAMIC( TWSDockBarChildDlg, PropertyData_DockBarChildDlg )

TWSDockBarChildDlg::TWSDockBarChildDlg( CWnd* pParent, HINSTANCE hInstance )
    : PropertyData_DockBarChildDlg( TWSDockBarChildDlg::IDD, pParent, hInstance )
    , m_cm( 0 )
{

}

TWSDockBarChildDlg::~TWSDockBarChildDlg()
{
}

void TWSDockBarChildDlg::DoDataExchange( CDataExchange* pDX )
{
    PropertyData_DockBarChildDlg::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_TWS_NAME_EDIT, m_name );
    DDX_Text( pDX, IDC_TWS_AREA_EDIT, m_area );
    DDX_Text( pDX, IDC_TWS_VELOCITY_EDIT, m_velocity );
    DDX_Text( pDX, IDC_TWS_Q_EDIT, m_q );
    DDX_Text( pDX, IDC_TWS_TEMP_EDIT, m_temp );

    DDX_Radio( pDX, IDC_TWS_TEMP_MEASURE_RADIO1, m_cm );
    DDX_Control( pDX, IDC_TWS_TUNNEL_MATERIAL_TYPE, m_combo );
}

BEGIN_MESSAGE_MAP( TWSDockBarChildDlg, PropertyData_DockBarChildDlg )
END_MESSAGE_MAP()


BOOL TWSDockBarChildDlg::OnInitDialog()
{
    PropertyData_DockBarChildDlg::OnInitDialog();

    m_combo.AddString( _T( "岩巷" ) );
    m_combo.AddString( _T( "煤巷" ) );
    m_combo.AddString( _T( "半煤岩巷" ) );

    return TRUE;
}

void TWSDockBarChildDlg::readControlMeasure()
{
    CString v1;
    //DataHelper::GetPropertyData( m_objId, _T( "超温控制措施" ), v1 );
    int i1 = 0;
    ArxUtilHelper::StringToInt( v1, i1 );
    m_cm = i1 % 3; // 0-无,1-控制措施,2-停止作业
}

void TWSDockBarChildDlg::readMaterial()
{
    CString v3;
    DataHelper::GetPropertyData( m_objId, _T( "材质类型" ), v3 );
    int i3 = 0;
    ArxUtilHelper::StringToInt( v3, i3 );
    i3 = i3 % 3;
    m_combo.SetCurSel( i3 );
}

void TWSDockBarChildDlg::writeControlMeasure()
{
    CString v1;
    v1.Format( _T( "%d" ), m_cm );
    //DataHelper::SetPropertyData( m_objId, _T( "超温控制措施" ), v1 );
}

void TWSDockBarChildDlg::writeMaterial()
{
    CString v3;
    v3.Format( _T( "%d" ), m_combo.GetCurSel() );
    DataHelper::SetPropertyData( m_objId, _T( "材质类型" ), v3 );
}

void TWSDockBarChildDlg::readPropertyData()
{
    DataHelper::GetPropertyData( m_objId, _T( "名称" ), m_name );
    DataHelper::GetPropertyData( m_objId, _T( "断面面积" ), m_area );
    DataHelper::GetPropertyData( m_objId, _T( "风速" ), m_velocity );
    DataHelper::GetPropertyData( m_objId, _T( "风量" ), m_q );
    DataHelper::GetPropertyData( m_objId, _T( "温度" ), m_temp );
    readControlMeasure();
    readMaterial();
}

void TWSDockBarChildDlg::writePropertyData(  )
{
    DataHelper::SetPropertyData( m_objId, _T( "名称" ), m_name );
    DataHelper::SetPropertyData( m_objId, _T( "断面面积" ), m_area );
    DataHelper::SetPropertyData( m_objId, _T( "风速" ), m_velocity );
    DataHelper::SetPropertyData( m_objId, _T( "风量" ), m_q );
    DataHelper::SetPropertyData( m_objId, _T( "温度" ), m_temp );
    writeControlMeasure();
    writeMaterial();
}

