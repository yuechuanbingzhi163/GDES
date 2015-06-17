#include "stdafx.h"
#include "RcuDesignDlg.h"
#include "RcuNewDlg.h"
#include "RcuEditDlg.h"
#include "RcuHelper.h"
#include "RcuDataLink.h"
#include "Rcu.h"

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

static bool CaculCoalFaceParam(const RockGateLink& rg_link,CoalSurfaceLink& cs_link)
{
	AcGePoint3d orign;
	ArxUtilHelper::StringToPoint3d(rg_link.m_pt,orign);
	Rcu rcu;
	rcu.setOrigin(orign);
	rcu.setRockGateParams(rg_link.m_dist,rg_link.m_top,rg_link.m_bottom,rg_link.m_left,rg_link.m_right);
	rcu.setTunnelParams(rg_link.m_height,rg_link.m_width,rg_link.m_width);
	//第二个参数是石门轴线与煤层走向的夹角
	//但是界面上暂时没有，所以考虑90度的
	double angle = DegToRad(cs_link.m_angle);
	rcu.setCoalParams(angle,PI*0.5,cs_link.m_thick);
	double w,h;
	rcu.drillExtent(w,h);
	cs_link.m_width = w;
	cs_link.m_height = h;
	acutPrintf(_T("\n计算时->宽度:%.4lf\t高度:%.4lf\n"),cs_link.m_width,cs_link.m_height);
	return true;
}

static bool GetRockGateDataFromDlg(RockGateLink& rg_link, CoalSurfaceLink& cs_link)
{
	CAcModuleResourceOverride resourceOverride;
	RcuNewDlg dlg;
	if(IDOK != dlg.DoModal()) return false;

	//从对话框中提取数据
	dlg.writeToDataLink(rg_link, cs_link);

	return true;
}

static bool ShowRcuEditDlg(const AcDbObjectId& rock_gate)
{
	CAcModuleResourceOverride resourceOverride;
	RcuEditDlg dlg;
	//关联一个石门
	dlg.m_rock_gate = rock_gate;
	return (IDOK == dlg.DoModal());
}

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
		CString value;
		ArxUtilHelper::DoubleToString(rg_link.m_radius, value);
		m_list.SetItemText( n, 6, value);
	}
	{
		m_list.SetItemText( n, 7, rg_link.m_pt);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(rg_link.m_left, value);
		m_list.SetItemText( n, 8, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(rg_link.m_right, value);
		m_list.SetItemText( n, 9, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(rg_link.m_top, value);
		m_list.SetItemText( n, 10, value);
	}
	{
		CString value;
		ArxUtilHelper::DoubleToString(rg_link.m_bottom, value);
		m_list.SetItemText( n, 11, value);
	}
}

static void InsertRockGateToListCtrl(CListCtrl& m_list, const AcDbObjectId& objId, RockGateLink& rg_link)
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

	//修改一行数据
	ModifyRockGateToListCtrl(m_list, n, rg_link);

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

static void InsertDatasToListCtrl( CListCtrl& m_list, const AcStringArray fields, const AcDbObjectIdArray& objIds )
{
	//首先删除所有行
	m_list.DeleteAllItems();

	int len = objIds.length();
	for( int i = 0; i < len; i++ )
	{
		m_list.InsertItem( i, _T( "xx" ) );

		CString num;
		num.Format( _T( "%d" ), i + 1 );
		m_list.SetItemText( i, 0, num );

		ItemData* pData = new ItemData();
		pData->iItem = i;
		pData->objId = objIds[i];
		m_list.SetItemData( i, ( LPARAM )pData ); // 设置数据

		int n = fields.length();
		for( int j = 0; j < n; j++ )
		{
			//m_list.SetItemData(i, i);
			CString value;
			DataHelper::GetPropertyData( objIds[i], fields[j].kACharPtr(), value );
			m_list.SetItemText( i, j + 1, value );
		}
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

static void CrackHeadColumnWidth(CListCtrl& m_list)
{
	m_list.SetColumnWidth(1, int(m_list.GetColumnWidth(1)*1.2));
	for(int i=5;i<m_list.GetHeaderCtrl()->GetItemCount()+1;i++)
	{
		m_list.SetColumnWidth(i, int(m_list.GetColumnWidth(i)*1.5));
	}
	m_list.SetColumnWidth(7, int(m_list.GetColumnWidth(7)*1.5));
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

// RcuDesignDlg 对话框

IMPLEMENT_DYNAMIC(RcuDesignDlg, DockBarChildDlg)

RcuDesignDlg::RcuDesignDlg(CWnd* pParent /*=NULL*/)
	: DockBarChildDlg(RcuDesignDlg::IDD, pParent)
{

}

RcuDesignDlg::~RcuDesignDlg()
{
	ClearListCtrlItemDatas(m_list);
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
	ON_COMMAND(ID_RCU_LIST_HILIGHT, &RcuDesignDlg::OnHilightCommand)
	ON_COMMAND(ID_RCU_LIST_MODIFY, &RcuDesignDlg::OnModifyCommand)
	ON_COMMAND(ID_RCU_LIST_DELETE, &RcuDesignDlg::OnDeleteCommand)
	ON_COMMAND(ID_RCU_LIST_ADD, &RcuDesignDlg::OnAddCommand)
END_MESSAGE_MAP()

BOOL RcuDesignDlg::OnInitDialog()
{
	DockBarChildDlg::OnInitDialog();

	m_list.SetExtendedStyle( m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	AcStringArray fields;
	FieldHelper::GetAllFields(_T("RockGate"), fields);
	// 构建Column
	BuildListCtrlHeadColumns( m_list, fields );
	CrackHeadColumnWidth(m_list);

	acDocManager->lockDocument( curDoc() );

	AcDbObjectIdArray objIds;
	RcuHelper::FindAllRockGates(objIds);

	acDocManager->unlockDocument( curDoc() );

	InsertDatasToListCtrl(m_list, fields, objIds);

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

void RcuDesignDlg::OnBnClickedExport()
{
	// TODO: 在此添加控件通知处理程序代码
}

void RcuDesignDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
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

void RcuDesignDlg::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//第几行
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
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

void RcuDesignDlg::OnHilightCommand()
{
	//MessageBox(_T("RcuDesignDlg::OnHilightCommand"));
	int row = GetCurSelOfList(m_list);
	if(row != LB_ERR)
	{
		ItemData* pData = ( ItemData* )m_list.GetItemData( row );

		//高亮选中石门图元
		acDocManager->lockDocument( curDoc() );
		ArxEntityHelper::SelectEntity(pData->objId);
		acDocManager->unlockDocument( curDoc() );

		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnModifyCommand()
{
	//MessageBox(_T("RcuDesignDlg::OnLocateCommand"));
	int row = GetCurSelOfList(m_list);
	if(row == LB_ERR)
	{
		MessageBox( _T( "请确保当前有一行被选中!" ) );
	}
	else
	{
		ItemData* pData = ( ItemData* )m_list.GetItemData( row );

		//切换controlbar的显示
		ControlBarShowSwitch cb_switch(this);

		acDocManager->lockDocument( curDoc() );
		//用户点击了"确认"按钮
		//需要重新更新listctrl中的数据
		if(ShowRcuEditDlg(pData->objId))
		{
			//从石门中提取数据
			RockGateLink rg_link;
			rg_link.setDataSource(pData->objId);
			rg_link.updateData(false);
			//修改当前选中行的数据
			ModifyRockGateToListCtrl(m_list, row, rg_link);

			CoalSurfaceLink cs_link;
			cs_link.updateData(false);
			acutPrintf(_T("\n计算之前->宽度:%.4lf\t高度:%.4lf\n"),cs_link.m_width,cs_link.m_height);
			CaculCoalFaceParam(rg_link,cs_link);
			cs_link.updateData(true);
			acutPrintf(_T("\n计算之后->宽度:%.4lf\t高度:%.4lf\n"),cs_link.m_width,cs_link.m_height);

		}

		acDocManager->unlockDocument( curDoc() );
		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnDeleteCommand()
{
	//MessageBox(_T("RcuDesignDlg::OnDeleteCommand"));
	int row = GetCurSelOfList(m_list);
	if(row != LB_ERR)
	{
		ItemData* pData = ( ItemData* )m_list.GetItemData( row );

		//删除石门图元
		acDocManager->lockDocument( curDoc() );
		ArxEntityHelper::EraseObject(pData->objId, true);
		acDocManager->unlockDocument( curDoc() );

		//删除选择的行
		ClearListCtrlItem(m_list, row);
		//cad窗口获取焦点
		acedGetAcadFrame()->SetFocus();
	}
}

void RcuDesignDlg::OnAddCommand()
{
	//非模态对话框AutoCAD获得焦点
	acedGetAcadDwgView()->SetFocus();

	acDocManager->lockDocument( curDoc() );

	//切换controlbar的显示
	ControlBarShowSwitch cb_switch(this);

	AcGePoint3d pt;
	if(!ArxUtilHelper::PromptPt(_T("\n请选择石门的插入点坐标："), pt)) return;

	//创建Datalink对象,减少字符串格式化代码
	RockGateLink rg_link;
	CoalSurfaceLink cs_link;
	if(!GetRockGateDataFromDlg(rg_link, cs_link)) return;

	//新建石门并设置插入点坐标
	RockGate* pRG = new RockGate();
	pRG->setInsertPt(pt);

	CoalSurface* pCS = new CoalSurface();
	pCS->setInsertPt(AcGePoint3d(pt.x, pt.y+500, pt.z));

	//添加石门到cad图形数据库
	if(!ArxUtilHelper::PostToModelSpace(pRG))
	{
		delete pRG; pRG = 0;
	}
	else
	{
		acutPrintf(_T("\n计算之前->宽度:%.4lf\t高度:%.4lf\n"),cs_link.m_width,cs_link.m_height);
		CaculCoalFaceParam(rg_link,cs_link);
		cs_link.updateData(true);
		acutPrintf(_T("\n计算之后->宽度:%.4lf\t高度:%.4lf\n"),cs_link.m_width,cs_link.m_height);

		//煤层关联到石门
		pCS->setRelatedGE(pRG->objectId());
		//添加煤层到cad图形数据库
		if(!ArxUtilHelper::PostToModelSpace(pCS))
		{
			delete pCS; pCS = 0;
			//煤层提交失败,则删除已添加的石门
			ArxEntityHelper::EraseObject(pRG->objectId(), true);
		}
		else
		{
			//通过DataLink关联数据对象,并写入到图元中
			rg_link.setDataSource(pRG->objectId());
			rg_link.updateData(true);
			cs_link.setDataSource(pCS->objectId());
			cs_link.updateData(true);

			//向listctrl中插入一行
			InsertRockGateToListCtrl(m_list, pRG->objectId(), rg_link);
		}
	}
	acDocManager->unlockDocument( curDoc() );
}