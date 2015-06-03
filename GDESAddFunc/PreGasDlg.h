#pragma once
#include "Resource.h"
#include "../ARX_ReportHelper/GasBaseAcesDlg.h"
#include "CGridListCtrlEx/CGridListCtrlGroups.h"
#include "PreGas_DataModel.h"

#include "afxcmn.h"

class PreGasDlg : public GasBaseAcesDlg
{
	DECLARE_DYNAMIC(PreGasDlg)

public:
	PreGasDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PreGasDlg();

// 对话框数据
	enum { IDD = IDD_OUTPRE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	// 核定区域内煤层的最大瓦斯含量
	CString m_preMaxGas;
	// 抽采后满足防突要求的残余瓦斯含量
	CString m_leafGas;
	// 矿井回采率
	CString m_mineReRate;
	// 矿井年实际预抽瓦斯量
	CString m_forGas;
	// 邻近层和围岩瓦斯储量系数
	CString m_gasY;
	// 矿井掘进出煤系数
	CString m_TTJ;
	// 工作面数
	CString m_faceNum;

	CString m_ret;

	CGridListCtrlGroups m_mineFaceList;
	PreGas_DataModel m_dataModel;
	vector<PreGas_DataRecord> m_records;

private:
	void OnInitList();

public:
	afx_msg void OnEnKillfocusFaceNumEdit();
	afx_msg void OnBnClickedOk();
};
