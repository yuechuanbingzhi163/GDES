#pragma once

#include <string>
#include <vector>

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

struct PumpProperty 
{
	unsigned long id;
	int speed;
	double power;
	double maxQ;
	int maxP;
	int absP;
};

typedef std::vector<PumpType> PumpTypeTable;
typedef std::vector<PumpProperty> PumpPropertyTable;