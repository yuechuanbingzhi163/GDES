#pragma once

#include "ResultDlg.h"

class GasPlanDlg : public GasBaseAcesDlg
{
	DECLARE_DYNAMIC(GasPlanDlg)

public:
	GasPlanDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GasPlanDlg();

// 对话框数据
	enum { IDD = IDD_YEARPLAN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();


private:
	HICON m_hIcon;
	//抽采达标范围内规划产量
	BOOL m_range;
	// 抽采达标工程
	BOOL m_SEE;
	// 抽采设施设备
	BOOL m_EEF;
	// 资金计划
	BOOL m_CP;
	// 达标规划采面交替
	BOOL m_CMFA;
	// 达标规划巷道掘进
	BOOL m_CRE;
	// 抽采量
	BOOL m_DEQ;
	// 年度瓦斯抽采达标的煤层范围及相应的年度产量安排
	BOOL m_APS;
	// 施工队伍、抽采时间、抽采量
	BOOL m_TTV;
	// 抽采指标、资金计划
	BOOL m_DFP;
	// 年度抽采工程
	BOOL m_AEP;
	// 年度抽采设备设施
	BOOL m_YEEF;
	// 年度采面交替
	BOOL m_YMFA;
	// 年度巷道掘进
	BOOL m_YRE;
};
