#pragma once

#include "SQLite.h"
#include <vector>

struct TypeTable 
{
	int id;
	LPCTSTR type;
	int absP;
	int weight;
	int length;
	int weidth;
	int heigth;
	LPCTSTR factName;
};
extern bool GetPumpMessage(LPCTSTR sql, LPCTSTR szDbPath, std::vector<TypeTable>& msg );
