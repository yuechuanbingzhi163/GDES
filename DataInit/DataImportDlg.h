#pragma once

#include "Resource.h"

class DataImportDlg : public CDialog
{
	DECLARE_DYNAMIC(DataImportDlg)

public:
	DataImportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DataImportDlg();

// 对话框数据
	enum { IDD = IDD_DATA_IMPORT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	void enableControl(long id, const CString& text, bool bEnable);
public:
	afx_msg void OnBnClickedDataImpotFieldBtn();
	afx_msg void OnBnClickedDataImpotStringListBtn();
	afx_msg void OnBnClickedDataImportIntListBtn();
	afx_msg void OnBnClickedDataImpotObjectListBtn();
	afx_msg void OnBnClickedDataImportGlobalInfoBtn();
};
