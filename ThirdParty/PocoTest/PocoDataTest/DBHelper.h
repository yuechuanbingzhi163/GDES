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

class DBHelper
{
public:
	DBHelper(const std::string& connector, const std::string& connectionString);
	~DBHelper();

	bool createPumpTypeTable();
	bool insertPumpType(const PumpType& pump);
	bool insertPumpTypeTable(const PumpTypeTable& pump_tables);
	bool getPumpTypeTable(PumpTypeTable& pump_tables);
private:
	struct DBHelperImpl* d;
};
