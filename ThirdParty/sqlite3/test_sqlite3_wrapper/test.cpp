#include "SQLite.h"
#include <cstdlib>
#include <cstdio>
#include <clocale>
#include <iostream> 

#include <fstream>
// 需要包含<fstream>才能使用
#include "AcFStream.h"
using namespace std;

void CreatPropertyTable(SQLite& sqlite)
{
	// 创建数据库表
    //******************************************************
	TCHAR sql[512] = {0};
	_stprintf_s(sql,_T("%s"),
		_T("CREATE TABLE [PropertyTable] (")
		_T("[id] INTEGER NOT NULL PRIMARY KEY, ")
		_T("[catagory_id] INTEGER REFERENCES [Category]([id]), ")
		_T("[speed] INTEGER, ")
		_T("[power] REAL, ")
		_T("[maxQ] REAL,")
		_T("[maxP] INTEGER, ")
		_T("[absP] INTEGER);")
		_T("CREATE INDEX [Property_id] ON [PropertyTable] ([id]);")
		);
	if(!sqlite.ExcuteNonQuery(sql))
	{
		_tprintf(_T("Create database table failed...\n"));
	}
	//******************************************************


	// 插入数据【通过参数绑定的方式，提交批量数据时，比上面的普通模式效率更高（提高约45%），同时可支持插入二进制数据】
	DWORD dwBeginTick = GetTickCount();
    //******************************************************
	// 当一次性插入多条记录时候，采用事务的方式，提高效率
	sqlite.BeginTransaction();
	memset(sql,0,sizeof(sql));
	_stprintf_s(sql,_T("insert into PropertyTable(catagory_id,speed,power,maxQ,maxP,absP) values(?,?,?,?,?,?)"));
	SQLiteCommand cmd(&sqlite,sql);
	// 批量插入数据
	AcIfstream in(_T("PropertyTable.txt"));  
	if (! in.is_open())  
	{ 
		cout << _T("文件打开失败..."); 
		return; 
	}  
	while (!in.eof() )  
	{  
		TCHAR id[256],spped[256],power[256],maxQ[256],maxP[256],absP[256];  
		//in.getline (buffer,100);
		in >> id >> spped >> power >> maxQ >> maxP >> absP;
		if(in.fail()) break;
		cmd.BindParam(1,_ttoi(id));
		cmd.BindParam(2,_ttoi(spped));
		cmd.BindParam(3,_tstof(power));
		cmd.BindParam(4,_tstof(maxQ));
		cmd.BindParam(5,_ttoi(maxP));
		cmd.BindParam(6,_ttoi(absP));
		if(!sqlite.ExcuteNonQuery(&cmd))
		{
			_tprintf(_T("%s\n"),sqlite.GetLastErrorMsg());
			break;
		}

		//cout << id << "\t" << spped << "\t" << power << "\t" << maxQ << "\t" << maxP << "\t" << absP << endl;  
		//cout << buffer[0] << endl;
	}

	// 清空cmd
	cmd.Clear();
	// 提交事务
	sqlite.CommitTransaction();
	_tprintf(_T("Insert Data Take %dMS...\n"),GetTickCount()-dwBeginTick);
    //******************************************************
}

void CreatTypeTable(SQLite& sqlite)
{
	// 创建数据库表
    //******************************************************
	TCHAR sql[512] = {0};
	_stprintf_s(sql,_T("%s"),
		_T("CREATE TABLE [TypeTable] (")
		_T("[id] INTEGER NOT NULL PRIMARY KEY, ")
		_T("[catagory_id] INTEGER REFERENCES [Category]([id]), ")
		_T("[type] NVARCHAR(20), ")
		_T("[absP] INTEGER, ")
		_T("[weight] INTEGER, ")
		_T("[length] INTEGER,")
		_T("[weidth] INTEGER,")
		_T("[heigth] INTEGER,")
		_T("[factoryName] NVARCHAR(100));")
		_T("CREATE INDEX [Property_id] ON [TypeTable] ([id]);")
		);
	if(!sqlite.ExcuteNonQuery(sql))
	{
		_tprintf(_T("Create database table failed...\n"));
	}
	//******************************************************


	// 插入数据【通过参数绑定的方式，提交批量数据时，比上面的普通模式效率更高（提高约45%），同时可支持插入二进制数据】
	DWORD dwBeginTick = GetTickCount();
    //******************************************************
	// 当一次性插入多条记录时候，采用事务的方式，提高效率
	sqlite.BeginTransaction();
	memset(sql,0,sizeof(sql));
	_stprintf_s(sql,_T("insert into TypeTable(catagory_id,type,absP,weight,length,weidth,heigth,factoryName) values(?,?,?,?,?,?,?,?)"));
	SQLiteCommand cmd(&sqlite,sql);
	// 批量插入数据
	AcIfstream in(_T("TypeTable.txt"));  
	if (! in.is_open())  
	{ 
		cout << _T("文件打开失败..."); 
		return; 
	}  
	while (!in.eof() )  
	{  
		TCHAR id[256],type[256],absP[256],weight[256],length[256],weidth[256],heigth[256],factoryName[256];  
		//in.getline (buffer,100);
		in >> id >> type >> absP >> weight >> length >> weidth >> heigth >> factoryName;
		if(in.fail()) break;

		cmd.BindParam(1,_ttoi(id));
		cmd.BindParam(2,type);
		cmd.BindParam(3,_ttoi(absP));
		cmd.BindParam(4,_ttoi(weight));
		cmd.BindParam(5,_ttoi(length));
		cmd.BindParam(6,_ttoi(weidth));
		cmd.BindParam(7,_ttoi(heigth));
		cmd.BindParam(8,factoryName);
		if(!sqlite.ExcuteNonQuery(&cmd))
		{
			_tprintf(_T("%s\n"),sqlite.GetLastErrorMsg());
			break;
		}

		//cout << id << "\t" << type << "\t" << absP << "\t" << weight << "\t" << length << "\t" << weidth << "\t" << heigth << "\t" << factoryName << endl;  
		//cout << buffer[0] << endl;
	}

	// 清空cmd
	cmd.Clear();
	// 提交事务
	sqlite.CommitTransaction();
	_tprintf(_T("Insert Data Take %dMS...\n"),GetTickCount()-dwBeginTick);
    //******************************************************
}

void SqliteOperate()
{
	TCHAR *szDbPath = _T("pump.db");

	::DeleteFile(szDbPath);

	SQLite sqlite;

	// 打开或创建数据库
	//******************************************************
	if(!sqlite.Open(szDbPath))
	{
		_tprintf(_T("%s\n"),sqlite.GetLastErrorMsg());
		return;
	}
    //******************************************************
	CreatPropertyTable(sqlite);
	CreatTypeTable(sqlite);

	TCHAR sql[512] = {0};
	// 查询
	DWORD dwBeginTick = GetTickCount();
	//******************************************************
	//memset(sql,0,sizeof(sql));
	//_stprintf(sql,_T("%s"),_T("select * from Book where name = '他的国5'"));
	_stprintf_s(sql,_T("%s"),_T("select * from PropertyTable"));

	SQLiteDataReader Reader = sqlite.ExcuteQuery(sql);

	int index = 0;
	while(Reader.Read())
	{
		_tprintf( _T("***************【第%d条记录】***************\n"),++index);
		_tprintf( _T("字段名:%s 字段值:%d\n"),Reader.GetName(0),Reader.GetIntValue(0));
		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader.GetName(1),Reader.GetStringValue(1));
		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader.GetName(2),Reader.GetStringValue(2));
		_tprintf( _T("字段名:%s 字段值:%d\n"),Reader.GetName(3),Reader.GetIntValue(3));
		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader.GetName(4),Reader.GetStringValue(4));
		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader.GetName(5),Reader.GetStringValue(5));
		_tprintf( _T("*******************************************\n"));
	}
	Reader.Close();
	_tprintf(_T("Query Take %dMS...\n"),GetTickCount()-dwBeginTick);
   // ******************************************************

	_stprintf_s(sql,_T("%s"),_T("select * from TypeTable where catagory_id = 1"));

	SQLiteDataReader Reader2 = sqlite.ExcuteQuery(sql);

	index = 0;
	while(Reader2.Read())
	{
		_tprintf( _T("***************【第%d条记录】***************\n"),++index);
		_tprintf( _T("字段名:%s 字段值:%d\n"),Reader2.GetName(0),Reader2.GetIntValue(0));
		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader2.GetName(1),Reader2.GetStringValue(1));
		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader2.GetName(2),Reader2.GetStringValue(2));
		_tprintf( _T("字段名:%s 字段值:%d\n"),Reader2.GetName(3),Reader2.GetIntValue(3));
		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader2.GetName(4),Reader2.GetStringValue(4));
		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader2.GetName(5),Reader2.GetStringValue(5));
		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader2.GetName(6),Reader2.GetStringValue(6));
		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader2.GetName(7),Reader2.GetStringValue(7));
		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader2.GetName(8),Reader2.GetStringValue(8));
		_tprintf( _T("*******************************************\n"));
	}
	Reader2.Close();
	_tprintf(_T("Query Take %dMS...\n"),GetTickCount()-dwBeginTick);

	// 关闭数据库
	sqlite.Close();
}

int main()
{
	/**
	* 参考:http://blog.csdn.net/cdn_founder/article/details/7679137
	*/
	setlocale(LC_ALL, "chs");//需要实现本地化，以实现中文正常输出
	SqliteOperate();
	return 0;
}

//void SqliteOperate()
//{
//	TCHAR *szDbPath = _T("Book.db");
//
//	::DeleteFile(szDbPath);
//
//	SQLite sqlite;
//
//	// 打开或创建数据库
//	//******************************************************
//	if(!sqlite.Open(szDbPath))
//	{
//		_tprintf(_T("%s\n"),sqlite.GetLastErrorMsg());
//		return;
//	}
//    //******************************************************
//
//	// 创建数据库表
//    //******************************************************
//	TCHAR sql[512] = {0};
//	_stprintf(sql,_T("%s"),
//		_T("CREATE TABLE [Book] (")
//		_T("[id] INTEGER NOT NULL PRIMARY KEY, ")
//		_T("[name] NVARCHAR(20), ")
//		_T("[author] NVARCHAR(20), ")
//		_T("[catagory_id] INTEGER REFERENCES [Category]([id]), ")
//		_T("[abstruct] NVARCHAR(100) ,")
//		_T("[path] sqNVARCHAR(50), ")
//		_T("[image] BLOB);")
//		_T("CREATE INDEX [Book_id] ON [Book] ([id]);")
//		);
//	if(!sqlite.ExcuteNonQuery(sql))
//	{
//		_tprintf(_T("Create database table failed...\n"));
//	}
//	//******************************************************
//
//	// 插入数据【普通方式】
//	DWORD dwBeginTick = GetTickCount();
//	//******************************************************
//	// 当一次性插入多条记录时候，采用事务的方式，提高效率
//	sqlite.BeginTransaction();
//	// 批量插入数据
//	for(int i=0;i<10;i++)
//	{
//		memset(sql,0,sizeof(sql));
//		_stprintf(sql,_T("insert into Book(name,author,catagory_id) values('红高粱%d','莫言',1)"),i);
//		if(!sqlite.ExcuteNonQuery(sql))
//		{
//			_tprintf(_T("%s\n"),sqlite.GetLastErrorMsg());
//			break;
//		}
//	}
//	// 提交事务
//	sqlite.CommitTransaction();
//	_tprintf(_T("Insert Data Take %dMS...\n"),GetTickCount()-dwBeginTick);
//    //******************************************************
//
//
//	// 插入数据【通过参数绑定的方式，提交批量数据时，比上面的普通模式效率更高（提高约45%），同时可支持插入二进制数据】
//	dwBeginTick = GetTickCount();
//    //******************************************************
//	// 当一次性插入多条记录时候，采用事务的方式，提高效率
//	sqlite.BeginTransaction();
//	memset(sql,0,sizeof(sql));
//	_stprintf(sql,_T("insert into Book(name,author,catagory_id,image) values(?,'韩寒',?,?)"));
//	SQLiteCommand cmd(&sqlite,sql);
//	// 批量插入数据
//	for(int i=0;i<10;i++)
//	{
//		TCHAR strValue[16] = {0};
//		_stprintf(strValue,_T("他的国%d"),i);
//		// 绑定第一个参数（name字段值）
//		cmd.BindParam(1,strValue);
//		// 绑定第二个参数（catagory_id字段值）
//		cmd.BindParam(2,20);
//		BYTE imageBuf[] = {0xff,0xff,0xff,0xff};
//		// 绑定第三个参数（image字段值,二进制数据）
//		cmd.BindParam(3,imageBuf,sizeof(imageBuf));
//		if(!sqlite.ExcuteNonQuery(&cmd))
//		{
//			_tprintf(_T("%s\n"),sqlite.GetLastErrorMsg());
//			break;
//		}
//	}
//	// 清空cmd
//	cmd.Clear();
//	// 提交事务
//	sqlite.CommitTransaction();
//	_tprintf(_T("Insert Data Take %dMS...\n"),GetTickCount()-dwBeginTick);
//    //******************************************************
//
//	// 查询
//	dwBeginTick = GetTickCount();
//	//******************************************************
//	memset(sql,0,sizeof(sql));
//	//_stprintf(sql,_T("%s"),_T("select * from Book where name = '他的国5'"));
//	_stprintf(sql,_T("%s"),_T("select * from Book"));
//
//	SQLiteDataReader Reader = sqlite.ExcuteQuery(sql);
//
//	int index = 0;
//	int len = 0;
//	while(Reader.Read())
//	{
//		_tprintf( _T("***************【第%d条记录】***************\n"),++index);
//		_tprintf( _T("字段名:%s 字段值:%d\n"),Reader.GetName(0),Reader.GetIntValue(0));
//		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader.GetName(1),Reader.GetStringValue(1));
//		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader.GetName(2),Reader.GetStringValue(2));
//		_tprintf( _T("字段名:%s 字段值:%d\n"),Reader.GetName(3),Reader.GetIntValue(3));
//		_tprintf( _T("字段名:%s 字段值:%s\n"),Reader.GetName(4),Reader.GetStringValue(4));
//		// 读取图片二进制文件
//		const BYTE *ImageBuf = Reader.GetBlobValue(6,len);
//		_tprintf( _T("*******************************************\n"));
//	}
//	Reader.Close();
//	_tprintf(_T("Query Take %dMS...\n"),GetTickCount()-dwBeginTick);
//    //******************************************************
//
//	// 关闭数据库
//	sqlite.Close();
//}
