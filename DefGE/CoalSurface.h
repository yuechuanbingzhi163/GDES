#pragma once

#include "RcuGE.h"

//煤层
class DEFGE_EXPORT_API CoalSurface : public RcuTagGE
{
public:
	ACRX_DECLARE_MEMBERS(CoalSurface) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	//构造函数
	CoalSurface();

	// 获取插入点坐标
	AcGePoint3d getInsertPt() const;

	// 设置插入点坐标
	void setInsertPt(const AcGePoint3d& pt);

	double getWidth() const;
	void setWidth(double w);

	double getHeight() const;
	void setHeight(double h);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // 插入点坐标
	double m_width, m_height; // 宽度和高度
};

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CoalSurface)
#endif
