#pragma once

#include "../MineGE/DataLink.h"

class RockGateLink : public DataLink
{
protected:
	virtual void regDatas()
	{
		linkStringData( _T( "名称" ), &m_name );
		linkDoubleData( _T( "长度" ), &m_length );
		linkDoubleData( _T( "宽度" ), &m_width );
		linkDoubleData( _T( "高度" ), &m_height );
		linkDoubleData( _T( "最小法距" ), &m_dist );
		linkDoubleData( _T( "钻孔半径" ), &m_radius );
		linkStringData( _T( "迎头底板中心点坐标" ), &m_pt );
		linkDoubleData( _T( "左帮保护" ), &m_left );
		linkDoubleData( _T( "右帮保护" ), &m_right );
		linkDoubleData( _T( "上帮保护" ), &m_top );
		linkDoubleData( _T( "下帮保护" ), &m_bottom );
	}

public:
	CString m_pt;
	double m_length;
	double m_width;
	double m_height;
	double m_left;
	double m_right;
	double m_top;
	double m_bottom;
	double m_dist;
	double m_radius;
	CString m_name;
};

class CoalSurfaceLink : public DataLink
{
protected:
	virtual void regDatas()
	{
		linkDoubleData( _T( "厚度" ), &m_thick );
		linkDoubleData( _T( "倾角" ), &m_angle );
	}

public:
	double m_thick;
	double m_angle;
};

class DrillSiteLink : public DataLink
{
protected:
	virtual void regDatas()
	{
		linkStringData( _T( "名称" ), &m_name );
		linkIntData( _T( "位置" ), &m_leftOrRight );
		linkDoubleData( _T( "与迎头的距离" ), &m_dist );
		linkDoubleData( _T( "宽度" ), &m_width );
		linkDoubleData( _T( "高度" ), &m_height );
		linkIntData( _T( "起始编号" ), &m_start );
	}

public:
	CString m_name;
	int m_leftOrRight;
	double m_dist;
	double m_width;
	double m_height;
	int m_start;
};

class PoreLink : public DataLink
{
protected:
	virtual void regDatas()
	{
		linkIntData( _T( "编号" ), &m_num );
		linkDoubleData( _T( "半径" ), &m_radius );
		linkStringData( _T( "坐标" ), &m_pt );
	}

public:
	int m_num;
	double m_radius;
	CString m_pt;
};