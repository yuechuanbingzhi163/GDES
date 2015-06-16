#pragma once
#include "RcuAcUiBaseDlg.h"
#include "RcuDataLink.h"
#include "afxwin.h"

//新增钻场对话框
class RcuAddDrillDlg : public RcuAcUiBaseDlg
{
	DECLARE_DYNAMIC(RcuAddDrillDlg)

public:
	RcuAddDrillDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RcuAddDrillDlg();

// 对话框数据
	enum { IDD = IDD_RCU_DRILL_NEW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	// 钻场名称
	CString m_name;
	// 钻场位置
	int m_leftOrRight;
	// 钻场位置下拉框
	CComboBox m_locaCombBox;
	// 钻场宽度
	double m_width;
	// 钻场高度
	double m_height;
	// 钻场与迎头的距离
	double m_dist;
	// 起始编号
	int m_index;

public:
	//从对话框中读取数据
	void writeToDataLink(DrillSiteLink& ds_link);

public:
	afx_msg void OnBnClickedOk();
};
