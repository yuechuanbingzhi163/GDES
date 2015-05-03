#pragma once

#include "MineGE.h"

/*
 * 标签类图元
 * 例如:
 * 	1) 标签巷道的风流方向、运煤方向等
 * 这类图元必须要要关联一个图元，它们是无法独立存在的
 * 被关联的宿主图元(Host)可以有很多的"标签图元"
 */
class MINEGE_DLLIMPEXP TagGE : public MineGE 
{
public:
	ACRX_DECLARE_MEMBERS(TagGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	// 获取关联图元
	AcDbObjectId getRelatedGE() const;

	// 设置关联的图元
	// 只能和MineGE图元关联
	void setRelatedGE(const AcDbObjectId& objId);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	// 如果m_objId.isNull()==true，那么就不会绘制图形
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);

protected:
	TagGE();

private:
	AcDbObjectId m_objId;    // 标签关联的宿主图元(host)
};

/*
 * 方向标签类图元
 * 例如风流方向、运煤符号等
 * 还包括风机、风门等(后续可能需要调整类的划分和层次结构)
 * 目前暂时这么分类
 */
class MINEGE_DLLIMPEXP DirectionTagGE : public TagGE {

public:
	ACRX_DECLARE_MEMBERS(DirectionTagGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	// 获取插入点坐标
	AcGePoint3d getInsertPt() const;

	// 设置插入点坐标
	void setInsertPt(const AcGePoint3d& pt);

	// 获取方向角度
	double getDirectionAngle() const;

	// 设置方向角度
	void setDirectionAngle(double angle);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	DirectionTagGE();
	DirectionTagGE(const AcGePoint3d& insertPt, double angle);

	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // 插入点坐标
	double m_angle;          // 方向角度
};

/*
 * 设施图元
 * 例如瓦斯泵、放水器等
 */
class MINEGE_DLLIMPEXP  FacilityGE: public DirectionTagGE {

public:
	ACRX_DECLARE_MEMBERS(FacilityGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	// 获取插入点坐标
	AcGePoint3d getInsertPt() const;

	// 设置插入点坐标
	void setInsertPt(const AcGePoint3d& pt);

	// 获取方向角度
	double getDirectionAngle() const;

	// 设置方向角度
	void setDirectionAngle(double angle);

public:
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	FacilityGE();
	FacilityGE(const AcGePoint3d& insertPt, double angle);

	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	// key param
	AcGePoint3d m_insertPt;  // 插入点坐标
	double m_angle;          // 方向角度
};

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TagGE)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DirectionTagGE)
ACDB_REGISTER_OBJECT_ENTRY_AUTO(FacilityGE)
//ACDB_REGISTER_OBJECT_ENTRY_AUTO(TextTagGE)
#endif