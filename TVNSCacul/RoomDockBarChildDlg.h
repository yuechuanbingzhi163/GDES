#pragma once

#include "PropertyData_DockBarChildDlg.h"

class RoomDockBarChildDlg : public PropertyData_DockBarChildDlg
{
	DECLARE_DYNAMIC(RoomDockBarChildDlg)

public:
	RoomDockBarChildDlg(CWnd *pParent =NULL, HINSTANCE hInstance =NULL);   // 标准构造函数
	virtual ~RoomDockBarChildDlg();

// 对话框数据
	enum { IDD = IDD_ROOM_PARAM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CString m_name;
	CString m_area;
	CString m_velocity;
	CString m_q;
	CString m_temp;
	int m_cm;

protected:
	void readPropertyData();
	void writePropertyData();

private:
	// 读写超温控制措施
	void readControlMeasure();	
	void writeControlMeasure();

public:
	virtual BOOL OnInitDialog();
};
