#pragma once
#include "Resource.h"
#include "../ARX_ReportHelper/GasBaseAcesDlg.h"
#include "afxwin.h"

class RealGasQDlg : public GasBaseAcesDlg
{
	DECLARE_DYNAMIC(RealGasQDlg)

public:
	RealGasQDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RealGasQDlg();

// 对话框数据
	enum { IDD = IDD_YEAR_GASQ_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

private:
	// 矿井上年度实际抽采瓦斯量
	CString m_lastGas;
	// 预开采区域瓦斯含量最大煤层应抽瓦斯吨煤含量
	CString m_maxGas;
	// 矿井总回风巷瓦斯浓度
	CString m_gasCon;
	// 矿井最大总回风风量
	CString m_maxQ;
	// 矿井超前抽采系数
	CString m_k;
	// 计算结果
	CString m_yearRet;
};
