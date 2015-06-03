#pragma once
#include "Resource.h"
#include "../ARX_ReportHelper/GasBaseAcesDlg.h"

class RateGasDlg : public GasBaseAcesDlg
{
	DECLARE_DYNAMIC(RateGasDlg)

public:
	RateGasDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RateGasDlg();

// 对话框数据
	enum { IDD = IDD_GAS_RATE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	// 矿井相对瓦斯涌出量
	CString m_relativeGas;
	// 矿井绝对瓦斯涌出量
	CString m_absGas;
	CString m_ret;
public:
	afx_msg void OnBnClickedOk();
};
