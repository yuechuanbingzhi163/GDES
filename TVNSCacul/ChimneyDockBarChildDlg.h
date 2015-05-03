#pragma once

#include "PropertyData_DockBarChildDlg.h"
#include "afxwin.h"

class ChimneyDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(ChimneyDockBarChildDlg)

public:
	ChimneyDockBarChildDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ChimneyDockBarChildDlg();

// 对话框数据
	enum { IDD = IDD_CHIMNEY_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_name;
	CString m_lenth;
	CString m_diameter;
	CString m_bend;
	CComboBox m_material;
	CString m_joints;
	CString m_friction;

protected:
	void readPropertyData();
	void writePropertyData();

	void readAlpha();
	void initDatas();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusChimneyDiameterEdit();
	afx_msg void OnCbnSelchangeChimneyMaterialCombo();
private:
	CString m_hmWindage;
public:
	afx_msg void OnEnKillfocusChimneyFrictionEdit();
	afx_msg void OnEnSetfocusChimneyFrictionEdit();
};
