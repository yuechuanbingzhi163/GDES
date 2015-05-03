#pragma once

#include "MineGE.h"

//起连接作用的图元，例如巷道、硐室、工作面、掘进工作面
// 它们在通风网络中可以看做是一条分支
class MINEGE_DLLIMPEXP LinkedGE : public MineGE
{
public:
	ACRX_DECLARE_MEMBERS(LinkedGE) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	// 设置始末点坐标
	void setSEPoint(const AcGePoint3d& startPt, const AcGePoint3d& endPt);

	// 获取始末点坐标
	void getSEPoint(AcGePoint3d& startPt, AcGePoint3d& endPt) const;

	// 反向(交换始末点位置)
	void reverse();

	// 计算两坐标之间的角度
	double getAngle() const;

public:
	// 处理连接点闭合情况
	// 返回的向量都是标准化的向量(模--向量长度为1)
	// 注："零向量"表示不处理闭合
	AcGeVector3d getStartPointInExtendAngle() const;
	AcGeVector3d getEndPointInExtendAngle() const;

	void dealWithStartPointBoundary(const AcGeRay3d& boundaryLine);
	void dealWithEndPointBoundary(const AcGeRay3d& boundaryLine);

	void extendByLength(double length);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);
	Acad::ErrorStatus subMoveGripPointsAt( const AcDbIntArray &indices, const AcGeVector3d &offset );
	virtual Acad::ErrorStatus subErase(Adesk::Boolean erasing);	// 处理删除时的闭合

protected:
	LinkedGE();
	LinkedGE(const AcGePoint3d& startPt, const AcGePoint3d& endPt);

private:
	void doEdgeGEJunctionClosure();    // 处理闭合

protected:
	AcGePoint3d m_startPt, m_endPt;    // 始末点坐标
} ;

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(LinkedGE)
#endif