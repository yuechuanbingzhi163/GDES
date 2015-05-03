#pragma once

#include "Tunnel.h"

// 弧线巷道图元(单独分离出来)
// 因为弧线巷道可以与直线巷道在一个图形中共存
// 也可以考虑从LinkedGE类派生
class DEFGE_EXPORT_API ArcTunnel : public Tunnel {

public:
	ACRX_DECLARE_MEMBERS(ArcTunnel) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	ArcTunnel();
	ArcTunnel(const AcGePoint3d& startPt, const AcGePoint3d& endPt, const AcGePoint3d& thirdPt);
	virtual ~ArcTunnel();

	// 获取/设置半径参数
	AcGePoint3d getThirdPt() const;
	void setThirdPt(const AcGePoint3d& pt);

	AcGePoint3d getArcCntPt() const;
	bool ptIsOnArc(AcGePoint3d pt) const;
	double getArcAngle(AcGePoint3d insertPt);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

protected:
	virtual void writeKeyParam(DrawParamWriter& writer) const;
	virtual void readKeyParam(DrawParamReader& reader);

private:
	// 采用3点弧的表示方法
	AcGePoint3d m_thirdPt;

} ;

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ArcTunnel)
#endif
