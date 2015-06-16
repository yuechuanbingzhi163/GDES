#pragma once

#include "acui.h"
#include "resource.h"

class AcUiBaseDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(AcUiBaseDlg)

public:
	AcUiBaseDlg(UINT idd,CWnd* pParent = NULL,HINSTANCE hDialogResource=NULL);   // 标准构造函数
	virtual ~AcUiBaseDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	HICON m_hIcon;
};
