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

class DBHelper
{
public:
	DBHelper(const std::string& connector, const std::string& connectionString);
	~DBHelper();

	bool createPumpTypeTable();
	bool insertPumpType(const PumpType& pump);
	bool insertPumpTypeTable(const PumpTypeTable& tbls);
	bool getPumpTypeTable(PumpTypeTable& tbls);

	bool createPumpPropertyTable();
	bool insertPumpProperty(const PumpProperty& pump);
	bool insertPumpPropertyTable(const PumpPropertyTable& tbls);
	bool getPumpPropertyTable(PumpPropertyTable& tbls);

private:
	struct DBHelperImpl* d;
};
