// AddDrillDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AddDrillDlg.h"


// AddDrillDlg 对话框

IMPLEMENT_DYNAMIC(AddDrillDlg, AcUiBaseDlg)

AddDrillDlg::AddDrillDlg(CWnd* pParent /*=NULL*/)
	: AcUiBaseDlg(AddDrillDlg::IDD, pParent)
{

}

AddDrillDlg::~AddDrillDlg()
{
}

void AddDrillDlg::DoDataExchange(CDataExchange* pDX)
{
	AcUiBaseDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AddDrillDlg, AcUiBaseDlg)
END_MESSAGE_MAP()


// AddDrillDlg 消息处理程序
