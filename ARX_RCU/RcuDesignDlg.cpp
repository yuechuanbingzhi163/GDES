#include "stdafx.h"
#include "RcuDesignDlg.h"
#include "RcuEditDrillSiteDlg.h"

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
//#include "../DefGE/RockGate.h"
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
static void ModifyDrillSiteToListCtrl(CListCtrl& m_list, int n, DrillSiteLink& ds_link)
{
	{
		m_list.SetItemText( n, 1, ds_link.m_name );
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_width, value);
		m_list.SetItemText( n, 2, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_height, value);
		m_list.SetItemText( n, 3, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_dist, value);
		m_list.SetItemText( n, 4, value);
	}
	{
		m_list.SetItemText( n, 5, ArxUtilHelper::Point3dToString(ds_link.m_pt));
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_left, value);
		m_list.SetItemText( n, 6, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_right, value);
		m_list.SetItemText( n, 7, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_top, value);
		m_list.SetItemText( n, 8, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_bottom, value);
		m_list.SetItemText( n, 9, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_pore_size, value);
		m_list.SetItemText( n, 10, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(ds_link.m_start, value);
		m_list.SetItemText( n, 11, value);
	}
}

//向钻场列表中新增一行
static void InsertDrillSiteToListCtrl(CListCtrl& m_list, const AcDbObjectId& objId, DrillSiteLink& ds_link)
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


static bool UpdateDrillSiteDataFromDlg(const AcDbObjectId& drill_site, DrillSiteLink& ds_link, CoalSurfaceLink& cs_link)
{
	CAcModuleResourceOverride resourceOverride;

	//初始化钻场对话框
	RcuEditDrillSiteDlg dlg;
	dlg.m_drill_site = drill_site;
	//如果钻场id不为空,则读取数据并更新到对话框
	dlg.readFromDataLink(ds_link, cs_link);

	if(IDOK != dlg.DoModal()) return false;

	//从对话框中提取数据
	dlg.writeToDataLink(ds_link, cs_link);
	

	//将数据更新到图元中
	ds_link.updateData(true);
	cs_link.updateData(true);

	return true;
}


//格式化钻场钻孔设计对话框的标题
static CString FormatPoreDlgTitle(const CString& ds_name)
{
	CString name = ds_name;
	name.Trim();
	if(name.IsEmpty())
	{
		name = _T("***");
	}

	CString title;
	title.Format(_T("%s钻场-钻孔设计"), name);
	return title;
}


//static bool UpdateOpenPoresFromDlg(const AcDbObjectId& drill_site,  DrillSiteLink& ds_link)
//{
//	CAcModuleResourceOverride resourceOverride;
//	
//	//RcuEditOpenPoreDlg dlg;
//	////格式化钻孔设计对话框的标题
//	//dlg.m_title = FormatPoreDlgTitle(ds_link.m_name);
//	////设置钻孔半径
//	//dlg.m_pore_size = ds_link.m_pore_size;
//	////提取钻孔的个数
//	//AcDbObjectIdArray pores;
//	//RcuHelper::GetRelatedOpenPores(drill_site, pores);
//	//dlg.m_pore_num = pores.length();
//	////提取钻孔的间距
//	////if(!pores.isEmpty())
//	////{
//	////	dlg.m_pore_gap = ds_link.m_pore_gap;
//	////}
//	////else
//	////{
//	////	dlg.m_pore_gap = 0;
//	////}
//
//	//if(IDOK != dlg.DoModal()) return false;
//
//	////从对话框中读取数据
//	////钻孔间距
//	////ds_link.m_pore_gap = dlg.m_pore_gap;
//	////更新数据到钻孔
//	//ds_link.updateData(true);
//
//	////删除关联的所有钻孔
//	//RcuHelper::ClearRelatedOpenPores(drill_site);
//
//	//创建新的钻孔图元
//	return RcuHelper::CreateOpenPores(drill_site, ds_link);
//}

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
}

BEGIN_MESSAGE_MAP(RcuDesignDlg, DockBarChildDlg)
	ON_BN_CLICKED(IDC_BUTTON4, &RcuDesignDlg::OnBnClickedExport)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &RcuDesignDlg::OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &RcuDesignDlg::OnNMRclickList1)
	ON_COMMAND(ID_RCU_LIST_HILIGHT, &RcuDesignDlg::OnHilightDrillSiteCommand)
	ON_COMMAND(ID_RCU_LIST_MODIFY, &RcuDesignDlg::OnModifyDrillSiteCommand)
	ON_COMMAND(ID_RCU_LIST_DELETE, &RcuDesignDlg::OnDeleteDrillSiteCommand)
	ON_COMMAND(ID_RCU_LIST_ADD, &RcuDesignDlg::OnAddDrillSiteCommand)
	ON_COMMAND(ID_RCU_LIST_UPDATE, &RcuDesignDlg::OnUpdateDrillSiteListCommand)
	ON_COMMAND(ID_RCU_COAL_HILIGHT, &RcuDesignDlg::OnHilightCoalSurfCommand)
	ON_COMMAND(ID_RCU_COAL_SURF, &RcuDesignDlg::OnDisplayCoalSurfCommand)
	ON_COMMAND(ID_COAL_SURF_PORE_DESIGN, &RcuDesignDlg::OnDesiginClosePoresCommand)

	ON_COMMAND(IDR_DRILL_SITE_HILIGHT, &RcuDesignDlg::OnHilightDrillSiteCommand)
	//ON_COMMAND(IDR_DRILL_SITE_PORE_DESIGN, &RcuDesignDlg::OnDesiginOpenPoresCommand)
	//ON_COMMAND(IDR_DRILL_SITE_PORE_CLEAR, &RcuDesignDlg::OnClearOpenPoresCommand)

	ON_MESSAGE(WM_RCU_ADD, &RcuDesignDlg::OnRcuAddMessage)
	ON_MESSAGE(WM_RCU_DEL, &RcuDesignDlg::OnRcuDelMessage)
	ON_MESSAGE(WM_RCU_MODIFY, &RcuDesignDlg::OnRcuModifyMessage)
	ON_MESSAGE(WM_RCU_UPDATE, &RcuDesignDlg::OnRcuUpdateMessage)

END_MESSAGE_MAP()

BOOL RcuDesignDlg::OnInitDialog()
{
	DockBarChildDlg::OnInitDialog();

	m_list.SetExtendedStyle( m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	AcStringArray drill_fields;
	FieldHelper::GetAllFields(_T("DrillSite"), drill_fields);
	//构建表头列表
	ListCtrlHelper::BuildListCtrlHeadColumns( m_list, drill_fields );
	CrackHeadColumnWidth(m_list);

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

void RcuDesignDlg::OnAddDrillSiteCommand()
{
	//切换controlbar的显示
	ControlBarShowSwitch cb_switch(this);

	//切换文档锁
	DocumentLockSwitch lock_switch;

	//选择钻场的插入点坐标
	AcGePoint3d pt;
	if(!ArxUtilHelper::PromptPt(_T("\n请选择钻场的插入点坐标："), pt)) return;

	//调用对话框获取钻场和煤层数据
	DrillSiteLink ds_link;
	CoalSurfaceLink cs_link;
	if(!UpdateDrillSiteDataFromDlg(AcDbObjectId::kNull, ds_link, cs_link)) return;

	//创建钻场和煤层图元
	if(RcuHelper::CreateDrillSite(pt, ds_link, cs_link))
	{
		if(!RcuHelper::ModifyDrillSiteRelatedGEs(ds_link.getDataSource(), cs_link, ds_link)) return;

		//向list1中插入一行钻场数据
		InsertDrillSiteToListCtrl(m_list, ds_link.getDataSource(), ds_link);

		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnDeleteDrillSiteCommand()
{
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//文档锁切换
		DocumentLockSwitch lock_switch;

		//删除图元
		//注:EraseObject2使用open/close删除才有效钻场和关联的图元
		//   而EraseObject函数使用的是事务,不一定能有效删除!!!
		ArxEntityHelper::EraseObject2(pData1->objId, true);

		//删除选择的行
		ListCtrlHelper::DeleteListCtrlItem(m_list, row1);

		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnModifyDrillSiteCommand()
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
	
	//提取钻场和煤层数据
	DrillSiteLink ds_link;
	CoalSurfaceLink cs_link;
	if(!RcuHelper::GetDrillSiteAndCoalSurfDatas(pData1->objId, ds_link, cs_link))
	{
		MessageBox(_T("提取钻场或煤层失败!!!"));
		return;
	}

	//切换controlbar的显示
	ControlBarShowSwitch cb_switch(this);

	//从对话框中更新数据
	if(UpdateDrillSiteDataFromDlg(pData1->objId, ds_link, cs_link))
	{
		if (!RcuHelper::ModifyCoalSurfPt(pData1->objId)) return;
		if(!RcuHelper::ModifyDrillSiteRelatedGEs(pData1->objId, cs_link, ds_link)) return;

		//修改当前选中钻场的数据
		ModifyDrillSiteToListCtrl(m_list, row1, ds_link);
		//acutPrintf(_T("\n计算之前->宽度:%.4lf\t高度:%.4lf\n"),cs_link.m_width,cs_link.m_height);
		
		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnHilightDrillSiteCommand()
{
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//文档锁切换
		DocumentLockSwitch lock_switch;
		//高亮选中钻场图元
		ArxEntityHelper::SelectEntity(pData1->objId);

		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnUpdateDrillSiteListCommand()
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

void RcuDesignDlg::OnDisplayCoalSurfCommand()
{
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//文档锁切换
		DocumentLockSwitch lock_switch;

		AcDbObjectId coal_surf;
		RcuHelper::GetRelatedCoalSurface(pData1->objId, coal_surf);
		
		CoalSurfaceLink cs_link;
		if(RcuHelper::ReadCoalSurfaceData(coal_surf, cs_link))
		{
			CString msg;
			msg.Format(_T("厚度:%.3f\t倾角:%.3f\n"), cs_link.m_thick, cs_link.m_angle);
			msg.AppendFormat(_T("钻孔抽采半径:%.3f\n"), cs_link.m_gas_radius);
			msg.AppendFormat(_T("抽采范围宽度:%.3f\t高度:%.3f\n"), cs_link.m_width, cs_link.m_height);
			msg.AppendFormat(_T("抽采范围中心点坐标:(%.3f, %.3f, %.3f)\n"), cs_link.m_pt.x, cs_link.m_pt.y, cs_link.m_pt.z);
			msg.AppendFormat(_T("平面法向量:(%.3f, %.3f, %.3f)\n"), cs_link.m_normV.x, cs_link.m_normV.y, cs_link.m_normV.z);
			msg.AppendFormat(_T("煤层走向向量:(%.3f, %.3f, %.3f)\n"), cs_link.m_headV.x, cs_link.m_headV.y, cs_link.m_headV.z);
			msg.AppendFormat(_T("煤层倾向向量:(%.3f, %.3f, %.3f)"), cs_link.m_dipV.x, cs_link.m_dipV.y, cs_link.m_dipV.z);
			MessageBox(msg);
		}
	}
}

void RcuDesignDlg::OnDesiginClosePoresCommand()
{
	int row1 = ListCtrlHelper::GetCurSelOfList(m_list);
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//文档锁切换
		DocumentLockSwitch lock_switch;

		AcDbObjectId coal_surf;
		RcuHelper::GetRelatedCoalSurface(pData1->objId, coal_surf);

		//删除所有的煤层钻孔
		RcuHelper::ClearRelatedClosePores(coal_surf);
		
		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnBnClickedExport()
{
	// TODO: 在此添加控件通知处理程序代码
}

void RcuDesignDlg::updateDrillSiteListCtrl(unsigned int op, const AcDbObjectId& drill_site)
{
	if(drill_site.isNull()) return;

	int row1 = ListCtrlHelper::SearchRowOfList(m_list, drill_site);
	if(op == 0)
	{
		if(row1 == LB_ERR)
		{
			//增加钻场
			addDrillSite(drill_site);
		}
		else
		{
			//修改钻场
			modifyDrillSite(row1);
		}
	}
	else if(op == 1 && row1 != LB_ERR)
	{
		//删除钻场
		delDrillSite(row1);
	}
	else if(op == 2 && row1 != LB_ERR)
	{
		//修改钻场
		modifyDrillSite(row1);
	}
}

LRESULT RcuDesignDlg::OnRcuAddMessage(WPARAM wParam, LPARAM lParam)
{
	ArxMsg* pMsg = (ArxMsg*)wParam;
	if(pMsg->clsName.CompareNoCase(_T("DrillSite")) == 0)
	{
		updateDrillSiteListCtrl(0, pMsg->objId);
	}
	return 0;
} 

LRESULT RcuDesignDlg::OnRcuDelMessage(WPARAM wParam, LPARAM lParam)
{
	ArxMsg* pMsg = (ArxMsg*)wParam;
	if(pMsg->clsName.CompareNoCase(_T("DrillSite")) == 0)
	{
		updateDrillSiteListCtrl(1, pMsg->objId);
	}
	return 0;
}

LRESULT RcuDesignDlg::OnRcuModifyMessage(WPARAM wParam, LPARAM lParam)
{
	ArxMsg* pMsg = (ArxMsg*)wParam;
	if(pMsg->clsName.CompareNoCase(_T("DrillSite")) == 0)
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

void RcuDesignDlg::addDrillSite(const AcDbObjectId& drill_site)
{
	//切换文档锁
	DocumentLockSwitch lock_switch;

	//读取钻场数据
	DrillSiteLink ds_link;
	if(!RcuHelper::ReadDrillSiteData(drill_site, ds_link)) return;

	//向list1中插入一行钻场数据
	InsertDrillSiteToListCtrl(m_list, drill_site, ds_link);
}

void RcuDesignDlg::delDrillSite(int row1)
{
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData( row1 );

		//文档锁切换
		DocumentLockSwitch lock_switch;

		//删除图元
		//注:EraseObject2使用open/close删除才有效钻场和关联的图元
		//   而EraseObject函数使用的是事务,不一定能有效删除!!!
		ArxEntityHelper::EraseObject2(pData1->objId, true);

		//删除选择的行
		ListCtrlHelper::DeleteListCtrlItem(m_list, row1);
	}
}

void RcuDesignDlg::modifyDrillSite(int row1)
{
	if(row1 != LB_ERR)
	{
		ItemData* pData1 = ( ItemData* )m_list.GetItemData(row1);

		//文档锁切换
		DocumentLockSwitch lock_switch;

		DrillSiteLink ds_link;
		//提取钻场关联的所有数据并填充到对话框中
		if(!RcuHelper::ReadDrillSiteData(pData1->objId, ds_link)) return;

		//修改当前选中钻场的数据
		ModifyDrillSiteToListCtrl(m_list, row1, ds_link);
	}
}

void RcuDesignDlg::updateUI()
{
	//清空钻场列表和钻场列表
	ListCtrlHelper::ClearListCtrl(m_list);

	//文档锁切换
	DocumentLockSwitch lock_switch;

	AcStringArray fields;
	FieldHelper::GetAllFields(_T("DrillSite"), fields);
	if(fields.isEmpty()) return;

	AcDbObjectIdArray objIds;
	RcuHelper::FindAllDrillSite(objIds);
	if(objIds.isEmpty()) return;

	ListCtrlHelper::InsertDatasToListCtrl(m_list, fields, objIds);

	//选中第1行,触发LVN_ITEMCHANGED消息
	m_list.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_list.EnsureVisible(0, FALSE);
	m_list.SetFocus();
}
