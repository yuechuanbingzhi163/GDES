#include "GraphData.h"
#include <cmath>
#include <vector>

// m的倒n阶[n(n-1)(n-2)...(n-m+1)]
static int factorial(int m, int n)
{
	if(n>m) return 0;

	int k = m, c = 1;
	for(int j=0;j<n;j++)
	{
		c *= k; k--;
	}
	return c;
}

// 计算多项式的导数
// 多项式f = a0 + a1*q + a2*q^2
// coefs  - 多项式的系数
// n      - 几阶导数
// q      - 输入的风量
static double CaclPolyomial(const DoubleArray& coefs, int n, double q)
{
	double s = 0, t = 1;
	for(size_t i=n;i<coefs.size();i++)
	{
		s += factorial(i, n)*coefs[i]*t;
		t *= q;
	}
	return s;
}

FanData::FanData()
    : a0(0),a1(0),a2(0)
{
}

double FanData::h0( double q ) const
{
	DoubleArray coefs;
	coefs.push_back(a0);
	coefs.push_back(a1);
	coefs.push_back(a2);
	//计算多项式的0阶导
	return CaclPolyomial(coefs, 0, q);
}

double FanData::h1( double q ) const
{
	DoubleArray coefs;
	coefs.push_back(a0);
	coefs.push_back(a1);
	coefs.push_back(a2);
	//计算多项式的1阶导
	return CaclPolyomial(coefs, 1, q);
}

EdgeData::EdgeData()
    : r(0), q(0), fq(0), adjust_r(0), adjust_h(0), density(1.2),alevel(AL_2)
{

}

double EdgeData::getR() const
{
	return r;
}

double EdgeData::getSumR() const
{
    return (r+adjust_r);
}

double EdgeData::f0() const
{
    return getSumR()*q*fabs(q);
}

double EdgeData::f1() const
{
    return fabs(2*getSumR()*q); // >=0
}

bool EdgeData::isNull() const
{
    return std::abs(r)<1e-5;
}

double EdgeData::getDeltaR() const
{
	return (fabs(q) < 1e-4) ? 0 : adjust_h / q;
}

NodeData::NodeData() : p(0),x(0),y(0),z(0)
{

}
