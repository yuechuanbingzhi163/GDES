#pragma once

#include "../MineGE/DataLink.h"

//4类石门揭煤图元数据关联,减少不必要的字符串与其他数据类型的转换代码
class CoalSurfaceLink : public DataLink
{
public:
	CoalSurfaceLink() : m_thick(0), m_angle(0), m_width(0), m_height(0), 
		                m_gas_radius(3)
	{

	}
protected:
	virtual void regDatas()
	{
		linkDoubleData( _T( "厚度" ), &m_thick );
		linkDoubleData( _T( "倾角" ), &m_angle );
		linkDoubleData( _T( "钻孔抽采半径" ), &m_gas_radius );
		//带$号的字段表示该字段仅用于内部计算,不应该也不需要显示在对话框界面上
		linkDoubleData( _T( "$抽采宽度" ), &m_width );
		linkDoubleData( _T( "$抽采高度" ), &m_height );
		linkPointData(_T("$中心点坐标"), &m_pt);
		linkVectorData(_T("$平面法向量"), &m_normV);
		linkVectorData(_T("$煤层走向向量"), &m_headV);
		linkVectorData(_T("$煤层倾向向量"), &m_dipV);
	}

public:
	double m_thick;
	double m_angle;
	double m_width;
	double m_height;
	//坐标内部存储是用字符串表示的,xyz之间通过逗号分隔
	AcGePoint3d m_pt;
	AcGeVector3d m_normV;
	AcGeVector3d m_headV;
	AcGeVector3d m_dipV;
	double m_gas_radius;
};

class DrillSiteLink : public DataLink
{
public:
	DrillSiteLink() : m_dist(0), m_width(0), m_height(0), 
					  m_left(0), m_right(0), m_top(0), m_bottom(0),
					  m_pore_size(0)
	{

	}
protected:
	virtual void regDatas()
	{
		linkStringData( _T( "名称" ), &m_name );
		linkDoubleData( _T( "宽度" ), &m_width );
		linkDoubleData( _T( "高度" ), &m_height );
		linkDoubleData( _T( "垂距" ), &m_dist );
		linkPointData( _T( "迎头底板中心点坐标" ), &m_pt );
		linkDoubleData( _T( "左帮保护" ), &m_left );
		linkDoubleData( _T( "右帮保护" ), &m_right );
		linkDoubleData( _T( "上帮保护" ), &m_top );
		linkDoubleData( _T( "下帮保护" ), &m_bottom );
		linkDoubleData( _T( "孔径" ), &m_pore_size );
		//带$号的字段表示该字段仅用于内部计算,不应该也不需要显示在对话框界面上
		linkPointData( _T( "$底帮坐标" ), &m_pt );
	}

public:
	CString m_name;
	double m_dist;
	double m_width;
	double m_height;
	//坐标内部存储是用字符串表示的,xyz之间通过逗号分隔
	AcGePoint3d m_pt;
	double m_pore_size;
	double m_left;
	double m_right;
	double m_top;
	double m_bottom;
};

class PoreLink : public DataLink
{
public:
	PoreLink() : m_pore_num(0), m_pore_size(0)
	{

	}
protected:
	virtual void regDatas()
	{
		linkIntData( _T( "编号" ), &m_pore_num );
		linkDoubleData( _T( "孔径" ), &m_pore_size );
		//带$号的字段表示该字段仅用于内部计算,不应该也不需要显示在对话框界面上
		linkPointData( _T( "$坐标" ), &m_pt );
	}

public:
	int m_pore_num;
	double m_pore_size;
	//坐标是用字符串表示的,xyz之间通过逗号分隔
	//可通过ArxUtilHelper::StringToPoint3d静态方法进行转换
	AcGePoint3d m_pt;
};