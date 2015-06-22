#include "StdAfx.h"
#include "ListCtrlHelper.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/HelperClass.h"

//获取当前选择的行
int ListCtrlHelper::GetCurSelOfList(CListCtrl& m_list)
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

int ListCtrlHelper::SearchRowOfList(CListCtrl& m_list, const AcDbObjectId& objId)
{
	int row = LB_ERR;
	for( int i = 0; i < m_list.GetItemCount(); i++ )
	{
		ItemData* pData = ( ItemData* )m_list.GetItemData( i );
		if(pData->objId == objId)
		{
			row = i; break;
		}
	}
	return row;
}

void ListCtrlHelper::BuildListCtrlHeadColumns( CListCtrl& m_list, const AcStringArray fields )
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

int ListCtrlHelper::InsertRowToListCtrl(CListCtrl& m_list, const AcDbObjectId& objId)
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

void ListCtrlHelper::InsertDatasToListCtrl( CListCtrl& m_list, const AcStringArray fields, const AcDbObjectIdArray& objIds )
{
	int len = objIds.length();
	for( int i = 0; i < len; i++ )
	{
		//增加一行
		InsertRowToListCtrl(m_list, objIds[i]);

		//设置各列的数据
		int n = fields.length();
		for( int j = 0; j < n; j++ )
		{
			//m_list2.SetItemData(i, i);
			CString value;
			DataHelper::GetPropertyData( objIds[i], fields[j].kACharPtr(), value );
			m_list.SetItemText( i, j + 1, value );
		}
	}
}

void ListCtrlHelper::DeleteListCtrlItem( CListCtrl& m_list, int row )
{
	int n = m_list.GetItemCount();
	if(row != LB_ERR && n > 0 && row < n) 
	{
		ItemData* pData = ( ItemData* )m_list.GetItemData( row );
		delete pData;
		m_list.DeleteItem(row);
	}
}

void ListCtrlHelper::ClearListCtrl( CListCtrl& m_list )
{
	int n = m_list.GetItemCount();
	for( int i = 0; i < n; i++ )
	{
		ItemData* pData = ( ItemData* )m_list.GetItemData( i );
		delete pData;
	}
	m_list.DeleteAllItems();
}
