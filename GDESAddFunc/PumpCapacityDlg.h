#pragma once
#include "Resource.h"
#include "../ARX_ReportHelper/GasBaseAcesDlg.h"
#include "CGridListCtrlEx/CGridListCtrlGroups.h"
#include "CListCtrl_DataModel.h"

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
	// 抽采泵富余系数
	CString m_surplus;
	// 抽采系统工况系数
	CString m_workCondiction;
	// 瓦斯抽采泵台数
	CString m_numPump;
	// 计算结果
	CString m_ret;

	CGridListCtrlGroups m_pumpListCtrl;
	CListCtrl_DataModel m_dataModel;
	vector<CListCtrl_DataRecord> m_records;

public:
	afx_msg void OnBnClickedOk();

private:
	void OnInitList();
	bool Caculate(const AcStringArray& baseData,const AcStringArray& pumpData,CString& strRet);
public:
	afx_msg void OnEnKillfocusPumpNumEdit();
};
