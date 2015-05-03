#pragma once

#include "../MineGE/LinkedGE.h"
#include "dlimexp.h"

// 依靠风筒导风的独头巷道称为掘进工作面
// 风筒和局部扇风机所在的巷道称为"导风巷道"
class DEFGE_EXPORT_API TTunnel : public LinkedGE
{
public:
	ACRX_DECLARE_MEMBERS(TTunnel) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	TTunnel();
	TTunnel(const AcGePoint3d& startPt, const AcGePoint3d& endPt);
	virtual ~TTunnel();

	//void setInTunnel(const AcDbObjectId& objId); // 设置"导风巷道"
	//AcDbObjectId getInTunnel() const;            // 获取"导风巷道"

	//virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	//virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

//protected:
//	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

//private:
//	AcDbObjectId m_inTunnel;  // "导风巷道"
};

#ifdef DEFGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(TTunnel)
#endif
