#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include <vector>
#include <iostream>
#include <map>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

struct Person
{
	std::string name;
	std::string address;
	int         age;
};

struct PumpType 
{
	int id;
	int catagory_id;
	std::string type;
	std::string factName;
	int absP;
	int weight;
	int length;
	int width;
	int height;
};

namespace Poco {
	namespace Data {

		template <>
		class TypeHandler<PumpType>
		{
		public:
			static void bind(std::size_t pos, const PumpType& obj, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
			{
				poco_assert_dbg (!pBinder.isNull());
				// the table is defined as Person (FirstName VARCHAR(30), lastName VARCHAR, SocialSecNr INTEGER(3))
				// Note that we advance pos by the number of columns the datatype uses! For string/int this is one.
				TypeHandler<int>::bind(pos++, obj.id, pBinder, dir);
				TypeHandler<int>::bind(pos++, obj.catagory_id, pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.type, pBinder, dir);
				TypeHandler<int>::bind(pos++, obj.absP, pBinder, dir);
				TypeHandler<int>::bind(pos++, obj.weight, pBinder, dir);
				TypeHandler<int>::bind(pos++, obj.length, pBinder, dir);
				TypeHandler<int>::bind(pos++, obj.width, pBinder, dir);
				TypeHandler<int>::bind(pos++, obj.height, pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.factName, pBinder, dir);
			}

			static std::size_t size()
			{
				return 9; // we handle three columns of the Table!
			}

			static void prepare(std::size_t pos, const PumpType& obj, AbstractPreparator::Ptr pPrepare)
			{
				poco_assert_dbg (!pPrepare.isNull());
				// the table is defined as Person (FirstName VARCHAR(30), lastName VARCHAR, SocialSecNr INTEGER(3))
				// Note that we advance pos by the number of columns the datatype uses! For string/int this is one.

				TypeHandler<int>::prepare(pos++, obj.id, pPrepare);
				TypeHandler<int>::prepare(pos++, obj.catagory_id, pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.type, pPrepare);
				TypeHandler<int>::prepare(pos++, obj.absP, pPrepare);
				TypeHandler<int>::prepare(pos++, obj.weight, pPrepare);
				TypeHandler<int>::prepare(pos++, obj.length, pPrepare);
				TypeHandler<int>::prepare(pos++, obj.width, pPrepare);
				TypeHandler<int>::prepare(pos++, obj.height, pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.factName, pPrepare);
			}

			static void extract(std::size_t pos, PumpType& obj, const PumpType& defVal, AbstractExtractor::Ptr pExt)
				/// obj will contain the result, defVal contains values we should use when one column is NULL
			{
				poco_assert_dbg (!pExt.isNull());

				TypeHandler<int>::extract(pos++, obj.id, defVal.id, pExt);
				TypeHandler<int>::extract(pos++, obj.catagory_id, defVal.catagory_id, pExt);
				TypeHandler<std::string>::extract(pos++, obj.type, defVal.type,pExt);
				TypeHandler<int>::extract(pos++, obj.absP, defVal.absP, pExt);
				TypeHandler<int>::extract(pos++, obj.weight, defVal.weight, pExt);
				TypeHandler<int>::extract(pos++, obj.length, defVal.length, pExt);
				TypeHandler<int>::extract(pos++, obj.width, defVal.width, pExt);
				TypeHandler<int>::extract(pos++, obj.height, defVal.height, pExt);
				TypeHandler<std::string>::extract(pos++, obj.factName, defVal.factName, pExt);
			}

		private:
			TypeHandler();
			~TypeHandler();
			TypeHandler(const TypeHandler&);
			TypeHandler& operator=(const TypeHandler&);
		};

	} } // namespace Poco::Data

void test2()
{
	// create a session
	Session session("SQLite", "test_pump.db");

	// drop sample table, if it exists
	session << "DROP TABLE IF EXISTS TypeTable", now;

	// (re)create table
	session << "CREATE TABLE TypeTable ([id] INTEGER NOT NULL PRIMARY KEY, [catagory_id] INTEGER REFERENCES [Category]([id]), [type] NVARCHAR(20), [absP] INTEGER, [weight] INTEGER, [length] INTEGER,[weidth] INTEGER,[heigth] INTEGER,[factoryName] NVARCHAR(100))", now;
//session << "CREATE TABLE TypeTable ([catagory_id] INTEGER, [type] NVARCHAR(20), [absP] INTEGER, [weight] INTEGER, [length] INTEGER,[weidth] INTEGER,[heigth] INTEGER,[factoryName] NVARCHAR(100))", now;
	PumpType pump;

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
	insert << "INSERT INTO TypeTable(catagory_id,type,absP,weight,length,weidth,heigth,factoryName) VALUES(?, ?, ?, ?, ?, ?, ?, ?)",
		use(pump.catagory_id),
		use(pump.type),
		use(pump.absP),
		use(pump.weight),
		use(pump.length),
		use(pump.width),
		use(pump.height),
		use(pump.factName);

	insert.execute();

	pump.catagory_id=2;
	pump.type="2BEA-101-0";
	pump.absP=33;
	pump.weight=110;
	pump.length=741;
	pump.width=375;
	pump.height=355;
	pump.factName="淄博水环真空泵厂有限公司";

	insert.execute();

	std::vector<int> ids;
	int id=0;
	Statement select(session);
	select << "select catagory_id from TypeTable",
		into(ids), now;

	//select << "select id from TypeTable",
	//	into(id),range(0,1);

	//while (!select.done())
	//{
	//	select.execute();
	//	std::cout << id<<std::endl;
	//}

	std::vector<std::string> types;
	Statement select2(session);
	select2 << "select type from TypeTable",
		into(types),
		now;

	std::vector<PumpType> pumps;
	Statement select3(session);
	select3 << "select * from TypeTable",
		into(pumps),
		now;

	//while (!select.done())
	//{
	//	select.execute();
	//	std::cout << pump.id<<std::endl;
	//}

	std::cout<<pumps[0].factName<<std::endl;
	std::cout<<pumps[1].factName<<std::endl;
	int aa=10;
}

void test1()
{
	// create a session
	Session session("SQLite", "sample.db");

	// drop sample table, if it exists
	session << "DROP TABLE IF EXISTS Person", now;

	// (re)create table
	session << "CREATE TABLE Person (Name VARCHAR(30), Address VARCHAR, Age INTEGER(3))", now;

	// insert some rows
	Person person = 
	{
		"Bart Simpson",
		"Springfield",
		12
	};

	Statement insert(session);
	insert << "INSERT INTO Person VALUES(?, ?, ?)",
		use(person.name),
		use(person.address),
		use(person.age);

	std::string xx = insert.toString();
	insert.execute();

	person.name    = "Lisa Simpson";
	person.address = "Springfield";
	person.age     = 10;

	insert.execute();

	// a simple query
	Statement select(session);
	select << "SELECT Name, Address, Age FROM Person",
		into(person.name),
		into(person.address),
		into(person.age),
		range(0, 1); //  iterate over result set one row at a time

	while (!select.done())
	{
		select.execute();
		std::cout << person.name << " " << person.address << " " << person.age << std::endl;
	}
}

int main(int argc, char** argv)
{
	// register SQLite connector
	Poco::Data::SQLite::Connector::registerConnector();

	//test1();
	test2();

	return 0;
}