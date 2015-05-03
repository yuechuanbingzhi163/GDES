#include "stdafx.h"
#include "DataListDlg.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"
#include "../GraphTool/GraphAlgo.h"

struct ItemData
{
    int iItem;            // 第几行的数据
	AcDbObjectIdArray path; // 路径
};

// 用于CtrlList的排序函数
struct SortData
{
    int iSubItem;
    CListCtrl* pList;
};

static int nSortOrder = 1;      // 排序的方式,1升序，-1降序。

int CALLBACK SortFunc( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
    ItemData* pData1 = ( ItemData* )lParam1;
    ItemData* pData2 = ( ItemData* )lParam2;
    SortData* pSD = ( SortData* )lParamSort;

    // 采用最简单的办法，不考虑具体是哪一栏的数据
    // 按照字符串排序即可

    CString s1 = pSD->pList->GetItemText( pData1->iItem, pSD->iSubItem );
    CString s2 = pSD->pList->GetItemText( pData2->iItem, pSD->iSubItem );

    int nRetVal = 0;

    if( pSD->iSubItem == 0 )
    {
        // 第1栏(编号)
        int n1 = _ttoi( s1 );
        int n2 = _ttoi( s2 );
        nRetVal = ( ( n1 < n2 ) ? -1 : 1 );
    }
    else
    {
        nRetVal = s1.CompareNoCase( s2 );
    }

    return nRetVal * nSortOrder;
}

static void AdjustItemDataNum( CListCtrl& m_list )
{
    // 因为排序可能导致iItem与实际的行位置不一致，需要调整
    int n = m_list.GetItemCount();
    for( int i = 0; i < n; i++ )
    {
        ItemData* pData = ( ItemData* )m_list.GetItemData( i );
        pData->iItem = i;
    }
}

static void GetAllRegTypes( AcStringArray& types )
{
    DrawHelper::GetAllRegGETypesForDraw( types );
}

static void GetTypeDxfNames( const AcStringArray& types, AcStringArray& dxfNames )
{
    int len = types.length();
    for( int i = 0; i < len; i++ )
    {
        AcRxClass* pClass = AcRxClass::cast( acrxClassDictionary->at( types[i].kACharPtr() ) );
        if( pClass != 0 )
        {
            dxfNames.append( pClass->dxfName() );
        }
    }
}

static void FillTypeList( AcStringArray& types, CComboBox& m_list )
{
    GetAllRegTypes( types );

    AcStringArray dxfNames;
    GetTypeDxfNames( types, dxfNames );

    assert( types.length() == dxfNames.length() );

    int len = dxfNames.length();
    for( int i = 0; i < len; i++ )
    {
        m_list.AddString( dxfNames[i].kACharPtr() );
    }

	LPCTSTR lpszmyString = _T("瓦斯抽采管路");

	// Delete all items that begin with the specified string. 
	int nItem = 0;
	nItem = m_list.FindString(nItem, lpszmyString);
	m_list.SetCurSel(nItem);
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

static void BuildHeadColumns( CListCtrl& m_list, const AcStringArray fields )
{
    int len = fields.length();

    CRect rect;
    m_list.GetClientRect( &rect );

    int n = len;
    if( n > 10 ) n = 10;
    double dw = rect.Width() / ( n + 1 );

    m_list.InsertColumn( 0, _T( "编号" ), LVCFMT_LEFT, dw );
    for( int i = 0; i < len; i++ )
    {
        m_list.InsertColumn( i + 1, fields[i].kACharPtr(), LVCFMT_LEFT, dw );
    }
}

//计算路径的总摩擦阻力
static double CaclPathF1(const AcDbObjectIdArray& path)
{
	double sumFrinction = 0;
	for( int i = 0; i < path.length(); i++ )
	{
		CString friction;
		DataHelper::GetPropertyData(path[i],_T("管路的摩擦阻力"),friction);
		sumFrinction += _tstof(friction);
		//acutPrintf(_T("\n管路[%d]:%.2lf"),i+1,sumFrinction);
	}
	return sumFrinction;
}

//计算路径的总局部阻力
static double CaclPathF2(const AcDbObjectIdArray& path)
{
	double sumPartFrinction = 0;
	for( int i = 0; i < path.length(); i++ )
	{
		CString friction;
		DataHelper::GetPropertyData(path[i],_T("管路的局部阻力"),friction);
		sumPartFrinction += _tstof(friction);
		//acutPrintf(_T("\n管路[%d]:%.2lf"),i+1,sumPartFrinction);
	}
	return sumPartFrinction;
}

static void FillDatas( CListCtrl& m_list, const AcDbObjectId& sObjId, const AcDbObjectId& tObjId)
{
	AcDbIntArray pathNums;
	AcDbObjectIdArray allPaths;
	GraphAlgo::FindGasTubePaths(sObjId, tObjId, pathNums, allPaths);

	int len = pathNums.length();
	int start = 0;
	for( int i = 0; i < len; i++ )
	{
		//acutPrintf(_T("\n插入第%d行数据"), i+1);
		m_list.InsertItem( i, _T( "xx" ) );

		//设置编号(第1列)
		CString num;
		num.Format( _T( "%d" ), i + 1 );
		m_list.SetItemText( i, 0, num );

		//设置路径(第2列)
		AcDbObjectIdArray path;
		CString pathStr;
		int end = start+pathNums[i];
		for(int j=start; j<end;j++)
		{
			//记录路径
			path.append(allPaths[j]);

			//需要给管路额外增加一个字段
			AcDbObjectId objId = allPaths[j];
			CString name;
			DataHelper::GetPropertyData(objId, _T("管路编号"), name);
			if(name.IsEmpty())
			{
				name = _T("NULL");
			}
			pathStr.Append(name);
			if(j < end-1)
			{
				pathStr.Append(_T("->"));
			}
		}
		start = end;
		//设置第2列的路径
		m_list.SetItemText( i, 1, pathStr );

		//计算路径的总摩擦阻力
		double F1 = CaclPathF1(path);
		//计算路径的总局部阻力
		double F2 = CaclPathF2(path);

		//设置总摩擦阻力(第3列)
		CString value;
		value.Format( _T( "%.3f" ), F1 );
		m_list.SetItemText( i, 2, value );

		//设置总摩擦阻力(第4列)
		value.Format( _T( "%.3f" ), F2 );
		m_list.SetItemText( i, 3, value );

		//设置第i行的itemdata
		ItemData* pData = new ItemData();
		pData->iItem = i;
		//ItemData也需要记录路径,方便后续使用
		pData->path.append(path);
		m_list.SetItemData( i, ( LPARAM )pData ); // 设置数据
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

IMPLEMENT_DYNAMIC( DataListDlg, DockBarChildDlg )

DataListDlg::DataListDlg( CWnd* pParent /*=NULL*/ )
    : DockBarChildDlg( DataListDlg::IDD, pParent )
{

}

DataListDlg::~DataListDlg()
{
}

void DataListDlg::DoDataExchange( CDataExchange* pDX )
{
    DockBarChildDlg::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_TYPE_LIST, m_typeList );
    DDX_Control( pDX, IDC_DATA_LIST, m_geList );
}

BEGIN_MESSAGE_MAP( DataListDlg, DockBarChildDlg )
    //{{AFX_MSG_MAP(DataListDlg)
    //}}AFX_MSG_MAP

    ON_BN_CLICKED( IDC_DATA_LIST_UPDATE_BTN, &DataListDlg::OnBnClickedUpdateBtn )
    //ON_BN_CLICKED(IDC_GE_LIST_READ_TYPE_BTN, &DataListDlg::OnBnClickedGeListReadTypeBtn)
    ON_NOTIFY( LVN_ITEMCHANGED, IDC_DATA_LIST, &DataListDlg::OnLvnItemchangedGeList )
    ON_NOTIFY( LVN_COLUMNCLICK, IDC_DATA_LIST, &DataListDlg::OnLvnColumnclickGeList )
END_MESSAGE_MAP()

BOOL DataListDlg::OnInitDialog()
{
    DockBarChildDlg::OnInitDialog();

    m_geList.SetExtendedStyle( m_geList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

    m_types.removeAll();
    m_typeList.ResetContent();

    FillTypeList( m_types, m_typeList );

    return TRUE;
}

void DataListDlg::OnClosing()
{
    //m_types.removeAll();
    //m_typeList.ResetContent();

    ClearItemData( m_geList );
    // 清空链表
    ClearList( m_geList );

	sObjId = AcDbObjectId::kNull;
	tObjId = AcDbObjectId::kNull;
}

//void DataListDlg::OnBnClickedGeListReadTypeBtn()
//{
//	m_types.removeAll();
//	m_typeList.ResetContent();
//
//	FillTypeList(m_types, m_typeList);
//}

static void GetGasTunnelFields(AcStringArray& fields)
{
	fields.append(_T("路径"));
	fields.append(_T("总摩擦阻力"));
	fields.append(_T("总局部阻力"));
}

void DataListDlg::OnBnClickedUpdateBtn()
{
    // 删除所有的数据(item data)
    ClearItemData( m_geList );

    // 清空链表
    ClearList( m_geList );

	int indx = m_typeList.GetCurSel();
	if(CB_ERR == indx)
	{
		AfxMessageBox(_T("没有选择图元类型"));
		return;
	}
    // 获取类型
    CString type = m_types[indx].kACharPtr();

    acDocManager->lockDocument( curDoc() );

    AcStringArray fields;
	//设置瓦斯管路路径的column名称
	GetGasTunnelFields(fields);
	// 获取所有的字段
    //FieldHelper::GetAllFields( type, fields );

    // 构建Column
    BuildHeadColumns( m_geList, fields );

    m_initFinished = false;

    // 填充数据
	FillDatas(m_geList, sObjId, tObjId);

    // 数据初始化完毕
    m_initFinished = true;

    acDocManager->unlockDocument( curDoc() );
}

//#include "../MineGE/LinkedGE.h"
//#include "../ArxHelper/HelperClass.h"
void DataListDlg::OnLvnItemchangedGeList( NMHDR* pNMHDR, LRESULT* pResult )
{
    if( !m_initFinished ) return;

    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );

    int row = pNMLV->iItem;
    if( row != LB_ERR )
    {
        ItemData* pData = ( ItemData* )m_geList.GetItemData( row );
        //acutPrintf(_T("\nid:%d"), pData->objId);

        acDocManager->lockDocument( curDoc() );

		//缩放并选中这条路径
		if(ArxEntityHelper::SelectEntities(pData->path))
		{
			ArxEntityHelper::ZoomToEntities(pData->path);
		}

        acDocManager->unlockDocument( curDoc() );
    }

    *pResult = 0;
}

//排序消息响应
void DataListDlg::OnLvnColumnclickGeList( NMHDR* pNMHDR, LRESULT* pResult )
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>( pNMHDR );

    static int ncurSortCol = -1;      // 保存当前的排序列

    /*
     * 一开始表示为-1，表示尚未按任何列排序
     * 若当前排序列与点击列相同，则更改增、减序的排序方式。
     * 若点击列与当前排序列不同的列，则改变排序序，并将排序方式改为增序
     */
    if( ncurSortCol == pNMLV->iSubItem )
    {
        nSortOrder *= -1;
    }
    else
    {
        ncurSortCol = pNMLV->iSubItem;
        nSortOrder = 1;
    }
    AdjustItemDataNum( m_geList );

    SortData sd = {pNMLV->iSubItem, &m_geList};
    m_geList.SortItems( SortFunc, ( LPARAM )&sd ); // 排序

    *pResult = 0;
}