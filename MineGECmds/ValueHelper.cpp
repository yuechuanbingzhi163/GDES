#include "StdAfx.h"
#include "ValueHelper.h"

#include "../MineGE/DataHelper.h"
#include "../MineGE/DataListHelper.h"

#include <fstream>
// 需要包含<fstream>才能使用
#include "AcFStream.h"

static CString GetAppPathDir()
{
	TCHAR szModulePath[_MAX_PATH];
	GetModuleFileName( _hdllInstance, szModulePath, _MAX_PATH );

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	_tsplitpath( szModulePath, drive, dir, NULL, NULL );

	TCHAR szPath[_MAX_PATH] = {0};
	_tmakepath( szPath, drive, dir, NULL, NULL );

	return CString( szPath );
}

static CString BuildPath( const CString& dir, const CString& fileName )
{
	CString path;
	path.Format( _T( "%s%s" ), dir, fileName );
	return path;
}

double ValueHelper::newTonDivided( int n,vector<double>&X,vector<double>&Y )
{
	double f=0;  
	double temp=0; 
	int i,j;
	for(i=0;i<n+1;i++)
	{  
		temp=Y[i];  
		for(j=0;j<n+1;j++)  
		{
			if(j!=i) 
				temp /= (X[i]-X[j]);
		}
		f += temp;  
	}  
	return f;  
}

double ValueHelper::getRetByNewTon( double x,vector<double>&X,vector<double> &Y )
{
	int i,j;
	double result=0;  
	for(i=0;i<X.size();i++)
	{  
		double temp=1;  
		double f=newTonDivided(i,X,Y);  
		for(j=0;j<i;j++)
		{  
			temp = temp*(x-X[j]);  
		}  
		result += f*temp;  
	}  
	return result;  
}

bool ValueHelper::Lagrange( const vector<double> & x, const vector<double> & y, const double & x1, double & y1 )
{
	y1 = 0.0;
	int n = x.size();
	for (int j = 0; j < n; j++)
	{
		double L = 1.0;
		for (int i = 0; i < n; i++)
		{
			if (i != j)
			{
				L *= (x1-x[i])/(x[j] - x[i]);
			}
		}
		y1 += L*y[j];
	}
	return true;
}

void ValueHelper::setKValueDatas( vector<double>&X,vector<double> &Y )
{
	CString dataDirName = _T( "Datas\\" );
	CString fileName;
	fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T( "不同管径的系数.txt" ) );

	AcIfstream inFile( fileName );
	while(!inFile.eof())
	{
		ACHAR cDiam[_MAX_PATH], cKValue[_MAX_PATH];
		inFile >> cDiam >> cKValue;
		if(inFile.fail()) break;
		double fDiam = _tstof(cDiam);
		double fKValue = _tstof(cKValue);

		X.push_back(fDiam);
		Y.push_back(fKValue);
		//acutPrintf(_T("\nX:%f\tY:%f\n"),fDiam,fKValue);
	}

	inFile.close();
}

double ValueHelper::getKValue( double fdiam)
{
	vector<double> X;
	vector<double> Y;
	setKValueDatas(X,Y);
	double ret;
	if(fdiam > 150)
	{
		ret = 0.71;
	}
	else
	{
		ret = getRetByNewTon(fdiam,X,Y);
	}
	return ret;
}


double ValueHelper::getKValueByLagrange( double fdiam )
{
	vector<double> X;
	vector<double> Y;
	setKValueDatas(X,Y);
	double ret;
	if(fdiam > 150)
	{
		ret = 0.71;
	}
	else
	{
		ret = Lagrange(X,Y,fdiam,ret);
	}
	return ret;

}

//void ValueHelper::setGasAirValue( const AcDbObjectId& objId )
//{
//	CString dataDirName = _T( "Datas\\" );
//	CString fileName =BuildPath ( BuildPath( GetAppPathDir(), dataDirName ),_T("gasAir.txt") );
//
//	//CString gasValueStr;
//	//AcDbObjectId objIdCacul;
//	//SingleDataObjectHelper::GetObjectId(_T("计算参数"),objIdCacul);
//
//	//DataHelper::GetPropertyData(objIdCacul,_T("工作面瓦斯浓度"),gasValueStr);
//
//	if(gasValueStr.IsEmpty()) gasValueStr = _T("0");
//	AcIfstream inFile(fileName);
//	AcStringArray fields,values;
//	//acutPrintf(_T("\n%s"),fileName);
//	if(!inFile) return;
//	CString gasValueStr0,gasAirValueStr;
//	while( !inFile.eof() )
//	{
//		ACHAR gasValueChar[_MAX_PATH], gasAirValueChar[_MAX_PATH];
//		inFile >> gasValueChar >> gasAirValueChar;
//		if(inFile.fail()) break;
//		gasValueStr0.Format(_T("%s"),gasValueChar);
//		if(gasValueStr == gasValueStr0)
//		{
//			gasAirValueStr.Format(_T("%s"),gasAirValueChar);
//			//acutPrintf(_T("\n混合瓦斯浓度:%s\n"),gasAirValueStr);
//			break;
//		}
//	}
//
//	inFile.close();
//
//	DataHelper::SetPropertyData( objId, _T( "混合瓦斯对空气的相对密度" ), gasAirValueStr );
//}