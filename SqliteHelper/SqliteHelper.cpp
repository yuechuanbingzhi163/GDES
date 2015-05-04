#include "SqliteHelper.h"

bool GetPumpMessage( CString sql, CString szDbPath, std::vector<TypeTable>& msg )
{
	SQLite sqlite;
	if(!sqlite.Open(szDbPath))
	{
		//_tprintf(_T("%s\n"),sqlite.GetLastErrorMsg());
		return false;
	}

	SQLiteDataReader Reader = sqlite.ExcuteQuery(sql);

	int index = 0;
	while(Reader.Read())
	{
		//acutPrintf( _T("***************【第%d条记录】***************\n"),++index);
		//acutPrintf( _T("字段名:%s 字段值:%d\n"),Reader.GetName(0),Reader.GetIntValue(0));
		//acutPrintf( _T("字段名:%s 字段值:%s\n"),Reader.GetName(1),Reader.GetStringValue(1));
		//acutPrintf( _T("字段名:%s 字段值:%s\n"),Reader.GetName(2),Reader.GetStringValue(2));
		//acutPrintf( _T("字段名:%s 字段值:%d\n"),Reader.GetName(3),Reader.GetIntValue(3));
		//acutPrintf( _T("字段名:%s 字段值:%s\n"),Reader.GetName(4),Reader.GetStringValue(4));
		//acutPrintf( _T("字段名:%s 字段值:%s\n"),Reader.GetName(5),Reader.GetStringValue(5));
		//acutPrintf( _T("*******************************************\n"));
		TypeTable typeTable;
		typeTable.id = Reader.GetIntValue(1);
		//_tcscpy(typeTable.type,Reader.GetStringValue(2));
		typeTable.type = Reader.GetStringValue(2);
		typeTable.absP = Reader.GetIntValue(3);
		typeTable.weight = Reader.GetIntValue(4);
		typeTable.length = Reader.GetIntValue(5);
		typeTable.weidth = Reader.GetIntValue(6);
		typeTable.heigth = Reader.GetIntValue(7);
		typeTable.factName = Reader.GetStringValue(8);
		msg.push_back(typeTable);
	}
	Reader.Close();
return true;
}