// aculParamDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CaculParamDlg.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

extern CString ReadIntStringDatas(int intData,CString typeName);
extern int ReadStringIntDatas(CString strData,CString typeName);

IMPLEMENT_DYNAMIC(CaculParamDlg, CDialog)

CaculParamDlg::CaculParamDlg(CWnd* pParent /*=NULL*/,AcDbObjectId objId)
	: CDialog(CaculParamDlg::IDD, pParent)
	, m_time(_T(""))
	, m_speed(_T(""))
	, m_A(_T(""))
	, m_N(_T(""))
	, m_g(_T(""))
	, m_q0(_T(""))
	, m_PN(_T(""))
	, m_C1(_T(""))
	, m_inT(_T(""))
	, m_outT(_T(""))
	, m_allPower(_T(""))
	, m_Cp(_T(""))
	, m_ro(_T(""))
{
	m_objId = objId;
}

CaculParamDlg::~CaculParamDlg()
{
}

void CaculParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TIME_EDIT, m_time);
	DDX_Text(pDX, IDC_WIND_SPEED_EDIT, m_speed);
	DDX_Text(pDX, IDC_DYNAMITE_EDIT, m_A);
	DDX_Text(pDX, IDC_WORKERS_EDIT, m_N);
	DDX_Control(pDX, IDC_DYNAMITE_WAY_COMBO, m_dynamiteWay);
	DDX_Text(pDX, IDC_AVG_DISCHARGE_EDIT, m_g);
	DDX_Text(pDX, IDC_AIR_INDICATOR_EDIT, m_q0);
	DDX_Text(pDX, IDC_POWER_NUMBER_EDIT, m_PN);
	DDX_Text(pDX, IDC_ALLOW_CONCETRATION_EDIT, m_C1);
	DDX_Text(pDX, IDC_IN_TEMP_EDIT, m_inT);
	DDX_Text(pDX, IDC_OUT_TEMP_EDIT, m_outT);
	DDX_Text(pDX, IDC_ALL_POWER_EDIT, m_allPower);
	DDX_Text(pDX, IDC_AIR_SET_HOT_EDIT, m_Cp);
	DDX_Text(pDX, IDC_AIR_DENCITY_EDIT, m_ro);
}


BEGIN_MESSAGE_MAP(CaculParamDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CaculParamDlg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_WIND_SPEED_EDIT, &CaculParamDlg::OnEnKillfocusWindSpeedEdit)
	ON_EN_SETFOCUS(IDC_WIND_SPEED_EDIT, &CaculParamDlg::OnEnSetfocusWindSpeedEdit)
END_MESSAGE_MAP()


// CaculParamDlg 消息处理程序

void CaculParamDlg::OnBnClickedOk()
{
	writePropertyData();
	OnOK();
}

void CaculParamDlg::readPropertyData()
{
	DataHelper::GetPropertyData( m_objId, _T( "通风时间" ), m_time );
	DataHelper::GetPropertyData( m_objId, _T("一次爆破炸药消耗量"), m_A );
	DataHelper::GetPropertyData( m_objId, _T( "排尘风速" ), m_speed );
	DataHelper::GetPropertyData( m_objId, _T( "工作人数" ), m_N );
	DataHelper::GetPropertyData( m_objId, _T("有害成分的平均排量"), m_g );
	DataHelper::GetPropertyData( m_objId, _T("有害成分的允许浓度"), m_C1 );
	DataHelper::GetPropertyData( m_objId, _T("单位功率的风量指标"), m_q0 );
	DataHelper::GetPropertyData( m_objId, _T("柴油设备功率数"), m_PN );
	DataHelper::GetPropertyData( m_objId, _T( "进风端气温" ), m_inT );
	DataHelper::GetPropertyData( m_objId, _T( "排风端气温" ), m_outT );
	DataHelper::GetPropertyData( m_objId, _T("热源总散热量"), m_allPower );
	DataHelper::GetPropertyData( m_objId, _T( "空气定热比" ), m_Cp );
	DataHelper::GetPropertyData( m_objId, _T( "空气密度" ), m_ro );

	m_dynamiteWay.AddString( _T( "电雷管起爆" ) );
	m_dynamiteWay.AddString( _T( "火雷管起爆" ) );

	CString v;
	DataHelper::GetPropertyData( m_objId, _T( "引爆方式" ), v );

	int idx = ReadStringIntDatas(v,_T("通风方式"));
	m_dynamiteWay.SetCurSel( idx );

	if (m_speed.IsEmpty())
	{
		m_speed = _T("建议0.15~4m/s");
	}
	UpdateData( FALSE );
}

void CaculParamDlg::writePropertyData()
{
	UpdateData( TRUE );

	DataHelper::SetPropertyData( m_objId, _T( "通风时间" ), m_time );
	DataHelper::SetPropertyData( m_objId, _T("一次爆破炸药消耗量"), m_A );
	DataHelper::SetPropertyData( m_objId, _T( "排尘风速" ), m_speed );
	DataHelper::SetPropertyData( m_objId, _T( "工作人数" ), m_N );
	DataHelper::SetPropertyData( m_objId, _T("有害成分的平均排量"), m_g );
	DataHelper::SetPropertyData( m_objId, _T("有害成分的允许浓度"), m_C1 );
	DataHelper::SetPropertyData( m_objId, _T("单位功率的风量指标"), m_q0 );
	DataHelper::SetPropertyData( m_objId, _T("柴油设备功率数"), m_PN );
	DataHelper::SetPropertyData( m_objId, _T( "进风端气温" ), m_inT );
	DataHelper::SetPropertyData( m_objId, _T( "排风端气温" ), m_outT );
	DataHelper::SetPropertyData( m_objId, _T("热源总散热量"), m_allPower );
	DataHelper::SetPropertyData( m_objId, _T( "空气定热比" ), m_Cp );
	DataHelper::SetPropertyData( m_objId, _T( "空气密度" ), m_ro );
	
	CString rString = ReadIntStringDatas(m_dynamiteWay.GetCurSel(),_T("引爆方式"));
	DataHelper::SetPropertyData( m_objId, _T( "引爆方式" ), rString );
}

BOOL CaculParamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	readPropertyData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE

}

void CaculParamDlg::OnEnKillfocusWindSpeedEdit()
{
	CString msg;
	GetDlgItem(IDC_WIND_SPEED_EDIT)->GetWindowText(msg);

	//当失去焦点的时候检测是排尘风速是否为空
	//如果是给出建议值
	if (msg.IsEmpty())
	{
		msg = _T("建议0.15~4m/s");
		CEdit* pFriction = (CEdit*) GetDlgItem(IDC_WIND_SPEED_EDIT);
		pFriction->SetWindowText( msg );	
	}
}

void CaculParamDlg::OnEnSetfocusWindSpeedEdit()
{
	CString msg;
	GetDlgItem(IDC_WIND_SPEED_EDIT)->GetWindowText(msg);

	//当获得焦点的时候检测排尘风速是否是"建议0.15~4m/s"
	//如果是则清空，这样做是为了方便输入值
	if (_T("建议0.15~4m/s") == msg)
	{
		msg = _T("");
		CEdit* pFriction = (CEdit*) GetDlgItem(IDC_WIND_SPEED_EDIT);
		pFriction->SetWindowText( msg );	
	}
}
