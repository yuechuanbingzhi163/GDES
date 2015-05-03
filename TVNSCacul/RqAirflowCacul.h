#pragma once
#include "../MineGE/LinkedGE.h"

//计算依据枚举
enum CACUL_BASIS
{
	AFTER_DAMP	= 0,	//按排出炮烟
	DUST_SPEED	= 1,	//按排尘风速
	UNIT_POWER	= 2,	//按单位功率
	HEAT_ADJT	= 3,	//热量调节
	WORKER_NUM	= 4		//工作人员数
};

class VentCaculate
{
public:
		VentCaculate(AcDbObjectId objId);
		~VentCaculate();
		/*static */void ensureAir();
private:
	void initDatas(AcDbObjectId objId);
	/*static */double afterDampAirCacul();
	double speedAirCacul();
	double powerAirCacul();
	double tempAirCacul();
	double workerAirCacul();

	double getVentLenth(AcDbObjectId objId);
	
	LinkedGE* getLinkedGE(AcDbObjectId objId);

	void cheakQ();
public:
	//计算结果
	double m_rqAirflow;		//最终需风量
	//double m_airLeakage;	//漏风备用系数
	//double m_windage;		//风阻
	int m_basis;	//最大需风量的计算依据
	
	CString m_dynamiteWay;	//引爆方式

private:
	//计算所需要的变量
	double m_lenth;			//巷道长度
	double m_area;			//巷道断面面积
	
	//计算所需参数
	double m_time;			//通风时间
	double m_speed;			//排尘风速
	double m_A;				//炸药量
	double m_N;				//工作人数
	double m_g;				//平均排量
	double m_q0;			//风量指标
	double m_PN;			//功率数
	double m_C1;			//允许浓度
	double m_inT;			//进风端气温
	double m_outT;			//排风端气温
	double m_allPower;		//总散热量
	double m_Cp;			//空气定热比
	double m_ro;			//空气密度
	
	//基本参数
	CString m_method;				//通风方法
	CString m_way;				//通风方式

};

