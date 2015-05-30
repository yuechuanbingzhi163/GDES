#include "StdAfx.h"

#include <math.h>

#define PI 3.1415926535897932384626433832795
//度数转弧度
#define DegToRad(x) x*0.01745329251994329576923690768489
//弧度转度数
#define RadToDeg(x) x*57.295779513082320876798154814105
//平方
#define square(x)  x*x
//余切
#define cot(x) 1.0/tan(x)

//向量在水平面的方向角和仰角
void vector_to_angle(const AcGeVector3d& v, double& ang1, double& ang2)
{
	//向量在xoy平面上的投影
	AcGeVector3d u = v.orthoProject(AcGeVector3d::kZAxis);
	acutPrintf(_T("\n投影值:x=%.3f, y=%.3f z=%.3f"), u.x, u.y, u.z);
	ang1 = u.angleTo(AcGeVector3d::kXAxis, AcGeVector3d::kZAxis);
	ang2 = u.angleTo(v);
}

class RockGate
{
public:
	//构造函数
	RockGate() 
		: origin(AcGePoint3d::kOrigin), beta(PI*0.5),
	      w1(0), w2(0), h1(0), h2(0),r0(0) {}

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
	//设置迎头钻孔参数
	void setHeadDrillParams(double w1, double w2)
	{
		this->w1 = w1;
		this->w2 = w2;
	}
	//设置钻机参数
	void setDrillMachineParams(double h1, double h2)
	{
		this->h1 = h1;
		this->h2 = h2;
	}
	void setDrillDiameter(double r0)
	{
		this->r0 = r0;
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
	//止煤面的宽和高
	void drillExtent(double& Lw, double& Lh)
	{
		AcGePoint3d p12 = k2(1);
		AcGePoint3d p22 = k2(2);
		AcGePoint3d p32 = k2(3);

		Lw = sqrt(square(p12.x-p22.x) + square(p12.y-p22.y) + square(p12.z-p22.z));
		Lh = sqrt(square(p12.x-p32.x) + square(p12.y-p32.y) + square(p12.z-p32.z));
	}
	//钻孔个数及间距
	void drillNumber(double Lw, double Lh, int& N1, int& N2, double& e1, double& e2)
	{
		N1 = int(Lw/r0)+2;
		N2 = int(Lh/r0)+2;
		e1 = Lw/(N1-1);
		e2 = Lh/(N2-1);
	}

private:
	//最小法距
	double minNormalDist() const
	{
		using namespace std;
		//沿着F点(最小法距点向煤层做垂线)
		return S1*sin(beta);
	}
	//如果只考虑石门水平，则delta等于alpha(煤层倾角)
	double getDelta() const
	{
		return this->alpha;
	}

	double getPhi() const
	{
		using namespace std;
		return atan(abs(tan(getDelta()))*cos(beta));
	}

	void coalSurface(AcGePlane& plane, double h) const
	{
		using namespace std;
		double L = origin.z;
		double Lf = minNormalDist() + h;
		double D = Lf*sqrt(1+square(cot(getDelta())));
		D += L*cos(beta);
		D += H*cot(beta);
		D += 0.5*W1*sin(beta);

		double A = sin(beta);
		double B = -1*cot(getDelta());
		double C = -1.0*cos(beta);

		//设置平面参数
		plane.set(A, B, C, D);
	}

	AcGePoint3d k0(int k) const
	{
		using namespace std;
		int c1 = (k%2==0)?1:-1;
		int c2 = ((1-k/3)%2==0)?1:-1;
		int c3 = ((k/3)%2==0)?1:-1;
		double x0 = c1*(W1*0.5 - w1*(1-c1))*0.5-w1*(1+c1*0.5);
		double y0 = h2*(1-c2)*0.5+h1*(1-c3)*0.5;
		double z0 = origin.z;
		return AcGePoint3d(x0,y0,z0);
	}

	AcGePoint3d k1(int k) const
	{
		using namespace std;
		double L = origin.z;
		double Lf = minNormalDist();
		int c1 = (k%2==0)?1:-1;
		int c2 = ((1-k/3)%2==0)?1:-1;
		int c3 = ((k/3)%2==0)?1:-1;
		double x1 = c1*(W2 + (1-c1)*d1 + (1+c1)*d2)*0.5 - c1*r0*cos(beta)*0.5;
		double y1 = (H + f1 - 0.5*r0*sin(getPhi()))*(1-c2)*0.5-(f2-0.5*r0*sin(getPhi()))*(1-c3);
		double z1 = (x1*sin(beta) - y1*cot(getDelta()) + Lf*sqrt(1+square(cot(getDelta()))) + L*cos(beta) + H*cot(getDelta()) - 0.5*W1*sin(beta)) / cos(beta);
		return AcGePoint3d(x1,y1,z1);
	}

	AcGePoint3d k2(int k) const
	{
		using namespace std;

		AcGePoint3d p0 = k0(k);
		AcGePoint3d p1 = k1(k);
		double Lf = minNormalDist();
		double L = origin.z;

		double a= p0.y*cot(getDelta()) + p0.z*cos(beta) -p0.x*sin(beta) - (Lf+h)*sqrt(1+square(cot(getDelta()))) - L*cos(beta) - H*cot(getDelta()) +0.5*W1*sin(beta);
		double b = (p1.x -p0.x)*sin(beta) - (p1.y - p0.y)*cot(getDelta()) - (p1.z-p0.z)*cos(beta);
		double tk = a/b;

		double x2 = (p0.x+tk*p1.x)/(1-tk);
		double y2 = (p0.y+tk*p1.y)/(1-tk);
		double z2 = (p0.z+tk*p1.z)/(1-tk);
		return AcGePoint3d(x2,y2,z2);
	}

private:
	AcGePoint3d origin; // 石门巷道断面下部中心点的坐标

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

	double w1, w2; // 石门迎头钻孔左右边距
	double h1, h2; // 钻机的最小最大工作高度
	double r0;     // 钻孔卸压直径
};
