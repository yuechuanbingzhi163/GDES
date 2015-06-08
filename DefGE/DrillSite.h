#pragma once

#include "RcuGE.h"

//钻场
class DEFGE_EXPORT_API DrillSite : public RcuTagGE
{
public:
	ACRX_DECLARE_MEMBERS(DrillSite) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	//构造函数
	DrillSite();

	// 获取插入点坐标
	AcGePoint3d getInsertPt() const;

	// 设置插入点坐标
	void setInsertPt(const AcGePoint3d& pt);

	// 获取连接点坐标
	AcGePoint3d getLinkPt() const;

	// 设置连接点坐标
	void setLinkPt(const AcGePoint3d& pt);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // 插入点坐标
	AcGePoint3d m_linkPt;    // 连接点坐标
};

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DrillSite)
#endif
