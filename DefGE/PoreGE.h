#pragma once

#include "../MineGE/MineGE.h"
#include "dlimexp.h"

//钻孔图元(开孔，终孔)
class DEFGE_EXPORT_API PoreGE : public MineGE
{
public:
	ACRX_DECLARE_MEMBERS(PoreGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	// 获取插入点坐标
	AcGePoint3d getInsertPt() const;

	// 设置插入点坐标
	void setInsertPt(const AcGePoint3d& pt);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	PoreGE();
	PoreGE(const AcGePoint3d& insertPt);

	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // 插入点坐标
};

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(PoreGE)
#endif
