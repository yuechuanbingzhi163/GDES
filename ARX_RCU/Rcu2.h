#pragma once

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

//另一个简化版的石门揭煤计算类
//注:角度默认使用弧度
class Rcu2
{
public:
	//构造函数
	Rcu2() : beta(PI*0.5) {}

	//设置煤层参数
	void setCoalSurf(double alpha, double thick)
	{
		this->alpha = alpha;
		//this->beta = beta;
		this->thick = thick;
	}
	//设置钻场巷道断面参数
	void setDrillSite1(double height, double width)
	{
		this->height = height;
		this->width = width;
	}
	//设置钻场的上下左右帮保护间距
	void setDrillSite2(double f1, double f2, double d1, double d2)
	{
		this->f1 = f1;
		this->f2 = f2;
		this->d1 = d1;
		this->d2 = d2;
	}
	//设置钻场距煤层面的最小法距
	void setDrillSite3(double minDist)
	{
		this->minDist = minDist;
	}
	//设置钻孔直径
	void setDrillDiameter(double d0)
	{
		this->d0 = d0;
	}

	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////

	/**
	 * 计算煤层面的抽采范围
	 * 由于我们假设迎头和煤层的走向是垂直的
	 * 抽采范围的宽度等于左右保护帮距加上巷道的宽度
	 * 抽采范围的高度计算则稍微麻烦一些
	 */
	void drillExtent(double& Lw, double& Lh)
	{
		//抽采范围宽度
		Lw = width + d1 + d2;
		//抽采范围高度
		Lh = (height + f1 + f2)/sin(alpha);
	}

	//煤层面(需要4个数据才能确定一个煤层平面)
	//平面上的一点cnt, 平面法向量normV, 煤层的走向向量headV, 煤层的倾向向量dipV
	//类似3维坐标系的normV-Z, dipV-Y, headV-X
	bool drillSurface(AcGePoint3d& cnt, AcGeVector3d& normV, AcGeVector3d& headV, AcGeVector3d& dipV)
	{
		//计算煤平面方程
		AcGePlane plane1, plane2;
		coalSurface(plane1, plane2);

		//抽采范围的中心点坐标
		//计算出左上帮和右下帮对角点,这2点的中点即为矩形的中心点
		AcGePoint3d left_top(0, 0, height+f1);
		AcGePoint3d right_bottom(0, 0, -1*f2);
		AcGeLine3d lt_ray(left_top, AcGeVector3d::kYAxis);
		AcGeLine3d rb_ray(right_bottom, AcGeVector3d::kYAxis);
		//两条射线与平面的交点
		AcGePoint3d p1, p2;
		if(Adesk::kTrue != plane1.intersectWith(lt_ray, p1)) return false;
		if(Adesk::kTrue != plane1.intersectWith(rb_ray, p2)) return false;

		//2个交点的中点即为中心点
		cnt = p1 + 0.5*(p2-p1);
		//平面法向量
		normV = plane1.normal();
		//acutPrintf(_T("\n法向量(%.2f,%.2f,%.2f)"),normV.x,normV.y,normV.z);
		//计算煤层的倾向向量
		AcGeVector3d v(-AcGeVector3d::kYAxis);
		v.rotateBy(-1*alpha, AcGeVector3d::kXAxis);
		dipV = v;
		//acutPrintf(_T("\n倾向向量(%.2f,%.2f,%.2f)"),dipV.x,dipV.y,dipV.z);
		//计算煤层的走向向量
		v.rotateBy(-0.5*PI, normV);
		headV = v;
		//acutPrintf(_T("\n走向向量(%.2f,%.2f,%.2f)"),headV.x,headV.y,headV.z);

		return true;
	}

	//钻孔个数及间距
	void drillNumber(double Lw, double Lh, int& N1, int& N2, double& e1, double& e2)
	{
		N1 = int(Lw/d0)+2;
		N2 = int(Lh/d0)+2;
		e1 = Lw/(N1-1);
		e2 = Lh/(N2-1);
	}

private:
	//计算煤平面方程(见煤和止煤)
	void coalSurface(AcGePlane& plane1, AcGePlane& plane2)
	{
		//迎头煤层的轴线距离
		double L = minDist/sin(alpha);
		//acutPrintf(_T("\n迎头煤层的轴线距离L=%.2f"),L);
		//迎头轴线与煤层的交点
		AcGePoint3d pt(0, L, 0);

		//计算平面的法向量
		AcGeVector3d v(-AcGeVector3d::kYAxis);
		v.rotateBy(PI*0.5 - alpha, AcGeVector3d::kXAxis);

		//由平面上的点和法向量确定一个平面
		plane1.set(pt, v);
		//acutPrintf(_T("\n见煤面点(%.2f,%.2f,%.2f),向量(%.2f,%.2f,%.2f)"),
		//	pt.x,pt.y,pt.z,v.x,v.y,v.z);
		plane2.set(pt - v*thick, v);
		//pt = pt - v*thick;
		//acutPrintf(_T("\n止煤面点(%.2f,%.2f,%.2f),向量(%.2f,%.2f,%.2f)"),
		//	pt.x,pt.y,pt.z,v.x,v.y,v.z);

	}

private:
	double alpha;    // 煤层倾角
	double beta;     // 钻场轴线与煤层走向的夹角(始终是90°)
	double thick;    // 煤层厚度

	double height;    // 钻场巷道高度
	double width;     // 钻场巷道宽度(假设钻场为矩形巷道)
	
	double minDist;     // 钻场距离煤层的最小法距
	double f1; // 石门揭煤轮廓外上控距离
	double f2; // 石门揭煤轮廓外下控距离
	double d1; // 石门揭煤轮廓外左控距离
	double d2; // 石门揭煤轮廓外右控距离

	double d0;     // 钻孔卸压直径
};
