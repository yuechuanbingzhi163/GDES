#pragma once

#include <afx.h>
#include "SQLite.h"
#include <vector>

struct TypeTable 
{
	int id;
	CString type;
	int absP;
	int weight;
	int length;
	int weidth;
	int heigth;
	CString factName;
};
extern bool GetPumpMessage(CString sql, CString szDbPath, std::vector<TypeTable>& msg );
