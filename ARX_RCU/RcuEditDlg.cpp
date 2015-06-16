#include "stdafx.h"
#include "RcuEditDlg.h"
#include "RcuHelper.h"
#include "RcuDataLink.h"
#include "RcuAddDrillDlg.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

#include "../DefGE/RcuGE.h"
#include "../DefGE/Pore.h"
#include "../DefGE/DrillSite.h"
#include "../DefGE/RockGate.h"
#include "../DefGE/CoalSurface.h"

struct ItemData
{
	int iItem;            // 第几行的数据
	AcDbObjectId objId;   // 数据id
};

static int InsertDataToDrillSiteList(CListCtrl& m_list, const AcDbObjectId& objId)
{
	int n = m_list.GetItemCount();

	//增加一行数据
	m_list.InsertItem( n, _T( "xx" ) );

	//设置编号
	CString num;
	num.Format( _T( "%d" ), n + 1 );
	m_list.SetItemText( n, 0, num );

	//给每一行附加数据
	ItemData* pData = new ItemData();
	pData->iItem = n;
	pData->objId = objId;
	m_list.SetItemData( n, ( LPARAM )pData ); // 设置数据
	return n;
}

static void ClearListCtrlItem( CListCtrl& m_list, int row )
{
	int n = m_list.GetItemCount();
	if(row != LB_ERR && n > 0 && row < n) 
	{
		ItemData* pData = ( ItemData* )m_list.GetItemData( row );
		delete pData;
		m_list.DeleteItem(row);
	}
}

static void ClearListCtrlItemDatas( CListCtrl& m_list )
{
	int n = m_list.GetItemCount();
	for( int i = 0; i < n; i++ )
	{
		ItemData* pData = ( ItemData* )m_list.GetItemData( i );
		delete pData;
	}
}

static void BuildListCtrlHeadColumns( CListCtrl& m_list, const AcStringArray fields )
{
	int len = fields.length();

	CRect rect;
	m_list.GetClientRect( &rect );

	int n = len;
	if( n > 10 ) n = 7;
	double dw = rect.Width() / ( n + 1 );

	m_list.InsertColumn( 0, _T( "编号" ), LVCFMT_CENTER, dw );
	for( int i = 0; i < len; i++ )
	{
		m_list.InsertColumn( i + 1, fields[i].kACharPtr(), LVCFMT_LEFT, dw );
	}
}

static void ClearListCtrl( CListCtrl& m_list )
{
	//首先删除所有行
	m_list.DeleteAllItems();
	//得到列数
	int n = m_list.GetHeaderCtrl()->GetItemCount();
	//删除所有列
	for( int i = 0; i < n; i++ )
	{
		m_list.DeleteColumn( 0 );
	}
}

static void ZoomToEntity( const AcDbObjectId& objId )
{
	// 在非模态对话框下无法使用
	//ads_name en;
	//if(Acad::eOk != acdbGetAdsName(en, objId)) return;
	//acedCommand(RTSTR, _T("ZOOM"), RTSTR, _T("O"), RTENAME, en, RTSTR, _T(""), 0);

	// 临时使用sendStringToExecute解决缩放定位问题
	CString cmd;
	cmd.Format( _T( "ZOOM O \003" ) ); // 按空格结束选择对象，然后esc(防止多余的空格重复执行命令)
	acDocManager->sendStringToExecute( curDoc(), cmd, true, false, false );
}

//获取当前选择的行
static int GetCurSelOfList(CListCtrl& m_list)
{
	int row = LB_ERR;
	for( int i = 0; i < m_list.GetItemCount(); i++ )
	{
		if( LVIS_SELECTED == m_list.GetItemState( i, LVIS_SELECTED ) )
		{
			row = i;
			break;
		}
	}
	return row;
}

static bool ShowAddDrillDlg(/*RockGateLink& rg_link, CoalSurfaceLink& cs_link*/)
{
	CAcModuleResourceOverride resourceOverride;
	RcuAddDrillDlg dlg;
	if(IDOK != dlg.DoModal()) return false;

	////从对话框中提取数据
	//dlg.writeToDataLink(rg_link, cs_link);

	return true;
}

IMPLEMENT_DYNAMIC(RcuEditDlg, RcuAcUiBaseDlg)

RcuEditDlg::RcuEditDlg(CWnd* pParent /*=NULL*/)
	: RcuAcUiBaseDlg(RcuEditDlg::IDD, pParent)
	, m_x(0)
	, m_y(0)
	, m_z(0)
	, m_length(0)
	, m_width(0)
	, m_height(0)
	, m_left(0)
	, m_right(0)
	, m_top(0)
	, m_bottom(0)
	, m_radius(0)
	, m_thick(0)
	, m_angle(0)
	, m_dist(0)
{

}

RcuEditDlg::~RcuEditDlg()
{
	ClearListCtrlItemDatas(m_list);
}

void RcuEditDlg::DoDataExchange(CDataExchange* pDX)
{
	RcuAcUiBaseDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, m_x);
	DDX_Text(pDX, IDC_EDIT7, m_y);
	DDX_Text(pDX, IDC_EDIT9, m_z);
	DDX_Text(pDX, IDC_EDIT1, m_length);
	DDX_Text(pDX, IDC_EDIT2, m_width);
	DDX_Text(pDX, IDC_EDIT3, m_height);
	DDX_Text(pDX, IDC_EDIT10, m_left);
	DDX_Text(pDX, IDC_EDIT11, m_right);
	DDX_Text(pDX, IDC_EDIT12, m_top);
	DDX_Text(pDX, IDC_EDIT13, m_bottom);
	DDX_Text(pDX, IDC_EDIT17, m_radius);
	DDX_Text(pDX, IDC_EDIT14, m_thick);
	DDX_Text(pDX, IDC_EDIT15, m_angle);
	DDX_Text(pDX, IDC_EDIT16, m_dist);
	DDX_Text(pDX, IDC_EDIT18, m_name);
	DDX_Control(pDX, IDC_LIST2, m_list);
}


BEGIN_MESSAGE_MAP(RcuEditDlg, RcuAcUiBaseDlg)
	ON_BN_CLICKED(IDOK, &RcuEditDlg::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &RcuEditDlg::OnNMDblclkList2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &RcuEditDlg::OnNMRclickList2)
	ON_COMMAND(IDR_DRILL_SITE_ADD, &RcuEditDlg::OnAddCommand)
	ON_COMMAND(IDR_DRILL_SITE_DELETE, &RcuEditDlg::OnDeleteCommand)
	ON_COMMAND(IDR_DRILL_SITE_MODIFY, &RcuEditDlg::OnModifyCommand)
	ON_COMMAND(IDR_DRILL_SITE_HILIGHT, &RcuEditDlg::OnHilightCommand)
END_MESSAGE_MAP()

/**
在OnInitDialog中退出程序的方法
	1. 调用EndDialog函数:EndDialog(0);
	2. 调用SendMessage（WM_CLOSE）:SendMessage(WM_CLOSE);
*/
BOOL RcuEditDlg::OnInitDialog()
{
	RcuAcUiBaseDlg::OnInitDialog();

	m_list.SetExtendedStyle( m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	AcStringArray fields;
	FieldHelper::GetAllFields(_T("DrillSite"), fields);
	// 构建Column
	BuildListCtrlHeadColumns( m_list, fields );

	acDocManager->lockDocument( curDoc() );
	bool ret = readDataFromRockGate(m_rock_gate);
	acDocManager->unlockDocument( curDoc() );

	if(!ret)
	{
		MessageBox(_T("向对话框写入数据失败!!!\n本对话框即将关闭"));
		//退出对话框
		EndDialog(0);
	}
	else
	{
		//更新数据到界面
		UpdateData(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void RcuEditDlg::OnBnClickedOk()
{
	//从界面提取数据
	UpdateData( TRUE );

	acDocManager->lockDocument( curDoc() );
	if(!writeDataToRockGate(m_rock_gate))
	{
		MessageBox(_T("更新石门以及相关数据失败!!!"));
	}
	acDocManager->unlockDocument( curDoc() );

	OnOK();
}

void RcuEditDlg::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	//第几行
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int row = pNMItemActivate->iItem;
	if( row != LB_ERR )
	{
		ItemData* pData = ( ItemData* )m_list.GetItemData( row );

		acDocManager->lockDocument( curDoc() );
		//高亮选择图元
		ArxEntityHelper::SelectEntity(pData->objId);
		//定位到图元
		//ArxEntityHelper::ZoomToEntity(pData->objId);
		ZoomToEntity(pData->objId);
		acDocManager->unlockDocument( curDoc() );
	}
	//设置pResult为1,屏蔽父窗口响应该消息
	*pResult = 1;
}

void RcuEditDlg::OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	//第几行
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int row = pNMItemActivate->iItem;
	//防止在空白区点击弹出菜单
	//if (m_list.GetSelectedCount() > 0)
	{
		//下面的这段代码, 不单单适应于ListCtrl
		CMenu menu, *pPopup;
		menu.LoadMenu(IDR_DRILL_SITE_MENU);
		pPopup = menu.GetSubMenu(0);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint); //鼠标位置
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, myPoint.x, myPoint.y, this);
	}
	//设置pResult为1,屏蔽父窗口响应该消息
	*pResult = 1;
}

void RcuEditDlg::exchangeRockGateData(RockGateLink& rg_link, bool save)
{
	if(save)
	{
		//从对话框界面提取数据
		rg_link.m_name = m_name;
		rg_link.m_radius = m_radius;
		rg_link.m_length = m_length;
		rg_link.m_width = m_width;
		rg_link.m_height = m_height;
		rg_link.m_left = m_left;
		rg_link.m_right = m_right;
		rg_link.m_top = m_top;
		rg_link.m_bottom = m_bottom;
		rg_link.m_pt = ArxUtilHelper::Point3dToString(AcGePoint3d(m_x, m_y, m_z));
		rg_link.m_dist = m_dist;
		//将数据更新到图元中
		rg_link.updateData(true);
	}
	else
	{
		//从图元中提取数据
		rg_link.updateData(false);
		//给对话框界面赋值
		m_name = rg_link.m_name;
		m_radius = rg_link.m_radius;
		m_length = rg_link.m_length;
		m_width = rg_link.m_width;
		m_height = rg_link.m_height;
		m_left = rg_link.m_left;
		m_right = rg_link.m_right;
		m_top = rg_link.m_top;
		m_bottom = rg_link.m_bottom;
		AcGePoint3d pt;
		ArxUtilHelper::StringToPoint3d(rg_link.m_pt, pt);
		m_x = pt.x; m_y = pt.y; m_z = pt.z;
		m_dist = rg_link.m_dist;
	}
}

void RcuEditDlg::exchangeCoalSurfaceData(CoalSurfaceLink& cs_link, bool save)
{
	if(save)
	{
		//从对话框界面提取数据
		cs_link.m_thick = m_thick;
		cs_link.m_angle = m_angle;
		//将数据更新到图元中
		cs_link.updateData(true);
	}
	else
	{
		//从图元中提取数据
		cs_link.updateData(false);
		//给对话框界面赋值
		m_thick = cs_link.m_thick;
		m_angle = cs_link.m_angle;
	}
}

int RcuEditDlg::findDrillSiteRow(const AcDbObjectId& drill_site)
{
	int row = LB_ERR;
	for( int i = 0; i < m_list.GetItemCount(); i++ )
	{
		ItemData* pData = ( ItemData* )m_list.GetItemData( i );
		if(pData->objId == drill_site)
		{
			row = i; break;
		}
	}
	return row;
}

void RcuEditDlg::exchangeDrillSiteData(DrillSiteLink& ds_link, bool save)
{
	AcDbObjectId drill_site = ds_link.getDataSource();
	if(drill_site.isNull()) return;

	int row = findDrillSiteRow(drill_site);

	if(save && row != LB_ERR)
	{
		//从listctrl中提取钻场数据
		{
			CString value = m_list.GetItemText(row, 1);
			ds_link.m_name = value;
		}
		{
			CString value = m_list.GetItemText(row, 2);
			ds_link.m_leftOrRight = 0; // 0代表错误的位置
			ArxUtilHelper::StringToInt(value, ds_link.m_leftOrRight);
		}
		{
			CString value = m_list.GetItemText(row, 3);
			ds_link.m_dist = 0;
			ArxUtilHelper::StringToDouble(value, ds_link.m_dist);
		}
		{
			CString value = m_list.GetItemText(row, 4);
			ds_link.m_width = 0;
			ArxUtilHelper::StringToDouble(value, ds_link.m_width);
		}
		{
			CString value = m_list.GetItemText(row, 5);
			ds_link.m_height = 0;
			ArxUtilHelper::StringToDouble(value, ds_link.m_height);
		}
		{
			CString value = m_list.GetItemText(row, 6);
			ds_link.m_start = 0;
			ArxUtilHelper::StringToInt(value, ds_link.m_start);
		}
		//将数据更新到图元中
		ds_link.updateData(true);
	}
	else
	{
		//增加数据
		if(row == LB_ERR)
		{
			row = InsertDataToDrillSiteList(m_list, drill_site);
		}

		//从图元中提取数据
		ds_link.updateData(false);
		//更新listctrl中钻场的数据
		{
			m_list.SetItemText(row, 1, ds_link.m_name);
		}
		{
			CString value;
			ArxUtilHelper::IntToString(ds_link.m_leftOrRight, value);
			m_list.SetItemText(row, 2, value);
		}
		{
			CString value;
			ArxUtilHelper::DoubleToString(ds_link.m_dist, value);
			m_list.SetItemText(row, 3, value);
		}
		{
			CString value;
			ArxUtilHelper::DoubleToString(ds_link.m_width, value);
			m_list.SetItemText(row, 4, value);
		}
		{
			CString value;
			ArxUtilHelper::DoubleToString(ds_link.m_height, value);
			m_list.SetItemText(row, 5, value);
		}
		{
			CString value;
			ArxUtilHelper::IntToString(ds_link.m_start, value);
			m_list.SetItemText(row, 6, value);
		}
	}
}

bool RcuEditDlg::readDataFromRockGate(const AcDbObjectId& rock_gate)
{
	//提取石门关联的所有数据并填充到对话框中
	RockGateLink rg_link;
	if(!RcuHelper::GetRockGateData(rock_gate, rg_link))
	{
		MessageBox(_T("提取石门数据发生错误!"));
		return false;
	}

	//查找石门关联的煤层
	AcDbObjectId coal_surf;
	RcuHelper::GetCoalSurface(rock_gate, coal_surf);
	if(coal_surf.isNull())
	{
		MessageBox(_T("该石门没有关联的煤层面!"));
		return false;
	}
	//提取煤层的数据
	CoalSurfaceLink cs_link;
	if(!RcuHelper::GetCoalSurfaceData(coal_surf, cs_link))
	{
		MessageBox(_T("提取煤层数据发生错误!"));
		return false;
	}

	//交换石门数据到对话框中
	exchangeRockGateData(rg_link, false);
	//交换煤层数据到对话框中
	exchangeCoalSurfaceData(cs_link, false);

	//查找石门关联的钻场
	AcDbObjectIdArray drill_sites;
	RcuHelper::GetDrillSites(rock_gate, drill_sites);
	for(int i=0;i<drill_sites.length();i++)
	{
		//提取钻场的数据
		DrillSiteLink ds_link;
		if(RcuHelper::GetDrillSiteData(drill_sites[i], ds_link))
		{
			//交换钻场数据到对话框中
			exchangeDrillSiteData(ds_link, false);
		}
	}
	return true;
}

bool RcuEditDlg::writeDataToRockGate(const AcDbObjectId& rock_gate)
{
	RockGateLink rg_link;
	rg_link.setDataSource(rock_gate);

	//查找石门关联的煤层
	AcDbObjectId coal_surf;
	RcuHelper::GetCoalSurface(rock_gate, coal_surf);
	if(coal_surf.isNull())
	{
		MessageBox(_T("找不到石门关联的煤层面!"));
		return false;
	}

	CoalSurfaceLink cs_link;
	cs_link.setDataSource(coal_surf);

	//从对话框中提取数据给石门
	exchangeRockGateData(rg_link, true);
	//从对话框中提取数据给煤层
	exchangeCoalSurfaceData(cs_link, true);

	//查找石门关联的钻场
	for(int i=0;i<m_list.GetItemCount();i++)
	{
		ItemData* pData = ( ItemData* )m_list.GetItemData( i );

		DrillSiteLink ds_link;
		ds_link.setDataSource(pData->objId);
		//从对话框中提取数据给钻场
		exchangeDrillSiteData(ds_link, true);
	}
	return true;
}

void RcuEditDlg::OnAddCommand()
{
	acDocManager->lockDocument( curDoc() );

	//ShowOrHideWindow show_hide(this);
	if(!ShowAddDrillDlg()) return;

	////高亮选中石门图元
	//acDocManager->lockDocument( curDoc() );
	////ArxEntityHelper::SelectEntity(pData->objId);
	acDocManager->unlockDocument( curDoc() );

	//cad窗口获取焦点
	acedGetAcadFrame()->SetFocus();
}

void RcuEditDlg::OnDeleteCommand()
{
	MessageBox(_T("OnDeleteCommand"));
}

void RcuEditDlg::OnModifyCommand()
{
	MessageBox(_T("OnModifyCommand"));
}

void RcuEditDlg::OnHilightCommand()
{
	MessageBox(_T("OnHilightCommand"));
}
