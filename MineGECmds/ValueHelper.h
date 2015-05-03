#ifndef VALUE_HELPER
#define VALUE_HELPER

#include<iostream>
#include<vector>  
using namespace std;

class ValueHelper
{
public:
	//static void setGasAirValue(const AcDbObjectId& objId);

	static double getRetByNewTon(double x,vector<double>&X,vector<double> &Y);
	static bool Lagrange(const vector<double> & x, const vector<double> & y, 
		const double & x1, double & y1);

	static double getKValue( double fdiam );
	static double getKValueByLagrange(double fDiam);

private:
	static double newTonDivided(int n,vector<double>&X,vector<double>&Y);
	static void setKValueDatas(vector<double>&X,vector<double> &Y);

};
#endif