#pragma once
#include "ResultDlg.h"

// SelfManageDlg 对话框

class SelfManageDlg : public GasBaseAcesDlg
{
	DECLARE_DYNAMIC(SelfManageDlg)

public:
	SelfManageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SelfManageDlg();

// 对话框数据
	enum { IDD = IDD_SELFMANAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
	// 瓦斯抽采达标评价工作体系
	BOOL m_GDES;
	// 瓦斯抽采达标评判细则
	BOOL m_GDJR;
	// 瓦斯抽采管理和考核奖惩制度
	BOOL m_GDMAR;
	// 抽采工程检查验收制度
	BOOL m_DPIA;
	// 先抽后采例会制度
	BOOL m_AFDRM;
	// 技术档案管理制度
	BOOL m_TFMS;
};
