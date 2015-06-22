#pragma once

struct ItemData
{
	int iItem;            // 第几行的数据
	AcDbObjectId objId;   // 数据id
};

class ListCtrlHelper
{
public:
	static int SearchRowOfList(CListCtrl& m_list, const AcDbObjectId& objId);
	//列表中的当前选中的行
	static int GetCurSelOfList(CListCtrl& m_list);
	//构造列表的列表头
	static void BuildListCtrlHeadColumns(CListCtrl& m_list, const AcStringArray fields);
	//新增一行
	static int InsertRowToListCtrl(CListCtrl& m_list, const AcDbObjectId& objId);
	//从多个图元中提取数据并增加到列表中
	static void InsertDatasToListCtrl(CListCtrl& m_list, const AcStringArray fields, const AcDbObjectIdArray& objIds);
	//删除列表中的某一行数据
	static void DeleteListCtrlItem(CListCtrl& m_list, int row);
	//清空列表
	static void ClearListCtrl(CListCtrl& m_list);
};