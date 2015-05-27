#include "StdAfx.h"
#include "GasPump.h"

#include <Poco/Data/Session.h>
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Tuple.h>
#include <Poco/String.h>
#include <Poco/Format.h>
#include <Poco/NumberFormatter.h>
#include <Poco/TextConverter.h>
#include <poco/ASCIIEncoding.h>
#include <poco/UTF16Encoding.h>
#include <poco/UTF8Encoding.h>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

#include <string>

void InitDataBaseSystem()
{
	// register SQLite connector
	Poco::Data::SQLite::Connector::registerConnector();
}

bool GetPumpTypeTable(const std::string& connector, const std::string& connectionString, PumpTypeTable& msg )
{
	Session session(connector, connectionString);
	PumpType pump;

	// (re)create table
	session << "CREATE TABLE TypeTable ([id] INTEGER NOT NULL PRIMARY KEY, [catagory_id] INTEGER REFERENCES [Category]([id]), [type] NVARCHAR(20), [absP] INTEGER, [weight] INTEGER, [length] INTEGER,[weidth] INTEGER,[heigth] INTEGER,[factoryName] NVARCHAR(100))", now;

	// insert some rows
	pump.catagory_id=1;
	pump.type="2BEA-101-0";
	pump.absP=33;
	pump.weight=90;
	pump.length=638;
	pump.width=375;
	pump.height=355;
	pump.factName="淄博水环真空泵厂有限公司";

	Statement insert(session);
	insert << "INSERT INTO TypeTable VALUES(?, ?, ?, ?, ?, ?, ?, ?)",
		use(pump.catagory_id),
		use(pump.type),
		use(pump.absP),
		use(pump.weight),
		use(pump.length),
		use(pump.width),
		use(pump.height),
		use(pump.factName);

	std::string xx = insert.toString();
	insert.execute();

	//pump.catagory_id=2;
	//pump.type="2BEA-101-0";
	//pump.absP=33;
	//pump.weight=110;
	//pump.length=741;
	//pump.width=375;
	//pump.height=355;
	//pump.factName="淄博水环真空泵厂有限公司";

	//insert.execute();

	//Statement select(session);
	//select << "select id from TypeTable",
	//	into(pump.id),now;

	//select << "select id, catagory_id, type, absP, weight, length, weidth, heigth, factoryName from TypeTable",
	//	into(pump.id),
	//	into(pump.catagory_id),
	//	into(pump.type),
	//	into(pump.absP),
	//	into(pump.weight),
	//	into(pump.length),
	//	into(pump.width),
	//	into(pump.height),
	//	into(pump.factName),
	//	range(0, 1); //  iterate over result set one row at a time

	//while (!select.done())
	//{
	//	select.execute();
	//	msg.push_back(pump);
	//}

	return true;
}

void testxx()
{
	InitDataBaseSystem();
	PumpTypeTable tables;
	//GetPumpTypeTable("SQlite", "pump.db", tables);
	GetPumpTypeTable("SQlite", "test_pump.db", tables);
	int a=10;
}