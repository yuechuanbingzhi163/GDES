#include "StdAfx.h"
#include "Rcu_DbReactor.h"
#include "UIHelper.h"

#include "../MineGE/MineGE.h"
#include "../MineGE/TagGE.h"
#include "../MineGE/DataObject.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

#include "../DefGE/RcuGE.h"
#include "../DefGE/Pore.h"
#include "../DefGE/DrillSite.h"
#include "../DefGE/RockGate.h"
#include "../DefGE/CoalSurface.h"

Rcu_DbReactor::Rcu_DbReactor ( AcDbDatabase* pDb ) : AcDbDatabaseReactor(), mpDatabase( pDb )
{
    if ( pDb )
    {
        //acutPrintf(_T("\nMineGEErase_DbReactor : %ld"), (long)pDb);
        pDb->addReactor ( this ) ;
    }
}

Rcu_DbReactor::~Rcu_DbReactor ()
{
    Detach () ;
}

void Rcu_DbReactor::Attach ( AcDbDatabase* pDb )
{
    Detach () ;
    if ( mpDatabase == NULL )
    {
        if ( ( mpDatabase = pDb ) != NULL )
            pDb->addReactor ( this ) ;
    }
}

void Rcu_DbReactor::Detach ()
{
    if ( mpDatabase )
    {
        mpDatabase->removeReactor ( this ) ;
        mpDatabase = NULL ;
    }
}

AcDbDatabase* Rcu_DbReactor::Subject () const
{
    return ( mpDatabase ) ;
}

bool Rcu_DbReactor::IsAttached () const
{
    return ( mpDatabase != NULL ) ;
}

void Rcu_DbReactor::objectAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
	//AcDbDatabaseReactor::objectAppended(dwg, dbObj);
	if(dbObj->isKindOf(RockGate::desc()) || dbObj->isKindOf(RcuGE::desc()))
	{
		ArxMsg msgParam = { dbObj->isA()->name(), dbObj->objectId() };
		UIHelper::SendMessage(WM_ADD_GE, msgParam);
	}
}

void Rcu_DbReactor::objectUnAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
	//AcDbDatabaseReactor::objectAppended(dwg, dbObj);
	if(dbObj->isKindOf(RockGate::desc()) || dbObj->isKindOf(RcuGE::desc()))
	{
		ArxMsg msgParam = { dbObj->isA()->name(), dbObj->objectId() };
		UIHelper::SendMessage(WM_DEL_GE, msgParam);
	}
}

void Rcu_DbReactor::objectReAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
	//AcDbDatabaseReactor::objectAppended(dwg, dbObj);
	if(dbObj->isKindOf(RockGate::desc()) || dbObj->isKindOf(RcuGE::desc()))
	{
		ArxMsg msgParam = { dbObj->isA()->name(), dbObj->objectId() };
		UIHelper::SendMessage(WM_ADD_GE, msgParam);
	}
}

// 在objectModified中无法启动事务
void Rcu_DbReactor::objectModified( const AcDbDatabase* dwg, const AcDbObject* dbObj )
{
    //AcDbDatabaseReactor::objectModified ( dwg, dbObj );
	if(dbObj->isKindOf(RockGate::desc()) || dbObj->isKindOf(RcuGE::desc()))
	{
		ArxMsg msgParam = { dbObj->isA()->name(), dbObj->objectId() };
		UIHelper::SendMessage(WM_CHANGE_GE, msgParam);
	}
}

void Rcu_DbReactor::objectErased( const AcDbDatabase* dwg, const AcDbObject* dbObj, Adesk::Boolean pErased )
{
	//AcDbDatabaseReactor::objectErased ( dwg, dbObj, pErased );
	if(dbObj->isKindOf(RockGate::desc()) || dbObj->isKindOf(RcuGE::desc()))
	{
		ArxMsg msgParam = { dbObj->isA()->name(), dbObj->objectId() };
		UIHelper::SendMessage(WM_DEL_GE, msgParam);
	}
}
