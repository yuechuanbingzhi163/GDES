#include "stdafx.h"
#include "RcuDesignDlg.h"
#include "RcuEditRockGateDlg.h"
#include "RcuEditDrillSiteDlg.h"
#include "RcuEditOpenPoreDlg.h"

#include "Rcu.h"
#include "RcuHelper.h"
#include "RcuDataLink.h"
#include "ListCtrlHelper.h"
#include "SwitchHelper.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

#include "../DefGE/RcuGE.h"
#include "../DefGE/Pore.h"
#include "../DefGE/DrillSite.h"
#include "../DefGE/RockGate.h"
#include "../DefGE/CoalSurface.h"

//自定义消息
#include "MyMsg.h"

//微调list1的表头列宽度
static void CrackHeadColumnWidth(CListCtrl& m_list)
{
	m_list.SetColumnWidth(1, int(m_list.GetColumnWidth(1)*1.2));
	for(int i=5;i<m_list.GetHeaderCtrl()->GetItemCount()+1;i++)
	{
		m_list.SetColumnWidth(i, int(m_list.GetColumnWidth(i)*1.5));
	}
	m_list.SetColumnWidth(7, int(m_list.GetColumnWidth(7)*1.5));
}

//修改钻场列表第n行的数据
static void ModifyDrillSiteToListCtrl(CListCtrl& m_list2, int n, const DrillSiteLink& ds_link)
{
	{
		m_list2.SetItemText( n, 1, ds_link.m_name );
	}
	{
		CString value;
		ArxUtilHelper::IntToString(ds_link.m_leftOrRight, value);
		m_list2.SetItemText( n, 2, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_dist, value);
		m_list2.SetItemText( n, 3, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_depth, value);
		m_list2.SetItemText( n, 4, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_height, value);
		m_list2.SetItemText( n, 5, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_radius, value);
		m_list2.SetItemText( n, 6, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_gap, value);
		m_list2.SetItemText( n, 7, value);
	}
	{
		CString value;
		ArxUtilHelper::IntToString(ds_link.m_start, value);
		m_list2.SetItemText( n, 8, value);
	}
	
	{
		m_list2.SetItemText( n, 9, ArxUtilHelper::Point3dToString(ds_link.m_pt));
	}
}

//修改石门列表第n行的数据
static void ModifyRockGateToListCtrl(CListCtrl& m_list, int n, RockGateLink& rg_link)
{
	{
		m_list.SetItemText( n, 1, rg_link.m_name );
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(rg_link.m_length, value);
		m_list.SetItemText( n, 2, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(rg_link.m_width, value);
		m_list.SetItemText( n, 3, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(rg_link.m_height, value);
		m_list.SetItemText( n, 4, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(rg_link.m_dist, value);
		m_list.SetItemText( n, 5, value);
	}
	{
		m_list.SetItemText( n, 6, ArxUtilHelper::Point3dToString(rg_link.m_pt));
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(rg_link.m_left, value);
		m_list.SetItemText( n, 7, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(rg_link.m_right, value);
		m_list.SetItemText( n, 8, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(rg_link.m_top, value);
		m_list.SetItemText( n, 9, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(rg_link.m_bottom, value);
		m_list.SetItemText( n, 10, value);
	}
}

//向石门列表中新增一行
static void InsertRockGateToListCtrl(CListCtrl& m_list, const AcDbObjectId& objId, RockGateLink& rg_link)
{
	int row = ListCtrlHelper::SearchRowOfList(m_list, objId);
	if(row == -1)
	{
		//增加一行
		row = ListCtrlHelper::InsertRowToListCtrl(m_list, objId);
	}
	//设置石门列表的各列数据
	ModifyRockGateToListCtrl(m_list, row, rg_link);
}

//向钻场列表中新增一行
static void InsertDrillSiteToListCtrl(CListCtrl& m_list, const AcDbObjectId& objId, const DrillSiteLink& ds_link)
{
	int row = ListCtrlHelper::SearchRowOfList(m_list, objId);
	if(row == -1)
	{
		//增加一行
		row = ListCtrlHelper::InsertRowToListCtrl(m_list, objId);
	}
	//设置钻场列表的各列数据
	ModifyDrillSiteToListCtrl(m_list, row, ds_link);
}

static bool UpdateRockGateDataFromDlg(const AcDbObjectId& rock_gate, RockGateLink& rg_link, CoalSurfaceLink& cs_link)
{
	CAcModuleResourceOverride resourceOverride;

	//初始化石门对话框
	RcuEditRockGateDlg dlg;
	dlg.m_rock_gate = rock_gate;
	//如果石门id不为空,则读取数据并更新到对话框
	dlg.readFromDataLink(rg_link, cs_link);

	if(IDOK != dlg.DoModal()) return false;

	//从对话框中提取数据
	dlg.writeToDataLink(rg_link, cs_link);

	//将数据更新到图元中
	rg_link.updateData(true);
	cs_link.updateData(true);

	return true;
}

static bool UpdateDrillSiteDataFromDlg(const AcDbObjectId& drill_site, DrillSiteLink& ds_link)
{
	CAcModuleResourceOverride resourceOverride;

	//初始化钻场对话框
	RcuEditDrillSiteDlg dlg;
	dlg.m_drill_site = drill_site;
	dlg.readFromDataLink(ds_link);

	if(IDOK != dlg.DoModal()) return false;

	//从对话框中提取数据
	dlg.writeToDataLink(ds_link);

	if(!drill_site.isNull())
	{
		//修改钻场的几何参数
		RcuHelper::ModifyDrillSiteParam(drill_site, ds_link);
	}

	//将数据更新到图元中
	ds_link.updateData(true);
	
	return true;
}

//格式化钻场钻孔设计对话框的标题
static CString FormatPoreDlgTitle(const CString& rg_name, const CString& ds_name)
{
	CString name1 = rg_name;
	CString name2 = ds_name;
	name1.Trim();
	name2.Trim();
	if(name1.IsEmpty())
	{
		name1 = _T("***");
	}
	if(name2.IsEmpty())
	{
		name2 = _T("###");
	}

	CString title;
	title.Format(_T("%s石门-%s钻场-钻孔设计"), name1, name2);
	return title;
}

//格式化钻场钻孔设计对话框中的位置信息
static CString FormatDrillSitePostion(double m_dist, int leftOrRight)
{
	CString pos;
	pos.Format(_T("%s，距离迎头%.2f米"), (leftOrRight==0)?_T("左帮"):_T("右帮"), m_dist);
	return pos;
}

static bool UpdateOpenPoresFromDlg(const AcDbObjectId& drill_site, RockGateLink& rg_link, DrillSiteLink& ds_link)
{
	CAcModuleResourceOverride resourceOverride;
	
	RcuEditOpenPoreDlg dlg;
	//格式化钻孔设计对话框的标题
	dlg.m_title = FormatPoreDlgTitle(rg_link.m_name, ds_link.m_name);
	//格式化钻场的位置信息
	dlg.m_pos = FormatDrillSitePostion(ds_link.m_dist, ds_link.m_leftOrRight);
	//设置钻孔半径
	dlg.m_radius = ds_link.m_radius;
	//提取钻孔的个数
	AcDbObjectIdArray pores;
	RcuHelper::GetRelatedOpenPores(drill_site, pores);
	dlg.m_num = pores.length();
	//提取钻孔的间距
	if(!pores.isEmpty())
	{
		dlg.m_gap = ds_link.m_gap;
	}
	else
	{
		dlg.m_gap = 0;
	}

	if(IDOK != dlg.DoModal()) return false;

	//从对话框中读取数据
	//钻孔间距
	ds_link.m_gap = dlg.m_gap;
	//更新数据到钻孔
	ds_link.updateData(true);

	//删除关联的所有钻孔
	RcuHelper::ClearRelatedOpenPores(drill_site);

	//创建新的钻孔图元
	return RcuHelper::CreateOpenPores(drill_site, ds_link);
}

// RcuDesignDlg 对话框

IMPLEMENT_DYNAMIC(RcuDesignDlg, DockBarChildDlg)

RcuDesignDlg::RcuDesignDlg(CWnd* pParent /*=NULL*/)
	: DockBarChildDlg(RcuDesignDlg::IDD, pParent)
{
}

RcuDesignDlg::~RcuDesignDlg()
{
	//ClearListCtrlItemDatas(m_list);
}

void RcuDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	DockBarChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_LIST3, m_list2);
}

BEGIN_MESSAGE_MAP(RcuDesignDlg, DockBarChildDlg)
	ON_BN_CLICKED(IDC_BUTTON4, &RcuDesignDlg::OnBnClickedExport)
	ON_BN_CLICKED(IDC_BUTTON1, &RcuDesignDlg::OnBnClickedDesignClosePores)
	ON_BN_CLICKED(IDC_BUTTON2, &RcuDesignDlg::OnBnClickedDesignOpenPores)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &RcuDesignDlg::OnLvnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &RcuDesignDlg::OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &RcuDesignDlg::OnNMRclickList1)
	ON_COMMAND(ID_RCU_LIST_HILIGHT, &RcuDesignDlg::OnHilightRockGateCommand)
	ON_COMMAND(ID_RCU_LIST_MODIFY, &RcuDesignDlg::OnModifyRockGateCommand)
	ON_COMMAND(ID_RCU_LIST_DELETE, &RcuDesignDlg::OnDeleteRockGateCommand)
	ON_COMMAND(ID_RCU_LIST_ADD, &RcuDesignDlg::OnAddRockGateCommand)
	ON_COMMAND(ID_RCU_LIST_UPDATE, &RcuDesignDlg::OnUpdateRockGateListCommand)
	ON_COMMAND(ID_RCU_COAL_HILIGHT, &RcuDesignDlg::OnHilightCoalSurfCommand)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST3, &RcuDesignDlg::OnNMDblclkList2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, &RcuDesignDlg::OnNMRclickList2)
	ON_COMMAND(IDR_DRILL_SITE_ADD, &RcuDesignDlg::OnAddDrillSiteCommand)
	ON_COMMAND(IDR_DRILL_SITE_DELETE, &RcuDesignDlg::OnDeleteDrillSiteCommand)
	ON_COMMAND(IDR_DRILL_SITE_MODIFY, &RcuDesignDlg::OnModifyDrillSiteCommand)
	ON_COMMAND(IDR_DRILL_SITE_HILIGHT, &RcuDesignDlg::OnHilightDrillSiteCommand)

	ON_MESSAGE(WM_RCU_ADD, &RcuDesignDlg::OnRcuAddMessage)
	ON_MESSAGE(WM_RCU_DEL, &RcuDesignDlg::OnRcuDelMessage)
	ON_MESSAGE(WM_RCU_MODIFY, &RcuDesignDlg::OnRcuModifyMessage)
	ON_MESSAGE(WM_RCU_UPDATE, &RcuDesignDlg::OnRcuUpdateMessage)

END_MESSAGE_MAP()

BOOL RcuDesignDlg::OnInitDialog()
{
	DockBarChildDlg::OnInitDialog();

	m_list.SetExtendedStyle( m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	AcStringArray rock_fields;
	FieldHelper::GetAllFields(_T("RockGate"), rock_fields);
	//构建表头列表
	ListCtrlHelper::BuildListCtrlHeadColumns( m_list, rock_fields );
	CrackHeadColumnWidth(m_list);

	m_list2.SetExtendedStyle( m_list2.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	AcStringArray drill_fields;
	FieldHelper::GetAllFields(_T("DrillSite"), drill_fields);
	//构建表头列表
	ListCtrlHelper::BuildListCtrlHeadColumns( m_list2, drill_fields );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void RcuDesignDlg::OnClosing()
{
	//删除列表中每一行附带的数据(这些数据都是new出来的)
	//ClearListCtrlItemDatas(m_list);
	// 清空链表
	//ClearList(m_list);
}

// RcuDesignDlg 消息处理程序

/**
http://blog.csdn.net/carl2380/article/details/4816392
http://blog.csdn.net/sjls2659/article/details/7580660
http://blog.csdn.net/oldmtn/article/details/9298577
单击ListCtrl时，LVN_ITEMCHANGED响应3次导致莫名其妙的崩溃
解决方法:
	增加如下代码
		if(LVIF_STATE == pNMLV->uChanged && (pNMLV->uNewState & LVIS_SELECTED))
		{
			int row = pNMLV->iItem;
		}
*/
void RcuDesignDlg::OnLvnItemchangedList1( NMHDR* pNMHDR, LRESULT* pResult )
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );
	*pResult = 0;

	if(LVIF_STATE == pNMLV->uChanged && (pNMLV->uNewState & LVIS_SELECTED))
	{
		int row1 = pNMLV->iItem;
		if( row1 != LB_ERR )
		{
			//当前选择的石门
			ItemData* pData1 = (ItemData*)m_list.GetItemData(row1);

			//清空钻场列表
			ListCtrlHelper::ClearListCtrl(m_list2);

			//文档锁切换
			DocumentLockSwitch lock_switch;

			//查找石门关联的钻场
			AcDbObjectIdArray drill_sites;
			RcuHelper::GetRelatedDrillSites(pData1->objId, drill_sites);
			if(drill_sites.isEmpty()) return;

			//查找钻场的所有字段
			AcStringArray fields;
			FieldHelper::GetAllFields(_T("DrillSite"), fields);
			if(fields.isEmpty()) return;

			//添加到钻场列表
			ListCtrlHelper::InsertDatasToListCtrl(m_list2, fields, drill_sites);

			//屏蔽父窗口对该消息的处理
			*pResult = 1;
		}
	}
}

void RcuDesignDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//第几行
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	int row1 = pNMItemActivate->iItem;
	if( row1 != LB_ERR )
	{
		ItemData* pData1 = (ItemData*)m_list.GetItemData(row1);

		//文档锁切换
		DocumentLockSwitch lock_switch;
		//高亮选择图元
		ArxEntityHelper::SelectEntity(pData1->objId);
		//定位到图元
		ArxEntityHelper::ZoomToEntityForModeless(pData1->objId);
		//设置pResult为1,屏蔽父窗口响应该消息
		*pResult = 1;
	}
}

void RcuDesignDlg::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//第几行
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	int row = pNMItemActivate->iItem;
	//防止在空白区点击弹出菜单
	//if (m_list.GetSelectedCount() > 0)
	{
		//下面的这段代码, 不单单适应于ListCtrl
		CMenu menu, *pPopup;
		menu.LoadMenu(IDR_RCU_LIST_MENU);
		pPopup = menu.GetSubMenu(0);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint); //鼠标位置
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, myPoint.x, myPoint.y,this);  
	}
	//设置pResult为1,屏蔽父窗口响应该消息
	*pResult = 1;
}

void RcuDesignDlg::OnAddRockGateCommand()
{
	//切换controlbar的显示
	ControlBarShowSwitch cb_switch(this);

	//切换文档锁
	DocumentLockSwitch lock_switch;

	//选择石门的插入点坐标
	AcGePoint3d pt;
	if(!ArxUtilHelper::PromptPt(_T("\n请选择石门的插入点坐标："), pt)) return;

	//调用对话框获取石门和煤层数据
	RockGateLink rg_link;
	CoalSurfaceLink cs_link;
	if(!UpdateRockGateDataFromDlg(AcDbObjectId::kNull, rg_link, cs_link)) return;

	//创建石门和煤层图元
	if(RcuHelper::CreateRockGate(pt, rg_link, cs_link))
	{
		//向list1中插入一行石门数据
		InsertRockGateToListCtrl(m_list, rg_link.getDataSource(), rg_link);

		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnDeleteRockGateCommand()
{
	//MessageBox(_T("RcuDesignDlg::OnDeleteRockGateCommand"));
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//文档锁切换
		DocumentLockSwitch lock_switch;

		//删除图元
		//注:EraseObject2使用open/close删除才有效石门和关联的图元
		//   而EraseObject函数使用的是事务,不一定能有效删除!!!
		ArxEntityHelper::EraseObject2(pData1->objId, true);

		//删除选择的行
		ListCtrlHelper::DeleteListCtrlItem(m_list, row1);
		ListCtrlHelper::ClearListCtrl(m_list2);

		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnModifyRockGateCommand()
{
	//MessageBox(_T("RcuDesignDlg::OnLocateCommand"));
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 == LB_ERR)
	{
		MessageBox( _T( "请确保当前有一行被选中!" ) );
		return;
	}
	ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

	//文档切换锁
	DocumentLockSwitch lock_switch;
	
	//提取石门和煤层数据
	RockGateLink rg_link;
	CoalSurfaceLink cs_link;
	if(!RcuHelper::GetRockGateAndCoalSurfDatas(pData1->objId, rg_link, cs_link))
	{
		MessageBox(_T("提取石门或煤层失败!!!"));
		return;
	}

	//切换controlbar的显示
	ControlBarShowSwitch cb_switch(this);

	//从对话框中更新数据
	if(UpdateRockGateDataFromDlg(pData1->objId, rg_link, cs_link))
	{
		//修改当前选中石门的数据
		ModifyRockGateToListCtrl(m_list, row1, rg_link);
		//acutPrintf(_T("\n计算之前->宽度:%.4lf\t高度:%.4lf\n"),cs_link.m_width,cs_link.m_height);
		
		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnHilightRockGateCommand()
{
	//MessageBox(_T("RcuDesignDlg::OnHilightRockGateCommand"));
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//文档锁切换
		DocumentLockSwitch lock_switch;
		//高亮选中石门图元
		ArxEntityHelper::SelectEntity(pData1->objId);

		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnUpdateRockGateListCommand()
{
	//更新界面
	updateUI();
}

void RcuDesignDlg::OnHilightCoalSurfCommand()
{
	//MessageBox(_T("RcuDesignDlg::OnHilightCoalSurfCommand"));
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//文档锁切换
		DocumentLockSwitch lock_switch;

		AcDbObjectId coal_surf;
		RcuHelper::GetRelatedCoalSurface(pData1->objId, coal_surf);

		//高亮并定位煤层图元
		ArxEntityHelper::SelectEntity(coal_surf);
		ArxEntityHelper::ZoomToEntityForModeless(coal_surf);

		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	//第几行
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	int row = pNMItemActivate->iItem;
	if( row != LB_ERR )
	{
		ItemData* pData = ( ItemData* )m_list2.GetItemData( row );

		//文档锁切换
		DocumentLockSwitch lock_switch;

		//高亮选择图元
		ArxEntityHelper::SelectEntity(pData->objId);
		//定位到图元
		ArxEntityHelper::ZoomToEntityForModeless(pData->objId);
		
		//设置pResult为1,屏蔽父窗口响应该消息
		*pResult = 1;
	}
}

void RcuDesignDlg::OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	//第几行
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	int row = pNMItemActivate->iItem;
	//防止在空白区点击弹出菜单
	//if (m_list2.GetSelectedCount() > 0)
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

void RcuDesignDlg::OnAddDrillSiteCommand()
{
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 == LB_ERR)
	{
		MessageBox(_T("请指定或添加石门后再进行钻场设计!"));
		return;
	}

	ItemData* pData1 = (ItemData*)m_list.GetItemData(row1);

	//切换controlbar的显示
	//ControlBarShowSwitch cb_switch(this);

	//文档锁切换
	DocumentLockSwitch lock_switch;

	//调用对话框获取钻场数据
	DrillSiteLink ds_link;
	if(!UpdateDrillSiteDataFromDlg(AcDbObjectId::kNull, ds_link)) return;

	//创建石门和煤层图元
	if(RcuHelper::CreateDrillSite(pData1->objId, ds_link))
	{
		//向list1中插入一行石门数据
		InsertDrillSiteToListCtrl(m_list2, ds_link.getDataSource(), ds_link);

		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnDeleteDrillSiteCommand()
{
	int row2 = ListCtrlHelper::GetCurSelOfList(m_list2);
	if(row2 != LB_ERR)
	{
		ItemData* pData2 = ( ItemData* )m_list2.GetItemData(row2);

		//文档锁切换
		DocumentLockSwitch lock_switch;
		//删除钻场图元
		ArxEntityHelper::EraseObject2(pData2->objId, true);
		//删除选择的行
		ListCtrlHelper::DeleteListCtrlItem(m_list2, row2);

		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnModifyDrillSiteCommand()
{
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 == LB_ERR)
	{
		MessageBox(_T("请指定或添加石门后再进行钻场设计!"));
		return;
	}

	int row2 = ListCtrlHelper::GetCurSelOfList(m_list2);
	if(row2 == LB_ERR)
	{
		MessageBox( _T( "请选择一个钻场进行设计!" ) );
		return;
	}

	ItemData* pData1 = (ItemData*)m_list.GetItemData(row1);
	ItemData* pData2 = (ItemData*)m_list2.GetItemData(row2);

	//切换文档锁
	DocumentLockSwitch lock_switch;

	//从图元中提取钻场数据
	DrillSiteLink ds_link;
	if(!RcuHelper::GetDrillSiteData(pData2->objId, ds_link))
	{
		MessageBox(_T("提取钻场数据失败!!!"));
		return;
	}

	//切换controlbar的显示
	//ControlBarShowSwitch cb_switch(this);

	//调用对话框更新钻场数据
	if(UpdateDrillSiteDataFromDlg(pData2->objId, ds_link))
	{
		//向list1中插入一行石门数据
		ModifyDrillSiteToListCtrl(m_list2, row2, ds_link);

		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnHilightDrillSiteCommand()
{
	//ShowOrHideWindow show_hide(this);
	int row2 = ListCtrlHelper::GetCurSelOfList(m_list2);
	if(row2 != LB_ERR)
	{
		ItemData* pData2 = (ItemData*)m_list2.GetItemData(row2);

		//文档锁切换
		DocumentLockSwitch lock_switch;

		//高亮选中钻场图元
		ArxEntityHelper::SelectEntity(pData2->objId);

		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnBnClickedDesignClosePores()
{
	MessageBox(_T("暂不考虑终孔设计!"));
}

void RcuDesignDlg::OnBnClickedDesignOpenPores()
{
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	int row2 = ListCtrlHelper::GetCurSelOfList(m_list2);

	if(row1 == LB_ERR || row2 == LB_ERR)
	{
		MessageBox(_T("请指定或添加一个石门、钻场后再进行钻孔设计!"));
		return;
	}

	ItemData* pData1 = (ItemData*)m_list.GetItemData(row1);
	ItemData* pData2 = (ItemData*)m_list2.GetItemData(row2);

	//切换文档锁
	DocumentLockSwitch lock_switch;

	//提取石门数据
	RockGateLink rg_link;
	if(!RcuHelper::ReadRockGateData(pData1->objId, rg_link)) return;

	//提取钻场数据
	DrillSiteLink ds_link;
	if(!RcuHelper::ReadDrillSiteData(pData2->objId, ds_link)) return;

	//显示钻孔设计对话框
	if(UpdateOpenPoresFromDlg(pData2->objId, rg_link, ds_link))
	{
		//更新钻场列表中的数据
		ModifyDrillSiteToListCtrl(m_list2, row2, ds_link);
		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnBnClickedExport()
{
	// TODO: 在此添加控件通知处理程序代码
}

void RcuDesignDlg::updateRockGateListCtrl(unsigned int op, const AcDbObjectId& rock_gate)
{
	if(rock_gate.isNull()) return;

	int row1 = ListCtrlHelper::SearchRowOfList(m_list, rock_gate);
	if(op == 0)
	{
		if(row1 == LB_ERR)
		{
			//增加石门
			addRockGate(rock_gate);
		}
		else
		{
			//修改石门
			modifyRockGate(row1);
		}
	}
	else if(op == 1 && row1 != LB_ERR)
	{
		//删除石门
		delRockGate(row1);
	}
	else if(op == 2 && row1 != LB_ERR)
	{
		//修改石门
		modifyRockGate(row1);
	}
}

void RcuDesignDlg::updateDrillSiteListCtrl(unsigned int op, const AcDbObjectId& drill_site)
{
	if(drill_site.isNull()) return;

	int row2 = ListCtrlHelper::SearchRowOfList(m_list2, drill_site);
	if(op == 0)
	{
		if(row2 == LB_ERR)
		{
			//增加钻场
			addDrillSite(drill_site);
		}
		else
		{
			//修改钻场
			modifyDrillSite(row2);
		}
	}
	else if(op == 1 && row2 != LB_ERR)
	{
		//删除钻场
		delDrillSite(row2);
	}
	else if(op == 2 && row2 != LB_ERR)
	{
		//修改钻场
		modifyDrillSite(row2);
	}
}

LRESULT RcuDesignDlg::OnRcuAddMessage(WPARAM wParam, LPARAM lParam)
{
	ArxMsg* pMsg = (ArxMsg*)wParam;
	if(pMsg->clsName.CompareNoCase(_T("RockGate")) == 0)
	{
		updateRockGateListCtrl(0, pMsg->objId);
	}
	else if(pMsg->clsName.CompareNoCase(_T("DrillSite")) == 0)
	{
		updateDrillSiteListCtrl(0, pMsg->objId);
	}
	return 0;
} 

LRESULT RcuDesignDlg::OnRcuDelMessage(WPARAM wParam, LPARAM lParam)
{
	ArxMsg* pMsg = (ArxMsg*)wParam;
	if(pMsg->clsName.CompareNoCase(_T("RockGate")) == 0)
	{
		updateRockGateListCtrl(1, pMsg->objId);
	}
	else if(pMsg->clsName.CompareNoCase(_T("DrillSite")) == 0)
	{
		updateDrillSiteListCtrl(1, pMsg->objId);
	}
	return 0;
}

LRESULT RcuDesignDlg::OnRcuModifyMessage(WPARAM wParam, LPARAM lParam)
{
	ArxMsg* pMsg = (ArxMsg*)wParam;
	if(pMsg->clsName.CompareNoCase(_T("RockGate")) == 0)
	{
		updateRockGateListCtrl(2, pMsg->objId);
	}
	else if(pMsg->clsName.CompareNoCase(_T("DrillSite")) == 0)
	{
		updateDrillSiteListCtrl(2, pMsg->objId);
	}
	return 0;
}

LRESULT RcuDesignDlg::OnRcuUpdateMessage(WPARAM wParam, LPARAM lParam)
{
	//更新界面
	updateUI();
	return 0;
}

void RcuDesignDlg::addRockGate(const AcDbObjectId& rock_gate)
{
	//切换文档锁
	DocumentLockSwitch lock_switch;

	//读取石门数据
	RockGateLink rg_link;
	if(!RcuHelper::ReadRockGateData(rock_gate, rg_link)) return;

	//向list1中插入一行石门数据
	InsertRockGateToListCtrl(m_list, rock_gate, rg_link);
}

void RcuDesignDlg::delRockGate(int row1)
{
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//文档锁切换
		DocumentLockSwitch lock_switch;

		//删除图元
		//注:EraseObject2使用open/close删除才有效石门和关联的图元
		//   而EraseObject函数使用的是事务,不一定能有效删除!!!
		ArxEntityHelper::EraseObject2(pData1->objId, true);

		//删除选择的行
		ListCtrlHelper::DeleteListCtrlItem(m_list, row1);
		ListCtrlHelper::ClearListCtrl(m_list2);
	}
}

void RcuDesignDlg::modifyRockGate(int row1)
{
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData(row1);

		//文档锁切换
		DocumentLockSwitch lock_switch;

		RockGateLink rg_link;
		//提取石门关联的所有数据并填充到对话框中
		if(!RcuHelper::ReadRockGateData(pData1->objId, rg_link)) return;

		//修改当前选中石门的数据
		ModifyRockGateToListCtrl(m_list, row1, rg_link);
	}
}

void RcuDesignDlg::addDrillSite(const AcDbObjectId& drill_site)
{
	//文档锁切换
	DocumentLockSwitch lock_switch;

	//调用对话框获取钻场数据
	DrillSiteLink ds_link;
	if(!RcuHelper::ReadDrillSiteData(drill_site, ds_link)) return;

	//向list2中插入一行钻场数据
	InsertDrillSiteToListCtrl(m_list2, drill_site, ds_link);
}

void RcuDesignDlg::delDrillSite(int row2)
{
	if(row2 != LB_ERR)
	{
		ItemData* pData2 = ( ItemData* )m_list2.GetItemData(row2);

		//文档锁切换
		DocumentLockSwitch lock_switch;
		//删除钻场图元
		ArxEntityHelper::EraseObject2(pData2->objId, true);
		//删除选择的行
		ListCtrlHelper::DeleteListCtrlItem(m_list2, row2);
	}
}

void RcuDesignDlg::modifyDrillSite(int row2)
{
	if(row2 != LB_ERR)
	{
		ItemData* pData2 = ( ItemData* )m_list2.GetItemData(row2);

		//文档锁切换
		DocumentLockSwitch lock_switch;

		//提取钻场数据
		DrillSiteLink ds_link;
		if(!RcuHelper::ReadDrillSiteData(pData2->objId, ds_link)) return;

		//修改当前选中钻场的数据
		ModifyDrillSiteToListCtrl(m_list, row2, ds_link);
	}
}

void RcuDesignDlg::updateUI()
{
	//清空钻场列表和石门列表
	ListCtrlHelper::ClearListCtrl(m_list);
	ListCtrlHelper::ClearListCtrl(m_list2);

	//文档锁切换
	DocumentLockSwitch lock_switch;

	AcStringArray fields;
	FieldHelper::GetAllFields(_T("RockGate"), fields);
	if(fields.isEmpty()) return;

	AcDbObjectIdArray objIds;
	RcuHelper::FindAllRockGates(objIds);
	if(objIds.isEmpty()) return;

	ListCtrlHelper::InsertDatasToListCtrl(m_list, fields, objIds);

	//选中第1行,触发LVN_ITEMCHANGED消息
	m_list.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_list.EnsureVisible(0, FALSE);
	m_list.SetFocus();
}
