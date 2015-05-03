#pragma once

class WindageCacul
{
public:
	WindageCacul(AcDbObjectId objId);
	~WindageCacul();
	double windageCaculRet();
	double windageCaculByHM();

private:
	void setMethodByFan(AcDbObjectId objId);
	void setKsaiBEByBend();

	void initDatas(AcDbObjectId objId);

private:
	//风筒的参数
	double m_alpha;	//风筒摩擦阻力系数
	double m_lenth;	//风筒长度（全长）
	double m_diam;	//风筒直径
	int	   m_joint;	//风筒接头数
	double m_hmWindage;	//百米风阻
	CString m_bends;	//拐弯角度
	CString m_method;	//通风方法
	CString m_meter;	//风筒材质

	//计算所需参数
	double m_ksaiJO;	//风筒接头局部阻力系数,本计算取偏大值0.15(取了固定值)
	double m_ksaiBE;	//风筒拐弯局部阻力系数
	double m_ksaiON;	//风筒出口局部阻力系数(针对压入式)->1
	double m_ksaiIN;	//风筒入口局部阻力系数(针对抽出式)->0.6

	double m_ro;	//空气密度，取工作面的空气密度
};