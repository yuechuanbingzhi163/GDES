#pragma once

#include "Resource.h"
#include "afxwin.h"

class BlockDraw_ConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(BlockDraw_ConfigDlg)

public:
	BlockDraw_ConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~BlockDraw_ConfigDlg();

// 对话框数据
	enum { IDD = IDD_BLOCK_DRAW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

private:
	void fillBlockList();
	CString getSelBlock();

public:
	CComboBox m_blockList;
	CString m_blockName;

};
