#include <vector>
#include <iostream>
#include <map>

#include <soci.h>
#include <sqlite3/soci-sqlite3.h>
using namespace soci;

struct Person
{
	std::string name;
	std::string address;
	int age;
};

struct GasPump
{
	int id;
	std::string type;
	std::string factName;
	int absP;
	int weight;
	int length;
	int width;
	int height;
};

namespace soci
{
	template<> 
	struct type_conversion<Person>
	{
		typedef values base_type;
		static void from_base(values const & v, indicator /* ind */, Person & p)
		{
			//std::cout<<"-----Person::from_base----"<<std::endl;
			p.name = v.get<std::string>("name");
			p.address = v.get<std::string>("address");
			p.age = v.get<int>("age");
		}
		static void to_base(const Person & p, values & v, indicator & ind)
		{
			//std::cout<<"-----Person::to_base----"<<std::endl;
			v.set("name", p.name);
			v.set("address", p.address);
			v.set("age", p.age);
			ind = i_ok;
		}
	};

	template<> 
	struct type_conversion<GasPump>
	{
		typedef values base_type;
		static void from_base(values const & v, indicator /* ind */, GasPump & p)
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
		static void to_base(const GasPump & p, values & v, indicator & ind)
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
}

void test1()
{
	try {
		// create a sql
		//session sql(soci::sqlite3, "sample.db");
		session sql(soci::sqlite3, ":memory:");
		//indicator ind;

		// drop sample table, if it exists
		sql << "drop table if exists Person";

		// (re)create table
		sql << "create table Person (name VARCHAR(30), address VARCHAR, age INTEGER(3))";

		// insert some rows
		Person person = 
		{
			"Bart Simpson",
			"Springfield",
			12
		};

		sql << "insert into Person values(:name, :address, :age)",
			use(person.name),
			use(person.address),
			use(person.age);

		sql << "select name, address, age from Person",
			into(person.name),
			into(person.address),
			into(person.age);

		std::cout<<"name="<<person.name<<std::endl;
		std::cout<<"address="<<person.address<<std::endl;
		std::cout<<"age="<<person.age<<std::endl;

		person.name    = "Lisa Simpson";
		person.address = "Springfield";
		person.age     = 10;
		sql << "insert into Person values( :name, :address, :age)", use(person);

		Person aPerson;
		statement st = (sql.prepare<<"select name, address, age from Person", into(aPerson));
		st.execute();
		while (st.fetch())
		{
			std::cout << aPerson.name << '\n';
		}
	}
	catch(soci_error const & e)
	{
		std::cout<<"soci error:"<<e.what()<<std::endl;
	}
}

void test2()
{
	try {
		// create a sql
		//session sql(soci::sqlite3, "test_pump2.db");
		session sql(soci::sqlite3, ":memory:");

		// drop sample table, if it exists
		sql << "drop table if exists TypeTable";

		// (re)create table
		sql << "create table TypeTable (id INTEGER PRIMARY KEY AUTOINCREMENT, type VARCHAR(20), absP INTEGER, weight INTEGER, length INTEGER, weidth INTEGER, heigth INTEGER, factoryName VARCHAR(100))";

		GasPump pump;
		// insert some rows
		pump.type="2BEA-101-0";pump.absP=33;pump.weight=90;pump.length=638;pump.width=375;pump.height=355;pump.factName="淄博水环真空泵厂有限公司";

		sql << "insert into TypeTable values(NULL, :type,:absP,:weight,:length,:weidth,:heigth,:factoryName)", use(pump);
		sql << "insert into TypeTable values(NULL, :type,:absP,:weight,:length,:weidth,:heigth,:factoryName)", use(pump);
		sql << "insert into TypeTable values(NULL, :type,:absP,:weight,:length,:weidth,:heigth,:factoryName)", use(pump);
		sql << "insert into TypeTable values(NULL, :type,:absP,:weight,:length,:weidth,:heigth,:factoryName)", use(pump);

		std::cout<<"11111--------------------------------------------111111"<<std::endl;

		sql << "insert into TypeTable(type,absP,weight,length,weidth,heigth,factoryName) values(:type,:absP,:weight,:length,:weidth,:heigth,:factoryName)",
			use(pump.type),
			use(pump.absP),
			use(pump.weight),
			use(pump.length),
			use(pump.width),
			use(pump.height),
			use(pump.factName);
		sql << "insert into TypeTable(type,absP,weight,length,weidth,heigth,factoryName) values(:type,:absP,:weight,:length,:weidth,:heigth,:factoryName)",
			use(pump.type),
			use(pump.absP),
			use(pump.weight),
			use(pump.length),
			use(pump.width),
			use(pump.height),
			use(pump.factName);

		std::cout<<"2222--------------------------------------------2222"<<std::endl;

		pump.type="2BEA-101-0";pump.absP=33;pump.weight=110;pump.length=741;pump.width=375;pump.height=355;pump.factName="淄博水环真空泵厂有限公司";

		std::vector<int> ids(10);
		sql << "select id from TypeTable", into(ids);

		int id=0;
		sql << "select id from TypeTable",into(id);
		if(sql.got_data())
		{
			std::cout << id<<std::endl;
		}

		std::vector<std::string> types(10);
		sql << "select type from TypeTable",into(types);

		std::cout<<"3333--------------------------------------------333"<<std::endl;

		GasPump aPump;
		statement st = (sql.prepare <<"select * from TypeTable", into(aPump));
		st.execute();
		while (st.fetch())
		{
			std::cout << aPump.id << '\n';
		}

		std::cout<<"444--------------------------------------------444"<<std::endl;
	}
	catch(soci_error const & e)
	{
		std::cout<<"soci error:"<<e.what()<<std::endl;
	}
}

#include "PumpDatabase.h"

void test_dbHelper()
{
	DBHelper db("test_pump.db");
	db.createPumpTypeTable();

	PumpType pump;
	pump.type="2BEA-101-0";	pump.absP=33;pump.weight=110;pump.length=741;pump.width=375;pump.height=355;pump.factName="淄博水环真空泵厂有限公司";
	db.insertPumpType(pump);

	pump.type="2BEA-101-0";pump.absP=33;pump.weight=90;	pump.length=638;pump.width=375;	pump.height=355;pump.factName="淄博水环真空泵厂有限公司";
	db.insertPumpType(pump);

	int id = -1;
	db.getLastPumpTypeId(id);
	std::cout<<"最后插入的id是"<<id<<std::endl;

	PumpTypeTable tbls;
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	tbls.push_back(pump);
	db.insertPumpTypeTable(tbls);
	std::cout<<"插入多个数据"<<std::endl;

	IDArray ids;
	ids.push_back(2);
	ids.push_back(5);
	ids.push_back(7);
	db.delPumpTypes(ids);
	std::cout<<"删除多个数据"<<std::endl;

	id = -1;
	db.getLastPumpTypeId(id);
	std::cout<<"最后插入的id是"<<id<<std::endl;

	tbls.clear();
	db.getPumpTypeTable(tbls, "weight=90");
	std::cout<<"泵类型个数:"<<tbls.size()<<std::endl;

	PumpType new_pump = tbls[2];
	new_pump.id = 3; new_pump.absP=30335;
	db.updatePumpType(new_pump);

	//删除id=4的所有行
	db.delPumpType(4);

	tbls.clear();
	db.getPumpTypeTable(tbls);
	std::cout<<"泵类型个数:"<<tbls.size()<<std::endl;

	PumpType pump3;
	db.getPumpType(3, pump3);
	std::cout<<"获取第id=3的泵类型, id="<<pump3.id<<" absP="<<pump3.absP<<std::endl;
}

#include <fstream>
#include <vector>
typedef std::vector<std::string> StringArray;

void ReadTypeTable(StringArray& fields, PumpTypeTable& tbls)
{
	std::ifstream infile("pump_type.txt");
	if(!infile) return ;

	std::string field;
	for(int i=0;i<8;i++)
	{
		infile>>field;
		fields.push_back(field);
	}

	PumpType pump;
	while(!infile.eof())
	{
		infile>> pump.id >> pump.type >> pump.absP >> pump.weight >> pump.length >> pump.width >> pump.height >> pump.factName;
		tbls.push_back(pump);
		if(infile.fail()) break;
	}
	infile.close();
}

void ReadPropertyTable(StringArray& fields, PumpPropertyTable& tbls)
{
	std::ifstream infile("pump_property.txt");
	if(!infile) return ;

	std::string field;
	for(int i=0;i<7;i++)
	{
		infile>>field;
		fields.push_back(field);
	}

	PumpProperty pump;
	while(!infile.eof())
	{
		infile>> pump.id >> pump.catagory_id >> pump.speed >> pump.power >> pump.maxQ >> pump.maxP >> pump.absP;
		tbls.push_back(pump);
		if(infile.fail()) break;
	}
	infile.close();
}

void CreatePumpDB()
{
	StringArray type_fields;
	PumpTypeTable type_tbls;
	ReadTypeTable(type_fields, type_tbls);

	StringArray prop_fields;
	PumpPropertyTable prop_tbls;
	ReadPropertyTable(prop_fields, prop_tbls);

	//新建sqlite数据库
	DBHelper db("new_pump.db");
	//新建TypeTable表并插入数据
	db.createPumpTypeTable();
	db.insertPumpTypeTable(type_tbls);
	//新建PropertyTable表并插入数据
	db.createPumpPropertyTable();
	db.insertPumpPropertyTable(prop_tbls);
}

int main(int argc, char** argv)
{
	test1();
	test2();
	test_dbHelper();
	//CreatePumpDB();

	return 0;
}