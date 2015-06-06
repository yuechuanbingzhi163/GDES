#pragma once

#include "acui.h"
#include "resource.h"

class WDCI2Dlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(WDCI2Dlg)

public:
	WDCI2Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~WDCI2Dlg();

// 对话框数据
	enum { IDD = IDD_WDCI2_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CAcUiPickButton m_pickBtn;
	double m_xPos;
	double m_yPos;
	double m_zPos;
	double m_velocity;
	double m_smax;
	double m_r;

	void readPropertyData();
	void writePropertyData();
	AcDbObjectId createObject();

public:
	AcDbObjectId m_objId;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedWdci1PtBtn();
};
