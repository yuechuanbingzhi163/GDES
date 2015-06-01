#pragma once
//#include "Resource.h"
//#include "CListCtrl_DataModel.h"
//#include "config.h"
#include "ReportDataHelper.h"
#include "dlimexp.h"

#include <iostream>
#include <fstream>
using namespace std;
#include "AcFStream.h"

//基础达标评价对话框基类
//实现的是图标的显示和读取文件函数
class ARX_REPORTHELPER_DLLIMPEXP GasBaseAcesDlg : public CDialog
{
	DECLARE_DYNAMIC(GasBaseAcesDlg)

public:
	GasBaseAcesDlg(UINT nIDTemplate,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GasBaseAcesDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	//virtual BOOL readDataFromFile(const CString& txtName, AcStringArray& datasArray);
	//virtual BOOL WriteDataToFile(const CString& txtName,const AcStringArray& nameArray,const AcStringArray& datasArray);
	
	CString GetAppPathDir();
	CString BuildPath( const CString& dir, const CString& fileName );

	DECLARE_MESSAGE_MAP()

protected:
	HICON m_hIcon;
};
