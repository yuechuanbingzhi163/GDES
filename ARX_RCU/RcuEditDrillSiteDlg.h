#pragma once
#include "RcuAcUiBaseDlg.h"
#include "RcuDataLink.h"

// 钻场设计对话框
class RcuEditDrillSiteDlg : public RcuAcUiBaseDlg
{
	DECLARE_DYNAMIC(RcuEditDrillSiteDlg)

public:
	RcuEditDrillSiteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RcuEditDrillSiteDlg();

// 对话框数据
	enum { IDD = IDD_RCU_DRILL_SITE_EDIT_DIALOG };

public:
	//石门图元id
	AcDbObjectId m_drill_site;
	//读取数据到对话框
	void readFromDataLink(DrillSiteLink& ds_link);
	//从对话框中读取数据
	void writeToDataLink(DrillSiteLink& ds_link);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	// 钻场名称
	CString m_name;
	// 钻场位置
	int m_pos;
	// 钻场位置下拉框
	CComboBox m_locaCombBox;
	// 钻场深度
	double m_width;
	// 钻场高度
	double m_height;
	// 钻场与迎头的距离
	double m_dist;
	// 起始编号
	int m_index;
	//孔径
	double m_pore_size;

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
