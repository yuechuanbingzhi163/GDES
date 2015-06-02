#include "StdAfx.h"
#include "Caculate.h"
#include <vector>

typedef std::vector<double> doubleVector;

static void StringsToNum( const AcStringArray& strDatas, doubleVector& doubleDatas)
{
	for(int i = 0; i < strDatas.length(); i++)
	{
		double temp = _tstof(strDatas[i].kACharPtr());
		doubleDatas.push_back(temp);
	}
}

/*
 * 参数1代表
 *	|-0矿井相对瓦斯涌出量
 *	|-1矿井总回风巷瓦斯浓度
 *	|-2当地大气压
 *	|-3矿井最大总回风风量
 *	|-4抽采泵富余系数
 *	|-5抽采系统工况系数
 *	|-6瓦斯泵台数
 * 参数2代表单台瓦斯泵额定流量、瓦斯抽采浓度、瓦斯泵压力
 * 参数3代表计算结果
*/
bool Calculate::PumpCapacityCacul( const AcStringArray& baseDatas,const AcStringArray& pumpDatas,CString& strRet )
{
	if(baseDatas.isEmpty() || pumpDatas.isEmpty()) return false;
	doubleVector dBaseDatas,dPumpDatas;
	StringsToNum(baseDatas,dBaseDatas);
	StringsToNum(pumpDatas,dPumpDatas);
	if(dBaseDatas[0] <= 0) return false;
	int pumpNum = (int)dBaseDatas[6];
	double sumPump = 0;
	for(int j = 0; j < pumpNum; j++)
	{
		double pumpQ = dPumpDatas[3*j];
		double pumpCon = dPumpDatas[3*j+1];
		double pumpP = dPumpDatas[3*j+2];

		double tmp1 = pumpQ * pumpCon* 0.01 * dBaseDatas[5] * (dBaseDatas[2]-pumpP);
		double temp = tmp1 / 2.0 / 101.325;
		sumPump += temp;
		//acutPrintf(_T("\n第%d台泵\t Q:%.2lf,C:%.2lf,P:%.2lf"),j+1,pumpQ,pumpCon,pumpP);
	}

	double ret = (dBaseDatas[1]*0.01*dBaseDatas[3] + sumPump) * 330 * 1440 * 0.0001 / dBaseDatas[0];
	strRet.Format(_T("%.2lf"),ret);
	//for(int i = 0 ; i < dBaseDatas.size(); i++)
	//{
	//	acutPrintf(_T("\n第%d个值:%.2lf"),i+1,dBaseDatas[i]);
	//}
	//acutPrintf(_T("\n第一个值:%.2lf\t第二个值:%.2lf\t第三个值:%.2lf"),dBaseDatas[1]*0.01*dBaseDatas[3],sumPump,330 * 1440 * 0.0001 / dBaseDatas[0]);
	return true;
}