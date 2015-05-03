#pragma once
#include "GasBaseAcesDlg.h"
#include "afxcmn.h"
#include "CGridListCtrlGroups.h"
#include "afxwin.h"

class ResultDlg : public GasBaseAcesDlg
{
	DECLARE_DYNAMIC(ResultDlg)

public:
	ResultDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ResultDlg();

// 对话框数据
	enum { IDD = IDD_RESULT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	CGridListCtrlGroups m_resultCtrlList;

	AcStringArray m_datas;		//结论数据
	IntArray m_retTrueIndxs;	//评判结果是正确的表行号
	CComboBox m_methodCombBox;	//评判方式的选择
	BOOL m_isTrue;			//评判结果的是否正确
	CString m_objectName;	//评判结果选择的词典名
	CString m_descObjectName;	//选择其他时描述对话框数据保存名
	AcStringArray m_bookMks;
	BOOL m_isOthers;			//判断是否选中其他选项
	CString m_reasonsDesc;		//自述内容
public:
	void setItemDatas(const AcStringArray& datas);
	void setItems();
	void setTrueIndex(const IntArray& trueIndexs,const CString& txtSaveName,const CString& descObjectName);
	void setBookMarks(const AcStringArray& bookMks);

private:
	void InitListCtrl();
	void writeDatasToFile(CString& strConlusion,const CString& strMethod);
	void showReasonsDescDlg(int nIndex);

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMClickResultList(NMHDR *pNMHDR, LRESULT *pResult);
};
