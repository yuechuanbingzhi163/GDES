#pragma once

#include "MineGE.h"

// Com ==> 缩写 Composite
// ComGE的绘制效果是由所包含的图元的图形效果组合而成
// 类似于Office中的"图形组合"
class MINEGE_DLLIMPEXP ComGE : public MineGE {

public:
	ACRX_DECLARE_MEMBERS(ComGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	bool appendEnt(const AcDbObjectId& objId);           // 组合图元
	void getAllEnts(AcDbObjectIdArray& objIds) const;    // 获取所有组合图元的id集合
	void decomposeAllEnts();                             // 解除组合

	// AcDbObject类函数重载实现
public:
	Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);
	Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;

	// 重载MineGE虚函数
protected:
	virtual void pushKeyParamToWriter(DrawParamWriter& writer) const;
	virtual void pullKeyParamFromReader(DrawParamReader& reader);

	// 目前只实现了"绘制draw"和"变换transform"方法
protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d& xform);
	virtual Acad::ErrorStatus subErase(Adesk::Boolean erasing);
	virtual Acad::ErrorStatus subGetGeomExtents(AcDbExtents& extents) const;

protected:
	ComGE(void);
	void showEnt(const AcDbObjectId& objId, bool isVisible = false); // 隐藏图元实体
	void removeAllEnts();                                         // 清空所有的图元
	void simpleAppendEnt(const AcDbObjectId& objId);              // 将图元添加到组合图元中，不做其它处理
	void findEntsNotInOtherComGEs(const AcDbObjectIdArray& geIds, AcDbObjectIdArray& objIds);        // 在其它ComGE中查找与当前包含图元中不重合的部分

private:
	AcDbObjectIdArray m_geIds;
};

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ComGE)
#endif