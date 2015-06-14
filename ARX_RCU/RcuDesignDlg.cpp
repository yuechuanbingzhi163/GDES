#include "stdafx.h"
#include "RcuDesignDlg.h"
#include "RcuHelper.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

struct ItemData
{
	int iItem;            // 第几行的数据
	AcDbObjectId objId;   // 数据id
};

// RcuDesignDlg 对话框

IMPLEMENT_DYNAMIC(RcuDesignDlg, DockBarChildDlg)

RcuDesignDlg::RcuDesignDlg(CWnd* pParent /*=NULL*/)
	: DockBarChildDlg(RcuDesignDlg::IDD, pParent)
{

}

RcuDesignDlg::~RcuDesignDlg()
{
}

void RcuDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	DockBarChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_rcuList);
}


BEGIN_MESSAGE_MAP(RcuDesignDlg, DockBarChildDlg)
	ON_BN_CLICKED(IDC_BUTTON1, &RcuDesignDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &RcuDesignDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &RcuDesignDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &RcuDesignDlg::OnBnClickedButton4)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &RcuDesignDlg::OnHdnItemdblclickList1)
END_MESSAGE_MAP()

static void FillDatas( CListCtrl& m_list, const AcStringArray fields, const AcDbObjectIdArray& objIds )
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

static void ClearItemData( CListCtrl& m_list )
{
	int n = m_list.GetItemCount();
	for( int i = 0; i < n; i++ )
	{
		ItemData* pData = ( ItemData* )m_list.GetItemData( i );
		delete pData;
	}
}

static void BuildHeadColumns( CListCtrl& m_list, const AcStringArray fields )
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

static void CrackHeadColumnWidth( CListCtrl& m_list)
{
	for(int i=5;i<m_list.GetHeaderCtrl()->GetItemCount()+1;i++)
	{
		m_list.SetColumnWidth(i, int(m_list.GetColumnWidth(i)*1.5));
	}
	m_list.SetColumnWidth(7, int(m_list.GetColumnWidth(7)*1.5));
}

static void ClearList( CListCtrl& m_list )
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

BOOL RcuDesignDlg::OnInitDialog()
{
	DockBarChildDlg::OnInitDialog();

	m_rcuList.SetExtendedStyle( m_rcuList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	AcStringArray fields;
	FieldHelper::GetAllFields(_T("RockGate"), fields);
	// 构建Column
	BuildHeadColumns( m_rcuList, fields );
	CrackHeadColumnWidth(m_rcuList);

	acDocManager->lockDocument( curDoc() );

	AcDbObjectIdArray objIds;
	RcuHelper::FindAllRockGates(objIds);

	acDocManager->unlockDocument( curDoc() );

	FillDatas(m_rcuList, fields, objIds);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void RcuDesignDlg::OnClosing()
{
	//删除列表中每一行附带的数据(这些数据都是new出来的)
	ClearItemData(m_rcuList);
	// 清空链表
	//ClearList(m_rcuList);
}

// RcuDesignDlg 消息处理程序

void RcuDesignDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void RcuDesignDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
}

void RcuDesignDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
}

void RcuDesignDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
}

void RcuDesignDlg::OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

