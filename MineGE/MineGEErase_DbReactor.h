#pragma once

/*
 * 当删除一个图元，要同时删除图元上的标签图元TagGE
 * 以及关联的模型图元ModelGE
 */
class MineGEErase_DbReactor : public AcDbDatabaseReactor 
{
protected:
	AcDbDatabase *mpDatabase ;

public:
	MineGEErase_DbReactor (AcDbDatabase *pDb =NULL) ;
	virtual ~MineGEErase_DbReactor () ;

	virtual void Attach (AcDbDatabase *pDb) ;
	virtual void Detach () ;
	virtual AcDbDatabase *Subject () const ;
	virtual bool IsAttached () const ;

	/*
	 * 当数据对象在后台被改变，强制更新与之关联的图形效果
	 * 有些图元的可视化效果与数据有关，例如测风站(Joint)
	 */
	virtual void objectModified(const AcDbDatabase* dwg, const AcDbObject* dbObj);

	/*
	 * 当图元被删除，同时也应删除图元关联的标签图元(TagGE)
	 */
	virtual void objectErased(const AcDbDatabase * dwg, const AcDbObject * dbObj, Adesk::Boolean pErased);
} ;
