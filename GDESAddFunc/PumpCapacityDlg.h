#pragma once
#include "Resource.h"
#include "../ARX_ReportHelper/GasBaseAcesDlg.h"
#include "CGridListCtrlEx/CGridListCtrlGroups.h"
#include "GasCapacity_DataModel.h"

#include "afxcmn.h"

// PumpCapacityDlg 对话框

class PumpCapacityDlg : public GasBaseAcesDlg
{
	DECLARE_DYNAMIC(PumpCapacityDlg)

public:
	PumpCapacityDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PumpCapacityDlg();

// 对话框数据
	enum { IDD = IDD_PUMP_ABILITY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	// 矿井相对瓦斯涌出量
	CString m_absGas;
	// 总回风巷瓦斯浓度
	CString m_gasConcentration;
	// 矿井最大总回风风量
	CString m_maxQ;
	// 当地大气压
	CString m_localP;
	// 抽采系统工况系数
	CString m_workCondiction;
	// 瓦斯抽采泵台数
	CString m_numPump;
	// 计算结果
	CString m_pumpRet;
	CString m_sysRet;

	CGridListCtrlGroups m_pumpListCtrl;
	GasCapacity_DataModel m_dataModel;
	vector<GasCapacity_DataRecord> m_records;

public:
	afx_msg void OnBnClickedOk();

private:
	void OnInitList();
public:
	afx_msg void OnEnKillfocusPumpNumEdit();
};
