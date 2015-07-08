#pragma once

#include "RcuAcUiBaseDlg.h"

#include "RcuDataLink.h"

//石门揭煤编辑对话框(从"钻场设计"右键菜单中调用该对话框)
class RcuEditDrillSiteDlg : public RcuAcUiBaseDlg
{
	DECLARE_DYNAMIC(RcuEditDrillSiteDlg)

public:
	RcuEditDrillSiteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RcuEditDrillSiteDlg();

// 对话框数据
	enum { IDD = IDD_RCU_DRILL_SITE_EDIT_DLG };

public:
	//钻场图元id
	AcDbObjectId m_drill_site;
	//读/写数据
	void readFromDataLink( DrillSiteLink& ds_link, CoalSurfaceLink& cs_link);
	void writeToDataLink( DrillSiteLink& ds_link, CoalSurfaceLink& cs_link);

private:
	//对话框与钻场图元之间交换数据
	void exchangeDrillSiteData( DrillSiteLink& ds_link, bool save);
	//对话框与煤层图元之间交换数据
	void exchangeCoalSurfaceData(CoalSurfaceLink& cs_link, bool save);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
	double m_x;
	double m_y;
	double m_z;
	double m_width;
	double m_height;
	double m_left;
	double m_right;
	double m_top;
	double m_bottom;
	double m_gas_radius;
	double m_thick;
	double m_angle;
	double m_dist;
	double m_pore_size;

	//"确定"按钮消息
	afx_msg void OnBnClickedOk();

	//虚函数重载
	virtual BOOL OnInitDialog();
};
