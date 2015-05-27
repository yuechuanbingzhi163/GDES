#include "StdAfx.h"
#include "DBHelper.h"

#define DATABASE_SQLITE
#include "database.hxx" // create_database
#include "GasPump-odb.hxx"

using namespace odb::core;

#include "../ArxHelper/HelperClass.h"

static bool HaveSameProperty( DBPtr db, const PumpProperty& pt, unsigned long& id )
{
	typedef odb::query<PumpProperty> query;
	typedef odb::result<PumpProperty> result;

	transaction t(db->begin());
	result r(db->query<PumpProperty>());

	bool ret = false;
	for (result::iterator itr(r.begin()); itr!=r.end(); ++itr)
	{
		if(*itr == pt)
		{
			id = itr->id_;
			ret = true; 
			break;
		}
	}
	t.commit();

	return ret;
}

static bool HaveSameType( DBPtr db, const PumpType& tt, int& id )
{
	typedef odb::query<PumpType> query;
	typedef odb::result<PumpType> result;

	transaction t(db->begin());
	result r(db->query<PumpType>());

	bool ret = false;
	for (result::iterator itr(r.begin()); itr!=r.end(); ++itr)
	{
		if(*itr == tt)
		{
			id = itr->id_;
			ret = true; 
			break;
		}
	}

	t.commit();

	return ret;
}

bool DBHelper::GetPumpTypeTable( DBPtr db, PumpTypeTable& msg )
{
	typedef odb::query<PumpType> query;
	typedef odb::result<PumpType> result;

	transaction t(db->begin());
	result r(db->query<PumpType>());
	for (result::iterator itr(r.begin()); itr != r.end(); ++itr)
	{
		PumpType*pumpTypeObj = new PumpType();
		pumpTypeObj->id_ = itr->id_;
		pumpTypeObj->type = itr->type;
		pumpTypeObj->absP = itr->absP;
		pumpTypeObj->weight = itr->weight;
		pumpTypeObj->length = itr->length;
		pumpTypeObj->width = itr->width
		pumpTypeObj->height = itr->height;
		pumpTypeObj->factName = itr->factName;
		msg.push_back(pumpTypeObj);
	}
	t.commit();

	return true;
}

bool DBHelper::GetPumpPropertyTable( DBPtr db, PumpPropertyTable& msg )
{
	typedef odb::query<PumpProperty> query;
	typedef odb::result<PumpProperty> result;

	transaction t(db->begin());
	result r(db->query<PumpProperty>());
	for (result::iterator itr(r.begin()); itr != r.end(); ++itr)
	{
		PumpProperty* pumpPropertyObj = new PumpProperty();
		pumpPropertyObj->id_ = itr->id_;
		pumpPropertyObj->speed = itr->speed;
		pumpPropertyObj->power = itr->power;
		pumpPropertyObj->maxQ = itr->maxQ;
		pumpPropertyObj->maxP = itr->maxP;
		pumpPropertyObj->absP = itr->absP;
		msg.push_back(pumpPropertyObj);
	}
	t.commit();

	return true;
}

bool DBHelper::InsertPumpToPropertyTable( DBPtr db, const PumpProperty& pt )
{
	//有相同的数据
	//if(HaveSameProperty(db,pt)) return false;

	//typedef odb::query<PumpProperty> query;
	//typedef odb::result<PumpProperty> result;

	transaction t(db->begin());
	unsigned long id = db->persist(pt);
	t.commit();

	return true;
}

bool DBHelper::InsertPumpToTypeTable(DBPtr db, const PumpType& tt, unsigned long& newId)
{
	//int ttID;
	////有相同的数据
	//if(HaveSameType(szDbPath,tt,ttID))
	//{
	//	newId = ttID;
	//	return false;
	//}

	//typedef odb::query<PumpType> query;
	//typedef odb::result<PumpType> result;

	transaction t(db->begin());
	newId = db->persist(tt);
	t.commit();

	return true;
}

bool DBHelper::DeletePumpFronTable( DBPtr db, unsigned long id)
{
	transaction t (db->begin ());
	db->erase<person> (id);
	t.commit ();

	return true;
}