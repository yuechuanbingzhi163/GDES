// RcuAddDrillDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RcuAddDrillDlg.h"


// RcuAddDrillDlg 对话框

IMPLEMENT_DYNAMIC(RcuAddDrillDlg, RcuAcUiBaseDlg)

RcuAddDrillDlg::RcuAddDrillDlg(CWnd* pParent /*=NULL*/)
	: RcuAcUiBaseDlg(RcuAddDrillDlg::IDD, pParent)
{

}

RcuAddDrillDlg::~RcuAddDrillDlg()
{
}

void RcuAddDrillDlg::DoDataExchange(CDataExchange* pDX)
{
	RcuAcUiBaseDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RcuAddDrillDlg, RcuAcUiBaseDlg)
END_MESSAGE_MAP()


// RcuAddDrillDlg 消息处理程序
