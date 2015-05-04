#include "SqliteHelper.h"

bool GetPumpTypeTable( CString sql, CString szDbPath, TypeTableVector& msg )
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
		TypeTable typeTable;
		typeTable.id = Reader.GetIntValue(1);
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

bool GetPumpPropertyTable( CString sql, CString szDbPath, PropertyTableVector& msg )
{
	SQLite sqlite;
	if(!sqlite.Open(szDbPath))
	{
		return false;
	}

	SQLiteDataReader Reader = sqlite.ExcuteQuery(sql);

	int index = 0;
	while(Reader.Read())
	{
		PropertyTable propertyTable;
		propertyTable.id = Reader.GetIntValue(1);
		propertyTable.speed = Reader.GetIntValue(2);
		propertyTable.power = Reader.GetFloatValue(3);
		propertyTable.maxQ = Reader.GetFloatValue(4);
		propertyTable.maxP = Reader.GetIntValue(5);
		propertyTable.absP = Reader.GetIntValue(6);
		msg.push_back(propertyTable);
	}
	Reader.Close();
	return true;
}