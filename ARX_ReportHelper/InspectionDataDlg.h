#pragma once
#include "ResultDlg.h"

class InspectionDataDlg : public GasBaseAcesDlg
{
	DECLARE_DYNAMIC(InspectionDataDlg)

public:
	InspectionDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~InspectionDataDlg();

// 对话框数据
	enum { IDD = IDD_INSPECTION_DATA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDwbCheck();
	afx_msg void OnBnClickedOk();

private:
	// 抽采工程竣工图
	BOOL m_DWB;
	// 工程开工时间
	BOOL m_PST;
	// 竣工时间
	BOOL m_CT;
	// 异常现象记录
	BOOL m_AR;
	// 其他验收资料
	BOOL m_OID;

private:
	void SetEnableDWB(BOOL isTrue);

};
