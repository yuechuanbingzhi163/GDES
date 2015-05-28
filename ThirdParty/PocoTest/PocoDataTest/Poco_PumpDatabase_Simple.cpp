#include "DBHelper.h"

#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include <poco/Data/DataException.h>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::DataException;

static void InitDbSystem()
{
	Poco::Data::SQLite::Connector::registerConnector();
}

static void ShutDownDbSystem()
{
	Poco::Data::SQLite::Connector::unregisterConnector();
}

struct DBHelperImpl
{
	DBHelperImpl(const std::string& connector, const std::string& connectionString) 
		: session(connector, connectionString) {}
	Session session;
};

//简化写法
#define SESSION d->session

DBHelper::DBHelper(const std::string& connector, const std::string& connectionString)
{
	InitDbSystem();
	d = new DBHelperImpl(connector, connectionString);
}

DBHelper::~DBHelper()
{
	delete d; d = 0;
	ShutDownDbSystem();
}

bool DBHelper::createPumpTypeTable()
{
	bool ret = true;
	try
	{
		// drop sample table, if it exists
		SESSION << "DROP TABLE IF EXISTS TypeTable", now;

		// (re)create table
		SESSION << "CREATE TABLE TypeTable ([id] INTEGER PRIMARY KEY AUTOINCREMENT, [type] NVARCHAR(20), [absP] INTEGER, [weight] INTEGER, [length] INTEGER,[weidth] INTEGER,[heigth] INTEGER,[factoryName] NVARCHAR(100))", now;
	}
	catch(DataException& e)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::insertPumpType(const PumpType& pump_)
{
	bool ret = true;
	try
	{
		//下面的use语句里直接使用const引用会出错
		//因此需要做一个强制转换，去掉const修饰
		PumpType& pump = const_cast<PumpType&>(pump_);

		Statement insert(SESSION);
		insert << "INSERT INTO TypeTable(type, absP, weight, length, weidth, heigth, factoryName) VALUES(?, ?, ?, ?, ?, ?, ?)",
			use(pump.type),
			use(pump.absP),
			use(pump.weight),
			use(pump.length),
			use(pump.width),
			use(pump.height),
			use(pump.factName), 
			now;
		//insert.execute();
	}
	catch(DataException& e)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::getPumpTypeTable(PumpTypeTable& pump_tables)
{
	bool ret = true;
	try
	{
		PumpType pump;

		Statement select(SESSION);
		select << "select id, type, absP, weight, length, weidth, heigth, factoryName from TypeTable",
			into(pump.id),
			into(pump.type),
			into(pump.absP),
			into(pump.weight),
			into(pump.length),
			into(pump.width),
			into(pump.height),
			into(pump.factName),
			range(0, 1);
		//select.execute();

		while(!select.done())
		{
			select.execute();
			pump_tables.push_back(pump);
		}
	}
	catch(DataException& e)
	{
		ret = false;
	}
	return ret;
}

bool DBHelper::insertPumpTypeTable(const PumpTypeTable& pump_tables)
{
	if(pump_tables.empty()) return false;

	bool ret = true;
	try
	{
		//下面的use语句里直接使用const引用会出错
		//因此需要做一个强制转换，去掉const修饰
		//PumpTypeTable& pump_tables = const_cast<PumpTypeTable&>(pump_tables_);
		PumpType pump = pump_tables[0];

		Statement insert(SESSION);
		insert << "INSERT INTO TypeTable(type, absP, weight, length, weidth, heigth, factoryName) VALUES(?, ?, ?, ?, ?, ?, ?)",
			use(pump.type),
			use(pump.absP),
			use(pump.weight),
			use(pump.length),
			use(pump.width),
			use(pump.height),
			use(pump.factName);
		//insert.execute();

		for(PumpTypeTable::const_iterator itr=pump_tables.begin(); itr!=pump_tables.end(); ++itr)
		{
			pump = *itr;
			insert.execute();
		}
	}
	catch(DataException& e)
	{
		ret = false;
	}
	return ret;
}
