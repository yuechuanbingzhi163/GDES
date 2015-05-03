#pragma once

#include "../MineGE/DataLink.h"

//typedef AcArray<TWS_DataLink> TWS_DataLinkArray;

class TVNSDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	CString name;                 // 名称
};

// 掘进工作面模型
class TWS_DataLink : public TVNSDataLink
{
protected:
	virtual void regDatas();

public:
	double area;                  // 断面面积
	double lenth;                 // 工作面的长度
	double q;

};

// 其它巷道
class Tunnel_DataLink : public TVNSDataLink
{
public:
	Tunnel_DataLink();
	~Tunnel_DataLink();

protected:
	virtual void regDatas();

public:
	double area;                  // 断面面积
	double lenth;
};


//用于读取数据的风筒数据链接
class Chimney_DataLink : public TVNSDataLink
{
protected:
	virtual void regDatas();

public:
	double lenth;
	CString bends;
	double diameter;      // 直径
	CString tm;          // 材质类型
	double leakage;		//百米漏风率
	int joints;			//接头个数
	double friction;	//摩擦阻力系数
	double leakageFactor; //漏风系数
	double windage;			//风阻
	double hmWindage;		//百米风阻
};

//基本参数数据
class BaseparamDataLink : public DataLink
{
protected:
	virtual void regDatas();

public:
	//CString name;                 // 矿井名
	//CString district;              // 所属采区
	CString method;						//通风方法
	CString way;						//通风方式
};

//计算参数
class Vent_InitDataLink : public DataLink
{
protected:
	virtual void regDatas();
public:
	double T;
	double ro;
	//double speed;
	double q0;
	double Cp;
};

class Vent_DataLink : public DataLink
{
protected:
	virtual void regDatas();
public:
	double m_time;			//通风时间
	double m_speed;			//排尘风速
	double m_A;				//炸药量
	double m_N;				//工作人数
	CString m_dynamiteWay;	//引爆方式
	double m_g;				//平均排量
	double m_q0;			//风量指标
	double m_PN;			//功率数
	double m_C1;			//允许浓度
	double m_inT;			//进风端气温
	double m_outT;			//排风端气温
	double m_allPower;		//总散热量
	double m_Cp;			//空气定热比
	double m_ro;			//空气密度
};

//计算参数
class LeakageCacul_DataLink : public DataLink
{
protected:
	virtual void regDatas();
public:
	CString hmLeakage;
	CString leakageFactor;
};
