#pragma once

#include "Resource.h"

class EditPumpDB : public CDialog
{
	DECLARE_DYNAMIC(EditPumpDB)

public:
	EditPumpDB(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~EditPumpDB();

// 对话框数据
	enum { IDD = IDD_FIND_PUMP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_btype;
	BOOL m_bweight;
	BOOL m_bspeed;
	BOOL m_bmaxp;
	BOOL m_bfactory;
	BOOL m_blength;
	BOOL m_bheight;
	BOOL m_bweidth;
	BOOL m_bminabsp;
	BOOL m_babsp;
	BOOL m_bpower;
	BOOL m_bmaxq;
	BOOL m_ball;
	CString m_type;
	CString m_weight;
	CString m_speed;
	CString m_maxp;
	CString m_factory;
	CString m_length;
	CString m_height;
	CString m_weidth;
	CString m_absp;
	CString m_power;
	CString m_maxq;
	CString m_minabsp;
	CString m_itemsNum;
};
