#pragma once
#include "resource.h"
#include "enumDefine.h"

class FanWPCaculHelper
{
public:
	static AcDbObjectIdArray getLinkedChim( const AcDbObjectId& objId );
	static void fanWPWrite( const AcDbObjectId& objId,
		CString name,double Q,double H ,double theoryQ,
		double theoryH,double spaceH ,
		const CString& minQStr,const CString& maxQStr,const CString& minHStr,const CString& maxHStr);
	static ddoubleArray getMultiLinkedChim( const AcDbObjectId& objId);
	static void FanWPCacul(const AcDbObjectIdArray& fanIds,CString ttunelWay,CString method,CString rqQStr,const chooseType& chFanType);
	static void ReadData(CString way,double Q,double H,cstringArray &names,cstringArray &minQs,cstringArray &maxQs,
		cstringArray &minHs,cstringArray &maxHs,cstringArray &ratedPowers);
	//按风量比较
	static void RealParamCaculQ( CString &name, 
		const cstringArray& names,const cstringArray& minQs,const cstringArray& maxQs,const cstringArray& minHs,const cstringArray& maxHs,
		double theoryQ,double r,double p,double& workQ,double& workH,double& spaceH,CString& minQStr,CString& maxQStr,CString& minHStr,
		CString& maxHStr,const cstringArray& ratedPowers,CString& ratedPower);
	//按额定功率比较
	static void RealParamCaculP( CString &name, 
		const cstringArray& names,const cstringArray& minQs,const cstringArray& maxQs,const cstringArray& minHs,const cstringArray& maxHs,
		double theoryQ,double r,double p,double& workQ,double& workH,double& spaceH,CString& minQStr,CString& maxQStr,CString& minHStr,
		CString& maxHStr,const cstringArray& ratedPowers,CString& ratedPower);

	static void classifyFanByWay( AcDbObjectIdArray& fanIds, AcDbObjectIdArray& pushInIds, AcDbObjectIdArray& pullOutIds);
	static int getMaxClassify(const AcDbObjectIdArray& objIds,CString ranField);

private:
	static void singleQHCacul(const AcDbObjectIdArray& fanIds,CString ttunelWay,CString rqQStr,const chooseType& chFanType);
	static void axilQHCacul(const AcDbObjectIdArray& fanIds,CString ttunelWay,CString rqQStr,const chooseType& chFanType);
	static void seriesQHCacul(const AcDbObjectIdArray& fanIds,CString ttunelWay,CString rqQStr,const chooseType& chFanType);
	static void mutiserQHCacul(const AcDbObjectIdArray& fanIds,CString ttunelWay,CString rqQStr,const chooseType& chFanType);
	static bool randIsReasonable(const AcDbObjectIdArray& objIds,CString randField, int classifyIntMax);
	static void cheakMethod(const AcDbObjectIdArray& objIds,CString rqQStr);
	static void libVentQHCacul( const AcDbObjectIdArray& fanIds,CString ttunelWay,CString rqQStr,const chooseType& chFanType);
	
	static int PorQ;//按风量选择还是功率选择,默认是功率选择
};

//单机单风筒
class SingleQHCacul
{
public:
	SingleQHCacul(AcDbObjectId fanObjId,double reqQ,const chooseType& chFanType);
	~SingleQHCacul();
	void caculateQH();

private:
	AcDbObjectId m_fanObjId;
	double m_reqQ;

	double m_workQ;	//风机理论风量
	double m_workH;	//风机理论风压

	chooseType m_chFanType;
};

//辅助风筒（风机并联）
class AxilQHCacul
{
public:
	AxilQHCacul(AcDbObjectIdArray fanObjIds,double reqQ,const chooseType& chFanType);
	~AxilQHCacul();
	void caculateQH(CString way);

protected:
	bool choseFansFromDB(AcDbObjectIdArray fanObjIds,int flag);//flag为通风方式的标签
	bool choseFansByRand(AcDbObjectIdArray objIds, int classifyIntMax,int flag);
private:
	AcDbObjectIdArray m_fanObjIds;
	double m_reqQ;

	double m_workQ;
	double m_workH;

	chooseType m_chFanType;
};

//间断风筒，串联局扇，钻孔通风
class SeriesQHCacul:public AxilQHCacul
{
public:
	SeriesQHCacul(AcDbObjectIdArray fanObjIds,double reqQ,const chooseType& chFanType);
	~SeriesQHCacul();
	//virtual void caculateQH(CString way);

private:
	AcDbObjectIdArray m_fanObjIds;
	double m_reqQ;

	double m_workQ;
	double m_workH;

	chooseType m_chFanType;
};

//单机多列
class MutiserQHCacul : public AxilQHCacul
{
public:
	MutiserQHCacul(AcDbObjectIdArray fanObjIds,double reqQ,const chooseType& chFanType);
	~MutiserQHCacul();
	virtual void caculateQH(CString way);

private:
	virtual bool choseFansFromDB(AcDbObjectIdArray fanObjIds,int flag);
	virtual bool choseFansByRand(AcDbObjectIdArray objIds, int classifyIntMax,int flag);

//private:
//	void caculated(AcDbObjectId fanObjId,CString way);

private:
	AcDbObjectIdArray m_fanObjIds;
	double m_reqQ;

	double m_workQ;
	double m_workH;

	chooseType m_chFanType;
};

//风库通风
class LibVentQHCacul : public AxilQHCacul
{
public:
	LibVentQHCacul(AcDbObjectIdArray fanObjIds,double reqQ,const chooseType& chFanType);
	~LibVentQHCacul();
	void caculateQH(CString way);

private:
	virtual bool choseFansFromDB(AcDbObjectIdArray fanObjtIds,int flag);
	virtual bool choseFansByRand(AcDbObjectIdArray objIds, int classifyIntMax,int flag);
	void setFanNumsInLibs();
	AcDbObjectId getrelatedLib(AcDbObjectId fanId);

private:
	AcDbObjectIdArray m_fanObjIds;
	double m_reqQ;

	double m_workQ;
	double m_workH;

	chooseType m_chFanType;
};
