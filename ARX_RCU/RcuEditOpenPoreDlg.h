#pragma once

#include "RcuAcUiBaseDlg.h"
#include "RcuDataLink.h"


class RcuEditOpenPoreDlg : public RcuAcUiBaseDlg
{
	DECLARE_DYNAMIC(RcuEditOpenPoreDlg)

public:
	RcuEditOpenPoreDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RcuEditOpenPoreDlg();

// 对话框数据
	enum { IDD = IDD_RCU_OPEN_PORES_DLG };

public:
	//外部设置标题
	CString m_title;
	//外部设置钻场位置文本
	CString m_pos;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_pore_num;
	double m_pore_gap;
	double m_pore_size;

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
