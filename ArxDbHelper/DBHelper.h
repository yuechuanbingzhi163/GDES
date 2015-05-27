#pragma once

#include "dlimexp.h"

#include <memory>   // std::auto_ptr
#include <vector>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include "GasPump.hxx"

using namespace odb::core;

typedef std::auto_ptr<odb::database> DBPtr;

typedef std::vector<PumpType*> PumpTypeTable;
typedef std::vector<PumpProperty*> PumpPropertyTable;

class ARXDBHELPER_DLLIMPEXP DBHelper
{
public:
	static bool GetPumpTypeTable(DBPtr db, PumpTypeTable& msg );
	static bool GetPumpPropertyTable(DBPtr db, PumpPropertyTable& msg );
	static bool InsertPumpToPropertyTable(DBPtr db, const PumpProperty& pt);
	static bool InsertPumpToTypeTable(DBPtr db, const PumpType& tt, unsigned long& newId);
	static bool DeletePumpFronTable(DBPtr db,const CString& sql);
};
