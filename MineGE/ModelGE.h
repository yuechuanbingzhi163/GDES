#pragma once

#include "dlimexp.h"

/*
 * MineGE或DataObject的观察者(参见：观察者模式)
 * 例如巷道、工作面图元在通风网络中映射成一条分支
 * 分支使用它们的一部分数据（通风数据）
 */
class MINEGE_DLLIMPEXP ModelGE : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(ModelGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	// 返回图元类型名称
	CString getTypeName() const;

	// 获取数据源
	AcDbObjectId getDataObject() const;

	// 设置数据源
	void setDataObject(const AcDbObjectId& objId);

	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler *pFiler);

protected:
	ModelGE();

private:
	AcDbObjectId m_objId;  // 数据源(从MineGE或DataObject派生)
} ;

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ModelGE)
#endif
