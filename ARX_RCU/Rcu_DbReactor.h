#pragma once

class Rcu_DbReactor : public AcDbDatabaseReactor 
{
protected:
	AcDbDatabase *mpDatabase ;

public:
	Rcu_DbReactor (AcDbDatabase *pDb =NULL) ;
	virtual ~Rcu_DbReactor () ;

	virtual void Attach (AcDbDatabase *pDb) ;
	virtual void Detach () ;
	virtual AcDbDatabase *Subject () const ;
	virtual bool IsAttached () const ;

	//virtual void objectUnAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj);
	//virtual void objectReAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj);
	virtual void objectAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj);
	virtual void objectModified(const AcDbDatabase* dwg, const AcDbObject* dbObj);
	virtual void objectErased(const AcDbDatabase * dwg, const AcDbObject * dbObj, Adesk::Boolean pErased);
} ;
