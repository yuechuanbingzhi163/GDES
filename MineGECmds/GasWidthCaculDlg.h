#pragma once
#include "afxwin.h"
#include "Resource.h"
#include "ValueHelper.h"

//#include<iostream>
//#include<vector>  
//using namespace std;
// GasWidthCaculDlg 对话框

enum coalQuality
{
	RICH_COAL		= 0,		//肥煤
	CHARRED_COAL	= 1,		//焦煤
	GAS_COAL		= 2,		//气煤
	LEAN_COAL		= 3,		//瘦煤
	BLIND_COAL		= 4,		//无烟煤
	LONGFLAME_COAL	= 5		//长焰煤
};

class GasWidthCaculDlg : public CDialog
{
	DECLARE_DYNAMIC(GasWidthCaculDlg)

public:
	GasWidthCaculDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GasWidthCaculDlg();

// 对话框数据
	enum { IDD = IDD_TENDENCY_TREND_WIDTH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	CComboBox m_coalQualComb;
	CString m_H1;
	CString m_H2;
	CString m_h1;
	CString m_h2;

	AcDbObjectId m_objId;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	void setMineGE( const AcDbObjectId& objId );

private:
	//double newTonDivided(int n,vector<double>&X,vector<double>&Y);
	//double getRetByNewTon(double x,vector<double>&X,vector<double> &Y);
	double getResult( double dTime );
	void WriteData();
	void ReadData();
};
