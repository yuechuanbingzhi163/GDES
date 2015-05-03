#pragma once
#include "ResultDlg.h"

// EPCMFDDlg 对话框

class EPCMFDDlg : public GasBaseAcesDlg
{
	DECLARE_DYNAMIC(EPCMFDDlg)

public:
	EPCMFDDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~EPCMFDDlg();

// 对话框数据
	enum { IDD = IDD_EPCMFD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	//抽采达标工艺方案设计
	BOOL m_DDSP;
	// 为抽采达标服务的各项工程布局、工程量
	BOOL m_DCSEL;
	// 进度、资金计划、连续关系
	BOOL m_SFCR;
	// 施工设备、主要器材
	BOOL m_CEME;
	// 采掘工作面施工设计
	BOOL m_MFCD;
	// 钻孔参数表、施工要求、钻孔工程量
	BOOL m_DPCRD;
	// 施工设备与进度计划
	BOOL m_CES;
	// 预期效果及组织管理
	BOOL m_EROM;
	// 有效抽瓦斯时间
	BOOL m_ETPG;
	// 安全技术设施
	BOOL m_SM;
	// 抽放钻孔布置图
	BOOL m_BL;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDdspCheck();
	afx_msg void OnBnClickedMfcdCheck();
private:
	void SetEnableDDSP(BOOL isTrue);
	void SetEnableMFCD(BOOL isTrue);
};
