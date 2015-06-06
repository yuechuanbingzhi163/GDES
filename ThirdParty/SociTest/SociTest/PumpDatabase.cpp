/**
* 参考链接：
* http://www.vimer.cn/2011/03/%E6%9B%B4%E7%AE%80%E6%B4%81%E7%9A%84c%E6%95%B0%E6%8D%AE%E5%BA%93%E8%AE%BF%E9%97%AE%E6%A1%86%E6%9E%B6-soci.html
* http://www.cnblogs.com/russellluo/p/3186786.html
*/

#include "PumpDatabase.h"

#include <soci.h>
#include <sqlite3/soci-sqlite3.h>
using namespace soci;

#include <sstream>

namespace soci
{
	template<> 
	struct type_conversion<PumpType>
	{
		typedef values base_type;
		static void from_base(values const & v, indicator /* ind */, PumpType & p)
		{
			p.id = v.get<int>("id");
			p.type = v.get<std::string>("type");
			p.factName = v.get<std::string>("factoryName");
			p.absP = v.get<int>("absP");
			p.weight = v.get<int>("weight");
			p.length = v.get<int>("length");
			p.width = v.get<int>("weidth");
			p.height = v.get<int>("heigth");
		}
		static void to_base(const PumpType & p, values & v, indicator & ind)
		{
			v.set("id", p.id);
			v.set("type", p.type);
			v.set("factoryName", p.factName);
			v.set("absP", p.absP);
			v.set("weight", p.weight);
			v.set("length", p.length);
			v.set("weidth", p.width);
			v.set("heigth", p.height);
			ind = i_ok;
		}
	};

	template<> 
	struct type_conversion<PumpProperty>
	{
		typedef values base_type;
		static void from_base(values const & v, indicator /* ind */, PumpProperty & p)
		{
			p.id = v.get<int>("id");
			p.catagory_id=v.get<int>("catagory_id");
			p.speed=v.get<int>("speed");
			p.power=v.get<double>("power");
			p.maxQ=v.get<double>("maxQ");
			p.maxP=v.get<int>("maxP");
			p.absP=v.get<int>("absP");
		}
		static void to_base(const PumpProperty & p, values & v, indicator & ind)
		{
			v.set("id", p.id);
			v.set("catagory_id", p.catagory_id);
			v.set("speed", p.speed);
			v.set("power", p.power);
			v.set("maxQ", p.maxP);
			v.set("maxP", p.maxP);
			ind = i_ok;
		}
	};
}

static std::string FormatSql(const std::string& tbl, const std::string& condition)
{
	std::stringstream ss;
	ss<<"select * from "<<tbl;
	if(!condition.empty())
	{
		ss << " where "<< condition;
	}
	return ss.str();
}

struct DBHelperImpl
{
	DBHelperImpl(const std::string& connectionString) 
		: sql(soci::sqlite3, connectionString) {}
	session sql;
};

//简化写法
#define SQL d->sql

DBHelper::DBHelper(const std::string& connectionString)
{
	d = new DBHelperImpl(connectionString);
}

DBHelper::~DBHelper()
{
	delete d; d = 0;
}

bool DBHelper::createPumpTypeTable()
{
	bool ret = true;
	try
	{
		// drop sample table, if it exists
		SQL << "drop table if exists TypeTable";

		// (re)create table
		SQL << "create table TypeTable ([id] INTEGER PRIMARY KEY AUTOINCREMENT, [type] NVARCHAR(20), [absP] INTEGER, [weight] INTEGER, [length] INTEGER, [weidth] INTEGER, [heigth] INTEGER, [factoryName] NVARCHAR(100))";
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::insertPumpType(const PumpType& pump)
{
	bool ret = true;
	try
	{
		SQL << "insert into TypeTable values(NULL,:type,:absP,:weight,:length,:weidth,:heigth,:factoryName)", use(pump);
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::insertPumpTypeTable(const PumpTypeTable& tbls)
{
	if(tbls.empty()) return false;

	bool ret = true;
	try
	{
		PumpType pump;
		statement st = (SQL.prepare <<
			"insert into TypeTable values(NULL,:type,:absP,:weight,:length,:weidth,:heigth,:factoryName)",
			use(pump));
		for (PumpTypeTable::const_iterator itr=tbls.begin(); itr!=tbls.end(); ++itr)
		{
			pump = *itr;
			st.execute(true);
		}
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::getPumpTypeTable(PumpTypeTable& tbls, const std::string& condition)
{
	bool ret = true;
	try
	{
		//SQL << "select * from TypeTable", into(tbls);
		PumpType aPump;
		statement st = (SQL.prepare <<FormatSql("TypeTable", condition), into(aPump));
		st.execute();
		while(st.fetch())
		{
			tbls.push_back(aPump);
		}
	}
	catch(soci_error const &)
	{
		tbls.clear();
		ret = false;
	}
	return ret;
}

bool DBHelper::updatePumpType(const PumpType& pump_)
{
	bool ret = true;
	try
	{
		//去掉const修饰，否则into/use会报错
		PumpType& pump = const_cast<PumpType&>(pump_);

		SQL << "update TypeTable set type=:type,absP=:absP,weight=:weight,length=:length,weidth=:weidth,heigth=:heigth,factoryName=:factoryName where id=:id", 
			use(pump),use(pump.id, "id");
	}
	catch(soci_error const & e)
	{
		std::string ss=e.what();
		ret = false;
	}
	return ret;
}

bool DBHelper::delPumpType(int id)
{
	bool ret = true;
	try
	{
		SQL << "delete from TypeTable where id=:id", use(id);
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::getLastPumpTypeId(int& id)
{
	bool ret = true;
	try
	{
		SQL << "select max(id) from TypeTable", into(id); // 通用方法
		//SQL << "select last_insert_rowid() from TypeTable", into(id); // sqlite
		//SQL << "select last_insert_id() from TypeTable", into(id);  // mysql
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::delPumpTypes(const IDArray& ids)
{
	bool ret = true;
	try
	{
		SQL << "delete from TypeTable where id=:id", use(ids);
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::getPumpType(int id, PumpType& pump)
{
	bool ret = true;
	try
	{
		SQL << "select * from TypeTable where id=:id", into(pump), use(id);
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}


bool DBHelper::getPumpTypeTableByIDs(const IDArray& ids, PumpTypeTable& tbls)
{
	bool ret = true;
	try
	{
		int id = -1;
		PumpType pump;
		statement st = (SQL.prepare<<"select * from TypeTable where id=:id", into(pump), use(id));
		for(IDArray::const_iterator itr=ids.begin(); itr!=ids.end(); ++itr)
		{
			id = *itr;
			st.execute(true);
			tbls.push_back(pump);
		}
	}
	catch(soci_error const &)
	{
		tbls.clear();
		ret = false;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


bool DBHelper::createPumpPropertyTable()
{
	bool ret = true;
	try
	{
		// drop sample table, if it exists
		SQL << "drop table if exists PropertyTable";

		// (re)create table
		SQL << "create table PropertyTable ([id] INTEGER PRIMARY KEY AUTOINCREMENT, [catagory_id] INTEGER REFERENCES [Category]([id]), [speed] INTEGER, [power] REAL, [maxQ] REAL,[maxP] INTEGER, [absP] INTEGER)";
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::insertPumpProperty(const PumpProperty& pump)
{
	bool ret = true;
	try
	{
		SQL << "insert into PropertyTable values(NULL, :catagory_id, :speed, :power, :maxQ, :maxP, :absP)", use(pump);
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::insertPumpPropertyTable(const PumpPropertyTable& tbls)
{
	if(tbls.empty()) return false;

	bool ret = true;
	try
	{
		PumpProperty pump;
		statement st = (SQL.prepare <<
			"insert into PropertyTable values(NULL,:catagory_id,:speed,:power,:maxQ,:maxP,:absP)",
			use(pump));
		for (PumpPropertyTable::const_iterator itr=tbls.begin(); itr!=tbls.end(); ++itr)
		{
			pump = *itr;
			st.execute(true);
		}
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::getPumpPropertyTable(PumpPropertyTable& tbls, const std::string& condition)
{
	bool ret = true;
	try
	{
		//SQL << "select * from TypeTable", into(tbls);
		PumpProperty aPump;
		statement st = (SQL.prepare <<FormatSql("PropertyTable", condition), into(aPump));
		st.execute();
		while (st.fetch())
		{
			tbls.push_back(aPump);
		}
	}
	catch(soci_error const &)
	{
		tbls.clear();
		ret = false;
	}
	return ret;
}

bool DBHelper::updatePumpProperty(const PumpProperty& pump)
{
	bool ret = true;
	try
	{
		SQL << "update PropertyTable set catagory_id=:catagory_id,speed=:speed,power=:power,maxQ=:maxQ,maxP=:maxP,absP=:absP where id=:id", 
			use(pump), use(pump.id, "id");
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::delPumpProperty(int& id)
{
	bool ret = true;
	try
	{
		SQL << "delete from PropertyTable where id=:id", use(id);
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::getLastPumpPropertyId(int& id)
{
	bool ret = true;
	try
	{
		SQL << "select max(id) from PropertyTable", into(id); // 通用方法
		//SQL << "select last_insert_rowid() from PropertyTable", into(id); // sqlite
		//SQL << "select last_insert_id() from PropertyTable", into(id);  // mysql
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}


bool DBHelper::getPumpProperty(int id, PumpProperty& pump)
{
	bool ret = true;
	try
	{
		SQL << "select * from PropertyTable where id=:id", into(pump), use(id);
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::delPumpProperties(const IDArray& ids)
{
	bool ret = true;
	try
	{
		SQL << "delete from PropertyTable where id=:id", use(ids);
	}
	catch(soci_error const &)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::getPumpPropertyTableByIDs(const IDArray& ids, PumpPropertyTable& tbls)
{
	bool ret = true;
	try
	{
		int id = -1;
		PumpProperty pump;
		statement st = (SQL.prepare <<"select * from PropertyTable where id=:id", into(pump), use(id));
		for(IDArray::const_iterator itr=ids.begin(); itr!=ids.end(); ++itr)
		{
			id = *itr;
			st.execute(true);
			tbls.push_back(pump);
		}
	}
	catch(soci_error const &)
	{
		tbls.clear();
		ret = false;
	}
	return ret;
}
