#pragma once

#include <afx.h>
#include "SQLite.h"
#include <vector>

enum TableName
{
	DB_TypeTable = 0,
	DB_PropertyTable = 1
};

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

struct PropertyTable 
{
	int id;
	int speed;
	double power;
	double maxQ;
	int maxP;
	int absP;
};

typedef std::vector<TypeTable> TypeTableVector;
typedef std::vector<PropertyTable> PropertyTableVector;

extern bool GetPumpTypeTable(CString sql, CString szDbPath, TypeTableVector& msg );
extern bool GetPumpPropertyTable(CString sql, CString szDbPath, PropertyTableVector& msg );
