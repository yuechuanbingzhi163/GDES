#pragma once

#include <string>
#include <vector>

struct PumpType 
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

struct PumpProperty 
{
	int id;
	int catagory_id;
	int speed;
	double power;
	double maxQ;
	int maxP;
	int absP;
};

typedef std::vector<PumpType> PumpTypeTable;
typedef std::vector<PumpProperty> PumpPropertyTable;
typedef std::vector<int> IDArray;

class DBHelper
{
public:
	DBHelper(const std::string& connector, const std::string& connectionString);
	~DBHelper();

	//建表
	bool createPumpTypeTable();
	//增加一个数据
	bool insertPumpType(const PumpType& pump);
	//增加多个数据
	bool insertPumpTypeTable(const PumpTypeTable& tbls);
	//查询
	bool getPumpTypeTable(PumpTypeTable& tbls, const std::string& condition="");
	//修改(根据pump的id来更新)
	bool updatePumpType(const PumpType& pump);
	//删除
	bool delPumpType(int id);
	//删除多条数据
	bool delPumpTypes(const IDArray& ids);
	//获取TypeTable最后一条数据的id
	bool getLastPumpTypeId(int& id);
	//根据id查找数据
	bool getPumpType(int id, PumpType& pump);
	//根据id查找一系列的数据
	bool getPumpTypeTableByIDs(const IDArray& ids, PumpTypeTable& tbls);

	//建表
	bool createPumpPropertyTable();
	//增加一个数据
	bool insertPumpProperty(const PumpProperty& pump);
	//增加多个数据
	bool insertPumpPropertyTable(const PumpPropertyTable& tbls);
	//查询
	bool getPumpPropertyTable(PumpPropertyTable& tbls, const std::string& condition="");
	//修改(根据pump的id来更新)
	bool updatePumpProperty(const PumpProperty& pump);
	//删除
	bool delPumpProperty(int& id);
	//删除多个数据
	bool delPumpProperties(const IDArray& ids);
	//获取PropertyTable表中最后一条数据的id
	bool getLastPumpPropertyId(int& id);
	//根据id查找数据
	bool getPumpProperty(int id, PumpProperty& pump);
	//根据id查找一系列的数据
	bool getPumpPropertyTableByIDs(const IDArray& ids, PumpPropertyTable& tbls);

private:
	struct DBHelperImpl* d;
};
