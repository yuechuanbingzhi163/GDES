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
	bool operator==( const TypeTable& tt )
	{
		return (/*id == tt.id &&*/ type == tt.type && absP == tt.absP
			&& weight == tt.weight && weidth == tt.weidth
			&& length == tt.length && heigth == tt.heigth
			&& factName == tt.factName);
	}

};

struct PropertyTable 
{
	int id;
	int speed;
	double power;
	double maxQ;
	int maxP;
	int absP;

	bool operator==( const PropertyTable& pt )
	{
		return (id == pt.id && speed == pt.speed && power == pt.power
			&& maxQ == pt.maxQ && maxP == pt.maxP
			&& absP == pt.absP );
	}

};

typedef std::vector<TypeTable> TypeTableVector;
typedef std::vector<PropertyTable> PropertyTableVector;

extern bool GetPumpTypeTable(CString sql, CString szDbPath, TypeTableVector& msg );
extern bool GetPumpPropertyTable(CString sql, CString szDbPath, PropertyTableVector& msg );
//extern bool GetPumpNewColId( const CString& tableName, CString szDbPath, CString& lastId );
extern bool InsertPumpToPropertyTable(const CString& szDbPath, const PropertyTable& pt);
extern bool InsertPumpToTypeTable(const CString& szDbPath, const TypeTable& tt,int& newId);
extern bool DeletePumpFronTable(const CString& szDbPath,const CString& sql);
