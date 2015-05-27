#pragma once

#include <string>

struct PumpType 
{
	unsigned long id;
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