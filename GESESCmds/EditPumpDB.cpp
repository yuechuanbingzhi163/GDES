// EditPumpDB.cpp : 实现文件
//

#include "stdafx.h"
#include "EditPumpDB.h"


// EditPumpDB 对话框

IMPLEMENT_DYNAMIC(EditPumpDB, CDialog)

EditPumpDB::EditPumpDB(CWnd* pParent /*=NULL*/)
	: CDialog(EditPumpDB::IDD, pParent)
	, m_btype(FALSE)
	, m_bweight(FALSE)
	, m_bspeed(FALSE)
	, m_bmaxp(FALSE)
	, m_bfactory(FALSE)
	, m_blength(FALSE)
	, m_bheight(FALSE)
	, m_bweidth(FALSE)
	, m_bminabsp(FALSE)
	, m_babsp(FALSE)
	, m_bpower(FALSE)
	, m_bmaxq(FALSE)
	, m_ball(FALSE)
	, m_type(_T(""))
	, m_weight(_T(""))
	, m_speed(_T(""))
	, m_maxp(_T(""))
	, m_factory(_T(""))
	, m_length(_T(""))
	, m_height(_T(""))
	, m_weidth(_T(""))
	, m_absp(_T(""))
	, m_power(_T(""))
	, m_maxq(_T(""))
	, m_minabsp(_T(""))
	, m_itemsNum(_T(""))
{

}

EditPumpDB::~EditPumpDB()
{
}

void EditPumpDB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_PUMP_NAME_CHECK, m_btype);
	DDX_Check(pDX, IDC_PUMP_WEIGHT_CHECK, m_bweight);
	DDX_Check(pDX, IDC_PUMP_SPEED_CHECK, m_bspeed);
	DDX_Check(pDX, IDC_PUMP_MAXP_CHECK, m_bmaxp);
	DDX_Check(pDX, IDC_PUMP_FACTORY_CHECK, m_bfactory);
	DDX_Check(pDX, IDC_PUMP_LENGTH_CHECK, m_blength);
	DDX_Check(pDX, IDC_PUMP_HEIGHT_CHECK, m_bheight);
	DDX_Check(pDX, IDC_PUMP_WEIDTH_CHECK, m_bweidth);
	DDX_Check(pDX, IDC_PUMP_MINABSP_CHECK, m_bminabsp);
	DDX_Check(pDX, IDC_PUMP_ABSP_CHECK, m_babsp);
	DDX_Check(pDX, IDC_PUMP_POWER_CHECK, m_bpower);
	DDX_Check(pDX, IDC_PUMP_MAXQ_CHECK, m_bmaxq);
	DDX_Check(pDX, IDC_ALL_PUMP_CHECK, m_ball);
	DDX_Text(pDX, IDC_FIND_PUMP_NAME_EDIT, m_type);
	DDX_Text(pDX, IDC_FIND_PUMP_WEIGHT_EDIT, m_weight);
	DDX_Text(pDX, IDC_FIND_PUMP_SPEED_EDIT, m_speed);
	DDX_Text(pDX, IDC_FIND_PUMP_MAXP_EDIT, m_maxp);
	DDX_Text(pDX, IDC_FIND_PUMP_FACTORY_EDIT, m_factory);
	DDX_Text(pDX, IDC_FIND_PUMP_LENGTH_EDIT, m_length);
	DDX_Text(pDX, IDC_FIND_PUMP_HEIGHT_EDIT, m_height);
	DDX_Text(pDX, IDC_FIND_PUMP_WEIDTH_EDIT, m_weidth);
	DDX_Text(pDX, IDC_FIND_PUMP_ABSP_EDIT, m_absp);
	DDX_Text(pDX, IDC_FIND_PUMP_POWER_EDIT, m_power);
	DDX_Text(pDX, IDC_FIND_PUMP_MAXQ_EDIT, m_maxq);
	DDX_Text(pDX, IDC_FIND_PUMP_MINABSP_EDIT, m_minabsp);
	DDX_Text(pDX, IDC_PUMP_ITEMSNUM_EDIT, m_itemsNum);
}


BEGIN_MESSAGE_MAP(EditPumpDB, CDialog)
END_MESSAGE_MAP()


// EditPumpDB 消息处理程序
