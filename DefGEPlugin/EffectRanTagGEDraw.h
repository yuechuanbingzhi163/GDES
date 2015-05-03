#pragma once

#include "../MineGEDraw/MineGEDraw.h"

class EffectRanTagGEDraw : public MineGEDraw
{
public:
	ACRX_DECLARE_MEMBERS(EffectRanTagGEDraw);

public:
	EffectRanTagGEDraw();
	~EffectRanTagGEDraw(void);

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

private:
	// key param
	AcGePoint3d m_insertPt; // 插入点坐标
	AcGePoint3d m_pt;
	double m_minDistan;
	double m_maxDistan; 
	double m_angle;
	double m_distance; //工作面总长度的1/10

	AcGePoint3d m_centrPt;

private:
	void drawText(AcGiWorldDraw *mode);
	void drawTag(AcGiWorldDraw* mode);
	void drawTagInit(AcGePoint3dArray& pts);
};

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(EffectRanTagGEDraw)
#endif
