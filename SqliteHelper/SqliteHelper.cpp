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
	sqlite.Close();
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
	sqlite.Close();
	return true;
}

static void GetPumpNewColId( const CString& tableName, SQLite& sqlite, int& newId )
{
	//CString sql = _T("select count(id) from ") + tableName;
	CString sql = _T("select * from ") + tableName + _T(" limit 1 offset (select count(*)-1  from ") + tableName + _T(")");
	SQLiteDataReader Reader = sqlite.ExcuteQuery(sql);

	Reader.Read();
	int id = Reader.GetIntValue(1);
	Reader.Close();

	newId = id;
	//CString strId;
	//strId.Format(_T("%d"),id);
	//sql = _T("select * from ") + tableName + _T(" where id = ") + strId;
	//SQLiteDataReader Reader2 = sqlite.ExcuteQuery(sql);

	//Reader2.Read();
	//newId = Reader2.GetIntValue(1);
	//Reader2.Close();

	//sqlite.Close();
}

static bool HaveSameProperty( const CString& szDbPath,const PropertyTable& pt )
{
	CString sql = _T("select * from PropertyTable");
	PropertyTableVector ptV;
	GetPumpPropertyTable(sql,szDbPath,ptV);
	int num = 0;
	for (int i = 0; i < ptV.size(); i++)
	{
		PropertyTable oldPT = ptV[i];
		if(oldPT == pt)
		{
			num += 1;
		}
	}
	if( num <= 0 ) return false;
	else return true;
}

bool InsertPumpToPropertyTable( const CString& szDbPath, const PropertyTable& pt )
{
	//有相同的数据
	if(HaveSameProperty(szDbPath,pt)) return false;
	SQLite sqlite;
	if(!sqlite.Open(szDbPath))
	{
		return false;
	}


	sqlite.BeginTransaction();
	CString sql = _T("insert into PropertyTable(catagory_id,speed,power,maxQ,maxP,absP) values(?,?,?,?,?,?)");
	SQLiteCommand cmd(&sqlite,sql);
	//int newId;
	//GetPumpNewColId(_T("PropertyTable"),sqlite,newId);
	// 批量插入数据
	cmd.BindParam(1,pt.id);
	cmd.BindParam(2,pt.speed);
	cmd.BindParam(3,pt.power);
	cmd.BindParam(4,pt.maxQ);
	cmd.BindParam(5,pt.maxP);
	cmd.BindParam(6,pt.absP);
	if(!sqlite.ExcuteNonQuery(&cmd))
	{
		return false;
		//_tprintf(_T("%s\n"),sqlite.GetLastErrorMsg());
		//break;
	}

	// 清空cmd
	cmd.Clear();
	// 提交事务
	sqlite.CommitTransaction();
	return true;
}

static bool HaveSameType( const CString& szDbPath,const TypeTable& tt,int& id )
{
	CString sql = _T("select * from TypeTable");
	TypeTableVector ttV;
	GetPumpTypeTable(sql,szDbPath,ttV);
	int num = 0;
	for (int i = 0; i < ttV.size(); i++)
	{
		TypeTable oldPT = ttV[i];
		if(oldPT == tt)
		{
			num += 1;
			id = oldPT.id;
		}
	}
	if( num <= 0 ) return false;
	else return true;
}

bool InsertPumpToTypeTable( const CString& szDbPath, const TypeTable& tt ,int& newId)
{
	int ttID;
	//有相同的数据
	if(HaveSameType(szDbPath,tt,ttID))
	{
		newId = ttID;
		return false;
	}

	SQLite sqlite;
	if(!sqlite.Open(szDbPath))
	{
		return false;
	}
	sqlite.BeginTransaction();
	CString sql = _T("insert into TypeTable(catagory_id,type,absP,weight,length,weidth,heigth,factoryName) values(?,?,?,?,?,?,?,?)");
	SQLiteCommand cmd(&sqlite,sql);
	GetPumpNewColId(_T("TypeTable"),sqlite,newId);
	newId += 1;
	// 批量插入数据
	cmd.BindParam(1,newId);
	cmd.BindParam(2,tt.type);
	cmd.BindParam(3,tt.absP);
	cmd.BindParam(4,tt.weight);
	cmd.BindParam(5,tt.length);
	cmd.BindParam(6,tt.weidth);
	cmd.BindParam(7,tt.heigth);
	cmd.BindParam(8,tt.factName);
	if(!sqlite.ExcuteNonQuery(&cmd))
	{
		return false;
	}

	// 清空cmd
	cmd.Clear();
	// 提交事务
	sqlite.CommitTransaction();
	return true;

}