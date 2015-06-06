#pragma once

#include "acui.h"
#include "resource.h"

class CSDCIDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CSDCIDlg)

public:
	CSDCIDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSDCIDlg();

// 对话框数据
	enum { IDD = IDD_CSDCI_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CAcUiPickButton m_pickBtn;
	double m_xPos;
	double m_yPos;
	double m_zPos;
	double m_velocity;
	double m_coeff;

	void readPropertyData();
	void writePropertyData();
	AcDbObjectId createObject();

public:
	AcDbObjectId m_objId;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCsdciPtBtn();
};
