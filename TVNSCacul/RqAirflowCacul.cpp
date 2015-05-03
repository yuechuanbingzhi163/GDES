#include "StdAfx.h"
#include "RqAirflowCacul.h"
#include "NetWorkData.h"
#include <cmath>

//#include "../MineGE/LinkedGE.h"
#include "../MineGE/HelperClass.h"

extern double GetMaxValues(double a, double b);
extern TWS_DataLink* TWSDatas(AcDbObjectId objId);
extern BaseparamDataLink* BaseparamDatas(AcDbObjectId objId);
extern Vent_DataLink* CaculParamDatas(AcDbObjectId objId);


VentCaculate::VentCaculate(AcDbObjectId objId):m_basis(0),m_rqAirflow(0)
{
	initDatas(objId);
}

VentCaculate::~VentCaculate()
{

}

void VentCaculate::initDatas(AcDbObjectId objId)
{
	TWS_DataLink* ts = TWSDatas(objId);
	//m_lenth = getVentLenth(objId);
	m_lenth = 100;
	m_area = ts->area;

	Vent_DataLink* vtl = CaculParamDatas(objId);
	m_time = vtl->m_time;				//通风时间
	m_speed = vtl->m_speed;				//排尘风速
	m_A = vtl->m_A;						//炸药量
	m_N = vtl->m_N;						//工作人数
	m_dynamiteWay = vtl->m_dynamiteWay;	//引爆方式
	m_g = vtl->m_g;						//平均排量
	m_q0 = vtl->m_q0;					//风量指标
	m_PN = vtl->m_PN;					//功率数
	m_C1 = vtl->m_C1;					//允许浓度
	m_inT = vtl->m_inT;					//进风端气温
	m_outT = vtl->m_outT;				//排风端气温
	m_allPower = vtl->m_allPower;		//总散热量
	m_Cp = vtl->m_Cp;					//空气定热比
	m_ro = vtl->m_ro;					//空气密度

	BaseparamDataLink *bpl = BaseparamDatas(objId);
	m_method = bpl->method;
	m_way = bpl->way;
}

double VentCaculate::afterDampAirCacul()
{
	double dampQ = 0;

	if (0 == m_lenth)
	{
		//AfxMessageBox(_T("请检测工作面长度!"),MB_OK | MB_ICONSTOP);
		return 0;
	}
	if (0 == m_area)
	{
		AfxMessageBox(_T("请检测工作面断面面积!"),MB_OK | MB_ICONSTOP);
		return 0;
	}

	if (_T("抽出式") == m_method)//抽出式
	{
		double throwLenth;

		if (_T("电雷管起爆") == m_dynamiteWay )//电雷管起爆
		{
			throwLenth = 15 + m_A / 5;
			//acutPrintf(_T("\n电雷管"));
		}

		else //if (_T("火雷管起爆") == m_dynamiteWay )//火雷管起爆
		{
			throwLenth = 15 + m_A;
			//acutPrintf(_T("\n火雷管"));
		}

		double v1 = m_A * throwLenth * m_area;
		dampQ = 18 * pow(v1,0.5) / m_time;

		//acutPrintf(_T("\n抽出式"));
	}

	else
	{
		double v1 = m_A * m_lenth * m_area;
		dampQ = 19 * pow(v1,0.5) / m_time;
		
		if (_T("压入式") == m_method)
		m_dynamiteWay = _T("压入式不考虑该项");
	}
	acutPrintf(_T("\n按炮烟计算风量:%lfm^3/s"),dampQ);

	return dampQ;
}

double VentCaculate::speedAirCacul()
{
	double speedQ = m_speed * m_area;
	acutPrintf(_T("\n按排尘风速计算风量:%lfm^3/s"),speedQ);
	return speedQ;
}

double VentCaculate::powerAirCacul()
{
	double powerQ1 = 0,powerQ2 = 0;
	if(0 != m_C1)
	{
		powerQ1 = 10 * m_g / 36 / m_C1;
	}
	else
	{
		//AfxMessageBox(_T("有害气体成分的允许浓度没有值"),MB_OK | MB_ICONSTOP);
		return 0;
	}
	powerQ2 = m_q0 * m_PN / 60;
	double powerQ = GetMaxValues(powerQ1,powerQ2);
	acutPrintf(_T("\n按单位功率计算风量:%lfm^3/s"),powerQ);
	return powerQ;
}

double VentCaculate::tempAirCacul()
{
	double tempQ = 0;
	double v = m_Cp * m_ro * (m_outT - m_inT);
	if (0 == v)
	{
		//AfxMessageBox(_T("进、排风端气温不能相等"),MB_OK | MB_ICONSTOP);
		return 0;
	}
	else if(v < 0)
	{
		//AfxMessageBox(_T("进风端气温不能大于排风端气温"),MB_OK | MB_ICONSTOP);
		return 0;
	}
	else
	{
		tempQ = m_allPower / v;
	}
	acutPrintf(_T("\n按热量调节计算风量:%lfm^3/s"),tempQ);
	return tempQ;
}

double VentCaculate::workerAirCacul()
{
	double workers = 4 * m_N / 60;
	acutPrintf(_T("\n按工作面人数计算风量:%lfm^3/s"),workers);
	return workers;
}

void VentCaculate::ensureAir()
{
	double reqArray[5];
	//if(-1 == (reqArray[0] = afterDampAirCacul())) return;
	//if(-1 == (reqArray[1] = speedAirCacul()))		return;
	//if(-1 == (reqArray[2] = powerAirCacul()))		return;
	//if(-1 == (reqArray[3] = tempAirCacul()))		return;
	//if(-1 == (reqArray[4] = workerAirCacul()))	return;

	reqArray[0] = afterDampAirCacul();
	reqArray[1] = speedAirCacul();
	reqArray[2] = powerAirCacul();
	reqArray[3] = tempAirCacul();
	reqArray[4] = workerAirCacul();

	double reqQ = 0;
	//取最大风量
	for (int i = 0; i< 5;i++)
	{
		reqQ = GetMaxValues(reqArray[i],reqQ);
	}

	//判断最大风量计算的依据
	for(int i = 0;i < 5; i++)
	{
		if (reqArray[i] == reqQ)
		{
			m_basis = i;
			break;
		}
	}

	m_rqAirflow = reqQ;
	cheakQ();
	if (m_rqAirflow <= 0)
	{
		return;
	}

	//acutPrintf(_T("\n总长度:%f"),m_lenth);
}

void VentCaculate::cheakQ()
{
	double cheakq = 4 * m_area;
	CString msg;
	switch(m_basis)
	{
	case AFTER_DAMP:
		msg = _T("最大风量为按炮烟计算所得风量");
		break;
	case DUST_SPEED:
		msg = _T("最大风量为按排尘风速计算所得风量");
		break;
	case UNIT_POWER:
		msg = _T("最大风量为按单位功率风量指标计算所得风量");
		break;
	case HEAT_ADJT:
		msg = _T("最大风量为按热量调节计算所得风量");
		break;
	case WORKER_NUM:
		msg = _T("最大风量为按工作面人数计算所得风量");
		break;
	default:
		break;
	}

	CString msgAll;
	if (cheakq < m_rqAirflow)
	{
		msgAll = _T("计算失败,风速超限!");
		m_rqAirflow = 0;
		msgAll.Append(_T("\n"));
		msgAll.Append(msg);
		AfxMessageBox(msgAll);
		return;
	}
	else if (m_rqAirflow <= 0 )
	{
		msgAll = _T("计算失败\n所给参数不全");
		AfxMessageBox(msgAll);
		return;
	}
	//else
	//{
	//	msgAll = _T("计算成功!");
	//}
}

double VentCaculate::getVentLenth(AcDbObjectId objId)
{
	AcDbObjectIdArray objIds;
	DrawHelper::FindMineGEs(_T("Tunnel"),objIds);
	int len = objIds.length();

	AcGePoint3d spt,ept;
	//获取选择工作面的始末节点
	LinkedGE *pEdge = getLinkedGE(objId);
	pEdge->getSEPoint(spt,ept);

	AcGePoint3dArray findedPts;
	findedPts.append(spt);

	for(int i = 0; i < len; i++)
	{
		pEdge = getLinkedGE(objIds[i]);
		pEdge->getSEPoint(spt,ept);
		//acutPrintf(_T("\n%d->始节点:(%f,%f),末节点(%f,%f)"),i,spt.x,spt.y,ept.x,ept.y);
		if (findedPts.contains(spt) && !findedPts.contains(ept))
		{
			findedPts.append(ept);
			i = -1;
		}
		else if (findedPts.contains(ept) && !findedPts.contains(spt))
		{
			findedPts.append(spt);
			i = -1;
		}
	}

	CString ventLenthStr;
	DataHelper::GetPropertyData( objId, _T( "长度" ), ventLenthStr);
	double ventLenth = _tstof(ventLenthStr);

	for (int i = 0;i < len; i++)
	{		
		//需要排除进风巷和回风巷
		CString name;
		bool isVentTunel = false;
		DataHelper::GetPropertyData( objIds[i], _T( "名称" ), name );

		if ( _T("进风巷") != name && _T("回风巷") != name)
		{
			isVentTunel = true;
		}

		//获取其他巷道的始末节点
		pEdge = getLinkedGE(objIds[i]);
		pEdge->getSEPoint(spt,ept);

		CString lenthStr;
		DataHelper::GetPropertyData( objIds[i], _T( "长度" ), lenthStr);
		double lenth = _tstof(lenthStr);
		
		if ((findedPts.contains(spt)||findedPts.contains(ept))&& isVentTunel)
		{
			//acutPrintf(_T("\n始节点:(%f,%f),末节点(%f,%f)"),spt.x,spt.y,ept.x,ept.y);
			ventLenth += lenth;
		}
	}
	return ventLenth;
}

LinkedGE* VentCaculate::getLinkedGE(AcDbObjectId objId)
{
	AcDbObject* pObj;
	acdbOpenObject( pObj, objId, AcDb::kForRead );

	LinkedGE* pEdge = LinkedGE::cast( pObj );
	pObj->close();

	return pEdge;
}