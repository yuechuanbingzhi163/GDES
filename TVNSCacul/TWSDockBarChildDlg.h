#pragma once
#include "PropertyData_DockBarChildDlg.h"

class TWSDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(TWSDockBarChildDlg)

public:
	TWSDockBarChildDlg(CWnd *pParent =NULL, HINSTANCE hInstance =NULL);   // 标准构造函数
	virtual ~TWSDockBarChildDlg();

// 对话框数据
	enum { IDD = IDD_TWS_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CString m_name;
	CString m_area;
	CString m_velocity;
	CString m_q;
	CString m_temp;
	int m_cm;
	CComboBox m_combo;

protected:
	void readPropertyData();
	void writePropertyData();

private:
	void readControlMeasure();
	void writeControlMeasure();
	void readMaterial();
	void writeMaterial();

public:
	virtual BOOL OnInitDialog();
};
