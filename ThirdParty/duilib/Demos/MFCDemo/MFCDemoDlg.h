// MFCDemoDlg.h : 头文件
//

#pragma once

class ComputerExamineUI : public CContainerUI
{
public:
	ComputerExamineUI(CPaintManagerUI& paintManager)
	{
		CDialogBuilder builder;
		CContainerUI* pComputerExamine = static_cast<CContainerUI*>(builder.Create(_T("ComputerExamine.xml"), (UINT)0, 0, &paintManager));
		if( pComputerExamine ) {
			this->Add(pComputerExamine);
		}
		else {
			this->RemoveAll();
		}
	}
};

class CDialogUI:public WindowImplBase
{
public:
	CDialogUI(){};
	~CDialogUI(){};
	LPCTSTR GetWindowClassName() const { return _T("UIDialog"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	virtual CDuiString GetSkinFile() { return _T("skin.xml"); }
	virtual CDuiString GetSkinFolder() { return _T("../skin/360SafeRes"); }

	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if( _tcscmp(pstrClass, _T("ComputerExamine")) == 0 ) return new ComputerExamineUI(m_PaintManager);
		return NULL;
	}
	//处理控件消息
	void Notify(TNotifyUI& msg)
	{
		if( msg.sType == _T("click") ) {
			if( msg.pSender->GetName() == _T("closebtn") ) {
				PostQuitMessage(0);
				return; 
			}
			else if( msg.pSender->GetName() == _T("minbtn") ) { 
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
			else if( msg.pSender->GetName() == _T("maxbtn") ) { 
				SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
			else if( msg.pSender->GetName() == _T("restorebtn") ) { 
				SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
		}
		else if(msg.sType==_T("setfocus"))
		{
			CDuiString name = msg.pSender->GetName();
			CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("switch")));
			if(name==_T("examine"))
				pControl->SelectItem(0);
			else if(name==_T("trojan"))
				pControl->SelectItem(1);
			else if(name==_T("plugins"))
				pControl->SelectItem(2);
			else if(name==_T("vulnerability"))
				pControl->SelectItem(3);
			else if(name==_T("rubbish"))
				pControl->SelectItem(4);
			else if(name==_T("cleanup"))
				pControl->SelectItem(5);
			else if(name==_T("fix"))
				pControl->SelectItem(6);
			else if(name==_T("tool"))
				pControl->SelectItem(7);
		}
	}
};

// CMFCDemoDlg 对话框
class CMFCDemoDlg : public CDialog
{
// 构造
public:
	CMFCDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CDialogUI m_dlgWnd;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
