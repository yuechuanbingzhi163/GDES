#include "StdAfx.h"
#include "config.h"
#include "FuncFieldHelper.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/MineGE.h"
#include "../MineGE/HelperClass.h"

#include <set>

#define SEPARATOR _T("~")

static bool IsEmptyString( const CString& str )
{
	return ( str.GetLength() == 0 );
}

bool FuncFieldHelper::RemoveFunction( const CString& f )
{
	if( IsEmptyString(f) ) return false;

	ArxDictHelper::RemoveKey(FUNC_FIELD_DICT, f);
	return true;
}

bool FuncFieldHelper::RemoveType(const CString& f, const CString& type)
{
	if( IsEmptyString(f) || IsEmptyString(type)) return false;

	AcStringArray allEntries;
	ArxDictHelper::GetAllEntries(FUNC_FIELD_DICT, f, allEntries);

	AcStringArray entries;
	for(int i=0;i<allEntries.length();i++)
	{
		int pos = allEntries[i].find(SEPARATOR);
		if(pos == -1) continue;

		if(allEntries[i].substr(0, pos).compareNoCase(type) == 0)
		{
			entries.append(allEntries[i]);
		}
	}

	for(int i=0;i<entries.length();i++)
	{
		ArxDictHelper::RemoveEntry(FUNC_FIELD_DICT, f, entries[i].kACharPtr());
	}

	return true;
}

bool FuncFieldHelper::GetFunctions(AcStringArray& funcs)
{
	ArxDictHelper::GetAllKeys(FUNC_FIELD_DICT, funcs);
	return true;
}

bool FuncFieldHelper::GetTypes(const CString& f, AcStringArray& types)
{
	if( IsEmptyString(f) ) return false;

	AcStringArray entries;
	if(!ArxDictHelper::GetAllEntries(FUNC_FIELD_DICT, f, entries)) return false;

	typedef std::set<AcString> AcStringSet;
	AcStringSet ss;

	for(int i=0;i<entries.length();i++)
	{
		int pos = entries[i].find(SEPARATOR);
		if(pos == -1) continue;

		AcString type = entries[i].substr(0, pos);
		if(ss.find(type) == ss.end())
		{
			types.append(type);
			ss.insert(type);
		}
	}
	return true;
}

bool FuncFieldHelper::GetFields(const CString& f, const CString& type, AcStringArray& fields)
{
	if( IsEmptyString(f) || IsEmptyString(type) ) return false;

	AcStringArray entries;
	if(!ArxDictHelper::GetAllEntries(FUNC_FIELD_DICT, f, entries)) return false;

	for(int i=0;i<entries.length();i++)
	{
		int pos = entries[i].find(SEPARATOR);
		if(pos == -1) continue;

		if(entries[i].substr(0, pos).compareNoCase(type) == 0)
		{
			fields.append(entries[i].substr(pos+1, -1));
		}
	}
	return true;
}

bool FuncFieldHelper::AddField(const CString& f, const CString& type, const CString& field)
{
	if( IsEmptyString(f) || IsEmptyString(type) || IsEmptyString(field) ) return false;

	CString entry;
	entry.Format(_T("%s%s%s"), type, SEPARATOR, field);
	//acutPrintf(_T("\nÌí¼Ó×Ö¶Î:%s"), entry);
	return ArxDictHelper::AddEntry(FUNC_FIELD_DICT, f, entry);
}

bool FuncFieldHelper::RemoveField(const CString& f, const CString& type, const CString& field)
{
	if( IsEmptyString(f) || IsEmptyString(type) || IsEmptyString(field) ) return false;
	
	CString entry;
	entry.Format(_T("%s%s%s"), type, SEPARATOR, field);
	return ArxDictHelper::RemoveEntry(FUNC_FIELD_DICT, f, entry);
}
