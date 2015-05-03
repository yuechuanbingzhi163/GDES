#include "StdAfx.h"
#include "LeakageCacul.h"
//#include "LeakageStandByDlg.h"
#include "NetWorkData.h"

#include "../DefGE/Chimney.h"
#include "../MineGE/HelperClass.h"

extern LeakageCacul_DataLink* LeakageCaculDatas(AcDbObjectId objId);
extern Chimney_DataLink* ChimneyDatas(AcDbObjectId objId );

LeakageCaculate::LeakageCaculate( AcDbObjectId objId ):m_leakage(0)
{
	m_objId = objId;
	initDatas();
}

LeakageCaculate::~LeakageCaculate()
{

}

void LeakageCaculate::initDatas()
{
	LeakageCacul_DataLink *lcdl = LeakageCaculDatas(m_objId);
	Chimney_DataLink *cdl = ChimneyDatas(m_objId);
	m_meterial = cdl->tm;
	m_chimLenth = cdl->lenth;
	m_joints = cdl->joints;
	m_diam = cdl->diameter * 0.001; //单位转换成m
	m_windage = cdl->windage;

	if (_T("不使用该方法计算") ==  lcdl->hmLeakage )
	{
		m_hmLeakage = -1;
		//acutPrintf(_T("不使用方法一"));
	}
	else
	{
		m_hmLeakage = _tstof(lcdl->hmLeakage) * 0.01;//单位有%转到实际单位 
	}

	if (_T("不使用该方法计算") ==  lcdl->leakageFactor )
	{
		m_leakageFactor = -1;
		//acutPrintf(_T("不使用方法二"));
	}
	else
	{
		m_leakageFactor = _tstof(lcdl->leakageFactor);
	}

}

double LeakageCaculate::LeakageCaculRet()
{
	if ( 0 == m_windage )
	{
		AfxMessageBox(_T("风筒的风阻没有计算!"),MB_OK | MB_ICONSTOP);
		return -1;
	}

	if ( 0 == m_chimLenth)
	{
		AfxMessageBox(_T("风筒的参数没有给定!"),MB_OK | MB_ICONSTOP);
		return -1;
	}

	if (true == isLinkedNext())
	{
		m_joints += 1;
	}

	if (-1 != m_hmLeakage)
	{
		if (0 == m_hmLeakage)
		{
			AfxMessageBox(_T("漏风系数计算参数没有输入"),MB_OK | MB_ICONSTOP);
			return -1;
		}

		if((100 - m_hmLeakage * m_chimLenth) <= 0)
		{
			AfxMessageBox(_T("百米漏风率过大"),MB_OK | MB_ICONSTOP);
			return -1;
		}
		m_leakage = 100 / (100 - m_hmLeakage * m_chimLenth);
	}
	if (-1 != m_leakageFactor)
	{
		if (0 == m_hmLeakage)
		{
			AfxMessageBox(_T("漏风系数计算参数没有输入"),MB_OK | MB_ICONSTOP);
			return -1;
		}

		if (_T("金属风筒") == m_meterial)
		{
			metalCacul();
		}

		else if (_T("胶布风筒") == m_meterial)
		{
			blanketCacul();
		}
	}

	return m_leakage;
}

void LeakageCaculate::metalCacul()
{
	double lenth = m_chimLenth / (1 + m_joints); //风筒断长
	double windage = m_windage / m_chimLenth;	//每米风筒的风阻
	
	double leakage = 1 + (m_leakageFactor * m_diam * m_joints * pow(windage * lenth,0.5))/3;
	m_leakage = pow(leakage,2);
}

void LeakageCaculate::blanketCacul()
{
	if (1 - m_joints * m_leakageFactor <= 0)
	{
		AfxMessageBox(_T("接头个数过多\n或者漏风系数过大"),MB_OK | MB_ICONSTOP);
		return;
	}
	m_leakage = 1 /( 1 - m_joints * m_leakageFactor);
}

bool LeakageCaculate::isLinkedNext()
{
	bool isLinked = false;
	AcDbEntity *pEnt;
	acdbOpenAcDbEntity(pEnt,m_objId,AcDb::kForRead);
	Chimney *pChim = Chimney::cast(pEnt);
	pEnt->close();

	AcGePoint3dArray pts = pChim->getControlPoint();
	//int ptsNum = pts.length();

	AcDbObjectIdArray chimsIds;
	DrawHelper::FindMineGEs(_T("Chimney"),chimsIds);

	int len = chimsIds.length();
	for (int i = 0; i < len; i++)
	{
		acdbOpenAcDbEntity(pEnt,chimsIds[i],AcDb::kForRead);
		pChim = Chimney::cast(pEnt);
		pEnt->close();

		AcGePoint3dArray ChimPts = pChim->getControlPoint();

		int ChimPtsNum = ChimPts.length();
		if ((pts.contains(ChimPts[0]) || pts.contains(ChimPts[ChimPtsNum-1])) && m_objId != chimsIds[i])
		{
			isLinked = true;
		}
	}

	return isLinked;
}