#pragma once

#include "acui.h"
#include "resource.h"

class GCPDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(GCPDlg)

public:
	GCPDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GCPDlg();

// 对话框数据
	enum { IDD = IDD_GCP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CAcUiPickButton m_pickBtn;
	double m_xPos;
	double m_yPos;
	double m_zPos;
	double m_gas;
	double m_level;
	double m_height;

	void readPropertyData();
	void writePropertyData();
	AcDbObjectId createObject();

public:
	AcDbObjectId m_objId;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedGcpPtBtn();
};
