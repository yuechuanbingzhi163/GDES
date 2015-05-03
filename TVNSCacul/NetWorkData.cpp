#include "StdAfx.h"
#include "NetWorkData.h"

void TVNSDataLink::regDatas()
{
	linkStringData( _T( "名称" ), &name );
}


void TWS_DataLink::regDatas()
{
	TVNSDataLink::regDatas();

	linkDoubleData( _T( "需风量" ), &q );
	linkDoubleData( _T( "长度" ), &lenth );
	linkDoubleData( _T( "断面面积" ), &area );
}


Tunnel_DataLink::Tunnel_DataLink()
{

}

Tunnel_DataLink::~Tunnel_DataLink()
{

}

void Tunnel_DataLink::regDatas()
{
	TVNSDataLink::regDatas();
	linkDoubleData( _T( "断面面积" ), &area );
	linkDoubleData( _T( "长度" ), &lenth );
}

void BaseparamDataLink::regDatas()
{
	//linkStringData(_T("矿井名称"),&name);
	//linkStringData(_T("所属采区"),&district);
	linkStringData(_T("通风方法"),&method);
	linkStringData(_T("通风方式"),&way);
}

void Vent_InitDataLink::regDatas()
{
	linkDoubleData( _T( "通风时间" ), &T );
	linkDoubleData( _T( "空气密度" ), &ro );
	//linkDoubleData( _T( "排尘风速" ), &speed );
	linkDoubleData( _T("单位功率的风量指标"), &q0);
	linkDoubleData( _T( "空气定热比" ), &Cp);

}

void Chimney_DataLink::regDatas()
{
	TVNSDataLink::regDatas();
	linkDoubleData( _T( "长度" ), &lenth);
	linkDoubleData( _T( "直径" ), &diameter );
	linkStringData( _T( "材质" ), &tm );
	linkStringData( _T( "弯角" ), &bends );
	linkDoubleData( _T( "百米漏风率" ), &leakage );
	linkDoubleData( _T( "摩擦系数" ), &friction );
	linkIntData( _T( "接头数" ), &joints );
	linkDoubleData( _T( "漏风系数" ), &leakageFactor );
	linkDoubleData( _T( "风筒风阻" ), &windage );
	linkDoubleData( _T( "百米风阻" ), &hmWindage );

}

void Vent_DataLink::regDatas()
{
	linkDoubleData( _T( "通风时间" ), &m_time );
	linkDoubleData( _T( "空气密度" ), &m_ro );
	linkDoubleData( _T( "排尘风速" ), &m_speed );
	linkDoubleData( _T("单位功率的风量指标"), &m_q0);
	linkDoubleData( _T( "空气定热比" ), &m_Cp);

	linkDoubleData( _T("一次爆破炸药消耗量"), &m_A );
	linkStringData( _T( "引爆方式" ), &m_dynamiteWay );
	linkDoubleData( _T( "工作人数" ), &m_N );
	linkDoubleData( _T("有害成分的平均排量"), &m_g);
	linkDoubleData( _T("有害成分的允许浓度"), &m_C1);

	linkDoubleData( _T("柴油设备功率数"), &m_PN );
	linkDoubleData( _T( "进风端气温" ), &m_inT );
	linkDoubleData( _T( "排风端气温" ), &m_outT );
	linkDoubleData( _T("热源总散热量"), &m_allPower );

}

void LeakageCacul_DataLink::regDatas()
{
	linkStringData(_T("百米漏风率"),&hmLeakage);
	linkStringData(_T("漏风系数"),&leakageFactor);
}