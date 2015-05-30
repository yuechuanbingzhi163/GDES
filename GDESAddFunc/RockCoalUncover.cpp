#include "StdAfx.h"

#include <math.h>

#define PI 3.1415926535897932384626433832795
//度数转弧度
#define DegToRad(x) x*0.01745329251994329576923690768489
//弧度转度数
#define RadToDeg(x) x*57.295779513082320876798154814105

class RockGate
{
public:
	//构造函数
	RockGate() : origin(AcGePoint3d::kOrigin), beta(90), Dz(0) {}

	//设置基点坐标
	void setOrigin(const AcGePoint3d& origin)
	{
		this->origin = origin;
	}
	//获取基点坐标
	AcGePoint3d getOrigin() const
	{
		return this->origin;
	}
	//设置煤层参数
	void setCoalParams(double alpha, double beta, double h)
	{
		this->alpha = alpha;
		this->beta = beta;
		this->h = h;
	}
	//获取煤层参数
	void getCoalParams(double& alpha, double& beta, double& h) const
	{
		alpha = this->alpha;
		beta = this->beta;
		h = this->h;
	}
	//设置石门巷道断面参数
	void setTunnelParams(double H, double W1, double W2)
	{
		this->H = H;
		this->W1 = W1;
		this->W2 = W2;
	}
	//获取石门巷道断面参数
	void getTunnelParams(double& H, double& W1, double& W2) const
	{
		H = this->H;
		W1 = this->W1;
		W2 = this->W2;
	}
	//设置石门参数
	void setRockGateParams(double f1, double f2, double d1, double d2)
	{
		this->f1 = f1;
		this->f2 = f2;
		this->d1 = d1;
		this->d2 = d2;
	}
	//获取石门参数
	void getRockGateParams(double& f1, double& f2, double& d1, double& d2) const
	{
		f1 = this->f1;
		f2 = this->f2;
		d1 = this->d1;
		d2 = this->d2;
	}
	//最小法距点坐标
	AcGePoint3d minNormalDistPoint() const
	{
		return origin + AcGeVector3d(W1*0.5, H, 0);
	}


	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////

	//计算见煤平面方程
	void beginCoalSurface(AcGePlane& plane)
	{
		coalSurface(plane, 0);
	}
	//计算止煤平面方程
	void endCoalSurface(AcGePlane& plane)
	{
		//见煤面加上煤厚就是止煤面
		coalSurface(plane, h);
	}

private:
	//最小法距
	double minNormalDist() const
	{
		using namespace std;
		//沿着F点(最小法距点向煤层做垂线)
		return S1*sin(DegToRad(beta));
	}
	//如果只考虑石门水平，则delta等于alpha(煤层倾角)
	double getDelta() const
	{
		return this->alpha;
	}

	void coalSurface(AcGePlane& plane, double h)
	{
		using namespace std;
		double Lf = minNormalDist() + h;
		double D = Lf*sqrt(1+1.0/tan(DegToRad(getDelta())));
		D += Dz*cos(DegToRad(beta));
		D += H/tan(DegToRad(beta));
		D += W1*-0.5*sin(DegToRad(beta));

		double A = sin(DegToRad(beta));
		double B = -1.0/tan(DegToRad(getDelta()));
		double C = -1.0*cos(DegToRad(beta));

		//设置平面参数
		plane.set(A, B, C, D);
	}

private:
	AcGePoint3d origin; // 石门巷道断面下部中心点的真实坐标
	double Dz;          // 石门断面的z坐标(默认为0)

	double alpha; // 煤层倾角
	double beta; // 石门轴线与煤层走向的夹角
	double h;    // 煤层厚度
	double H;    // 石门巷道高度
	double W1, W2; // 石门巷道宽度(W1、W2为梯形巷道上、下底，非梯形巷道时W1 = W2表示巷道宽)
	double S1; // 石门轴线距煤层距离
	double f1; // 石门揭煤轮廓外上控距离
	double f2; // 石门揭煤轮廓外下控距离
	double d1; // 石门揭煤轮廓外左控距离
	double d2; // 石门揭煤轮廓外右控距离
};
