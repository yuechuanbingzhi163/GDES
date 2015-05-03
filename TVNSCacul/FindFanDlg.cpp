#include "stdafx.h"
//#include "FindFanDlg.h"
////#include "../Sqlite/Sqlite.h"
//
//// FindFanDlg 对话框
//
//IMPLEMENT_DYNAMIC(FindFanDlg, CDialog)
//
//FindFanDlg::FindFanDlg(CWnd* pParent /*=NULL*/)
//	: CDialog(FindFanDlg::IDD, pParent)
//	, m_name(_T(""))
//	, m_minQ(_T(""))
//	, m_maxQ(_T(""))
//	, m_minH(_T(""))
//	, m_maxH(_T(""))
//	, m_power(_T(""))
//{
//
//}
//
//FindFanDlg::~FindFanDlg()
//{
//}
//
//void FindFanDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	DDX_Text(pDX, IDC_FIND_FANNAME_EDIT, m_name);
//	DDX_Text(pDX, IDC_FIND_MINQ_EDIT, m_minQ);
//	DDX_Text(pDX, IDC_FIND_MAXQ_EDIT, m_maxQ);
//	DDX_Text(pDX, IDC_FIND_MINH_EDIT, m_minH);
//	DDX_Text(pDX, IDC_FIND_MAXH_EDIT, m_maxH);
//	DDX_Text(pDX, IDC_FIND_POWER_EDIT, m_power);
//
//	//DDX_Control(pDX, IDC_FIND_FANNAME_EDIT, m_nameEdit);
//	//DDX_Control(pDX, IDC_FIND_MINQ_EDIT, m_minQEdit);
//	//DDX_Control(pDX, IDC_FIND_MAXQ_EDIT, m_maxQEdit);
//	//DDX_Control(pDX, IDC_FIND_MINH_EDIT, m_minHEdit);
//	//DDX_Control(pDX, IDC_FIND_MAXH_EDIT, m_maxHEdit);
//	//DDX_Control(pDX, IDC_FIND_POWER_EDIT, m_powerEdit);
//
//	//DDX_Control(pDX, IDC_BYNAME_CHECK, m_nameBtn);
//	//DDX_Control(pDX, IDC_BYMINQ_CHECK, m_minQBtn);
//	//DDX_Control(pDX, IDC_BYMAXQ_CHECK, m_maxQBtn);
//	//DDX_Control(pDX, IDC_BYMINH_CHECK, m_minHBtn);
//	//DDX_Control(pDX, IDC_BYMAXH_CHECK, m_maxHBtn);
//	//DDX_Control(pDX, IDC_BYPOWER_CHECK, m_powerBtn);
//
//	DDX_Control(pDX, IDC_FIND_METHOD_COMBO, m_methodComBox);
//	DDX_Control(pDX, IDC_FIND_RET_LIST, m_List);
//}
//
//
//BEGIN_MESSAGE_MAP(FindFanDlg, CDialog)
//	ON_BN_CLICKED(IDC_BYNAME_CHECK, &FindFanDlg::OnBnClickedBynameCheck)
//	ON_BN_CLICKED(IDC_BYMINQ_CHECK, &FindFanDlg::OnBnClickedByminqCheck)
//	ON_BN_CLICKED(IDC_BYMAXQ_CHECK, &FindFanDlg::OnBnClickedBymaxqCheck)
//	ON_BN_CLICKED(IDC_BYMINH_CHECK, &FindFanDlg::OnBnClickedByminhCheck)
//	ON_BN_CLICKED(IDC_BYMAXH_CHECK, &FindFanDlg::OnBnClickedBymaxhCheck)
//	ON_BN_CLICKED(IDC_BYALL_CHECK, &FindFanDlg::OnBnClickedByallCheck)
//	ON_BN_CLICKED(IDC_BYPOWER_CHECK, &FindFanDlg::OnBnClickedBypowerCheck)
//	ON_BN_CLICKED(ID_FIND, &FindFanDlg::OnBnClickedFind)
//	ON_NOTIFY(NM_CLICK, IDC_FIND_RET_LIST, &FindFanDlg::OnNMClickFindRetList)
//	ON_CBN_SELCHANGE(IDC_FIND_METHOD_COMBO, &FindFanDlg::OnCbnSelchangeFindMethodCombo)
//	ON_NOTIFY(NM_DBLCLK, IDC_FIND_RET_LIST, &FindFanDlg::OnNMDblclkFindRetList)
//	ON_NOTIFY(NM_KILLFOCUS, IDC_FIND_RET_LIST, &FindFanDlg::OnNMKillfocusFindRetList)
//	ON_BN_CLICKED(IDC_UPDATE_DB_BUTTON, &FindFanDlg::OnBnClickedUpdateDbButton)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_FIND_RET_LIST, &FindFanDlg::OnCustomDraw)
//END_MESSAGE_MAP()
//
//
//// FindFanDlg 消息处理程序
//
//void FindFanDlg::OnBnClickedBynameCheck()
//{
//	if ( BST_CHECKED == IsDlgButtonChecked( IDC_BYNAME_CHECK ) )
//	{
//		m_allBtn->SetCheck(0);
//		//m_allBtn->EnableWindow(FALSE);
//
//		m_nameEdit->EnableWindow(TRUE);
//		GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(TRUE);
//	}
//	else
//	{
//		if ( BST_CHECKED != IsDlgButtonChecked( IDC_BYMINQ_CHECK ) &&
//			 BST_CHECKED != IsDlgButtonChecked( IDC_BYMAXQ_CHECK ) && 
//			 BST_CHECKED != IsDlgButtonChecked( IDC_BYMINH_CHECK ) &&
//			 BST_CHECKED != IsDlgButtonChecked( IDC_BYMAXH_CHECK ) &&
//			 BST_CHECKED != IsDlgButtonChecked( IDC_BYPOWER_CHECK ) &&
//			 BST_CHECKED != IsDlgButtonChecked( IDC_BYALL_CHECK ))
//		{
//			//m_allBtn->EnableWindow(TRUE);
//			GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(FALSE);
//		}
//
//		m_nameEdit->EnableWindow(FALSE);
//	}
//}
//
//void FindFanDlg::OnBnClickedByminqCheck()
//{
//	if ( BST_CHECKED == IsDlgButtonChecked( IDC_BYMINQ_CHECK ) )
//	{
//		m_allBtn->SetCheck(0);
//		//m_allBtn->EnableWindow(FALSE);
//		m_minQEdit->EnableWindow(TRUE);
//		GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(TRUE);
//	}
//	else
//	{
//		m_minQEdit->EnableWindow(FALSE);
//
//		if ( BST_CHECKED != IsDlgButtonChecked( IDC_BYNAME_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYMAXQ_CHECK ) && 
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYMINH_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYMAXH_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYPOWER_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYALL_CHECK ))
//		{
//			//m_allBtn->EnableWindow(TRUE);
//			GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(FALSE);
//		}
//	}
//}
//
//void FindFanDlg::OnBnClickedBymaxqCheck()
//{
//	if ( BST_CHECKED == IsDlgButtonChecked( IDC_BYMAXQ_CHECK ) )
//	{
//		m_allBtn->SetCheck(0);
//		//m_allBtn->EnableWindow(FALSE);
//		m_maxQEdit->EnableWindow(TRUE);
//		GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(TRUE);
//	}
//	else
//	{
//		m_maxQEdit->EnableWindow(FALSE);
//
//		if ( BST_CHECKED != IsDlgButtonChecked( IDC_BYMINQ_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYNAME_CHECK ) && 
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYMINH_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYMAXH_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYPOWER_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYALL_CHECK ))
//		{
//			//m_allBtn->EnableWindow(TRUE);
//			GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(FALSE);
//		}
//	}
//}
//
//void FindFanDlg::OnBnClickedByminhCheck()
//{
//	if ( BST_CHECKED == IsDlgButtonChecked( IDC_BYMINH_CHECK ) )
//	{
//		m_allBtn->SetCheck(0);
//		//m_allBtn->EnableWindow(FALSE);
//
//		m_minHEdit->EnableWindow(TRUE);
//		GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(TRUE);
//	}
//	else
//	{
//		m_minHEdit->EnableWindow(FALSE);
//
//		if ( BST_CHECKED != IsDlgButtonChecked( IDC_BYMINQ_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYMAXQ_CHECK ) && 
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYNAME_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYMAXH_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYPOWER_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYALL_CHECK ))
//		{
//			//m_allBtn->EnableWindow(TRUE);
//			GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(FALSE);
//		}
//	}
//}
//
//void FindFanDlg::OnBnClickedBymaxhCheck()
//{
//	if ( BST_CHECKED == IsDlgButtonChecked( IDC_BYMAXH_CHECK ) )
//	{
//		m_allBtn->SetCheck(0);
//		//m_allBtn->EnableWindow(FALSE);
//
//		m_maxHEdit->EnableWindow(TRUE);
//		GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(TRUE);
//	}
//	else
//	{
//		m_maxHEdit->EnableWindow(FALSE);
//
//		if ( BST_CHECKED != IsDlgButtonChecked( IDC_BYMINQ_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYMAXQ_CHECK ) && 
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYMINH_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYNAME_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYPOWER_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYALL_CHECK ))
//		{
//			//m_allBtn->EnableWindow(TRUE);
//			GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(FALSE);
//		}
//	}
//}
//
//void FindFanDlg::OnBnClickedBypowerCheck()
//{
//	if ( BST_CHECKED == IsDlgButtonChecked( IDC_BYPOWER_CHECK ) )
//	{
//		m_allBtn->SetCheck(0);
//		//m_allBtn->EnableWindow(FALSE);
//
//		m_powerEdit->EnableWindow(TRUE);
//		GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(TRUE);
//	}
//	else
//	{
//		m_powerEdit->EnableWindow(FALSE);
//
//		if ( BST_CHECKED != IsDlgButtonChecked( IDC_BYMINQ_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYMAXQ_CHECK ) && 
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYMINH_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYMAXH_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYNAME_CHECK ) &&
//			BST_CHECKED != IsDlgButtonChecked( IDC_BYALL_CHECK ))
//		{
//			//m_allBtn->EnableWindow(TRUE);
//			GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(FALSE);
//		}
//	}
//}
//
//void FindFanDlg::OnBnClickedByallCheck()
//{
//	//选中
//	if ( BST_CHECKED == IsDlgButtonChecked( IDC_BYALL_CHECK ) )
//	{
//		//其他选项都不能选择
//		m_nameBtn->SetCheck(0);
//		m_nameBtn->EnableWindow(FALSE);
//		m_minQBtn->SetCheck(0);
//		m_minQBtn->EnableWindow(FALSE);
//		m_maxQBtn->SetCheck(0);
//		m_maxQBtn->EnableWindow(FALSE);
//		m_minHBtn->SetCheck(0);
//		m_minHBtn->EnableWindow(FALSE);
//		m_maxHBtn->SetCheck(0);
//		m_maxHBtn->EnableWindow(FALSE);
//		m_powerBtn->SetCheck(0);
//		m_powerBtn->EnableWindow(FALSE);
//
//		GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(FALSE);
//
//		//所有编辑项都不能编辑
//		m_nameEdit->EnableWindow(FALSE);
//		m_minQEdit->EnableWindow(FALSE);
//		m_maxQEdit->EnableWindow(FALSE);
//		m_minHEdit->EnableWindow(FALSE);
//		m_maxHEdit->EnableWindow(FALSE);
//		m_powerEdit->EnableWindow(FALSE);
//	}
//	//未勾选上
//	else
//	{
//		m_nameBtn->EnableWindow(TRUE);
//		m_minQBtn->EnableWindow(TRUE);
//		m_maxQBtn->EnableWindow(TRUE);
//		m_minHBtn->EnableWindow(TRUE);
//		m_maxHBtn->EnableWindow(TRUE);
//		m_powerBtn->EnableWindow(TRUE);
//		intArray cheakedIds = getCheckedIDs();
//		if(0 == cheakedIds.length())
//			GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(FALSE);
//		else
//			GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(TRUE);
//	}
//}
//
//BOOL FindFanDlg::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//	m_nameBtn = (CButton*)GetDlgItem(IDC_BYNAME_CHECK);
//	m_minQBtn = (CButton*)GetDlgItem(IDC_BYMINQ_CHECK);
//	m_maxQBtn = (CButton*)GetDlgItem(IDC_BYMAXQ_CHECK);
//	m_minHBtn = (CButton*)GetDlgItem(IDC_BYMINH_CHECK);
//	m_maxHBtn = (CButton*)GetDlgItem(IDC_BYMAXH_CHECK);
//	m_allBtn = (CButton*)GetDlgItem(IDC_BYALL_CHECK);
//	m_powerBtn = (CButton*)GetDlgItem(IDC_BYPOWER_CHECK);
//	m_allBtn->SetCheck(1);
//
//	m_nameEdit = (CEdit*)GetDlgItem(IDC_FIND_FANNAME_EDIT);
//	m_minQEdit = (CEdit*)GetDlgItem(IDC_FIND_MINQ_EDIT);
//	m_maxQEdit = (CEdit*)GetDlgItem(IDC_FIND_MAXQ_EDIT);
//	m_minHEdit = (CEdit*)GetDlgItem(IDC_FIND_MINH_EDIT);
//	m_maxHEdit = (CEdit*)GetDlgItem(IDC_FIND_MAXH_EDIT);
//	m_powerEdit = (CEdit*)GetDlgItem(IDC_FIND_POWER_EDIT);
//
//	m_methodComBox.AddString(_T("压入式"));
//	m_methodComBox.AddString(_T("抽出式"));
//	m_methodComBox.SetCurSel(0);
//
//	listOnInit();
//	UpdateData(FALSE);
//
//	return TRUE;
//}
//
//void FindFanDlg::setCountFans()
//{
//	int itemNum = m_List.GetItemCount();
//	CString itemNumStr;
//	itemNumStr.Format(_T("%d"),itemNum);
//	GetDlgItem(IDC_ITEMSNUM_EDIT)->SetWindowText(itemNumStr);
//}
//
//void FindFanDlg::OnBnClickedFind()
//{
////	m_List.DeleteAllItems();
////
////	intArray cheakedIDs = getCheckedIDs();
////	CString sql = getFindFansSQL(cheakedIDs);
////	if(sql.IsEmpty())
////	{
////		AfxMessageBox(_T("请给定查询条件!"),MB_OK | MB_ICONINFORMATION );
////		//int itemNum = m_List.GetItemCount();
////		//CString itemNumStr;
////		//itemNumStr.Format(_T("%d"),itemNum);
////		setCountFans();
////		return;
////	}
////	Sqlite sqlite;
////	AcStringArray retArray = sqlite.findFan(sql);
////	if (retArray.isEmpty())
////	{
////		AfxMessageBox(_T("没有找到想要的风机!"),MB_OK | MB_ICONINFORMATION );
////		//int itemNum = m_List.GetItemCount();
////		//CString itemNumStr;
////		//itemNumStr.Format(_T("%d"),itemNum);
////		//GetDlgItem(IDC_ITEMSNUM_EDIT)->SetWindowText(_T(""));
////		setCountFans();
////		return;
////	}
////	//避免刷新数据的时候闪烁
////	m_List.SetRedraw(FALSE);
////	//更新内容
////	CString msg;
////	for (int i = 0; i < retArray.length(); i = i+6)
////	{
////		//if (i%6 == 0 && 0 != i)
////		//{
////		//	msg.Append(_T("\n"));
////		//}
////		//msg.Append(retArray[i]);
////		//msg.Append(_T("\t"));
////		m_List.InsertItem(i/6, retArray[i]);  
////		m_List.SetItemText(i/6, 1, retArray[i + 1]);  
////		m_List.SetItemText(i/6, 2, retArray[i + 2]);  
////		m_List.SetItemText(i/6, 3, retArray[i + 3]);  
////		m_List.SetItemText(i/6, 4, retArray[i + 4]);  
////		m_List.SetItemText(i/6, 5, retArray[i + 5]);  
////	}
////
////	m_List.SetRedraw(TRUE);
////	m_List.Invalidate();
////	m_List.UpdateWindow();
////
////	setCountFans();
////	//UpdateData(FALSE);
//}
////
//intArray FindFanDlg::getCheckedIDs()
//{
////	CWnd   *pWnd; 
////	intArray IDArray;
////	pWnd=GetWindow(GW_CHILD); 
////	while(pWnd!=NULL) 
////	{ 
////		long lzStyle = GetWindowLong(pWnd->m_hWnd,GWL_STYLE);//
////		if ( BS_CHECKBOX == (lzStyle &   BS_CHECKBOX))
////		{
////			int ID = pWnd->GetDlgCtrlID();
////			if ( BST_CHECKED == IsDlgButtonChecked( ID ))
////			{
////				IDArray.append(ID);
////			}
////		}
////
////		pWnd=pWnd-> GetNextWindow(); 
////	}
//	//for(int j = 0; j < IDArray.length(); j++)
//	//{
//	//	CString strID;
//	//	strID.Format(_T("%d"),j);
//	//	CString textByID;
//	//	GetDlgItem(IDArray[j])->GetWindowText(textByID);
//	//	AfxMessageBox(textByID);
//	//}
//
//	return NULL;
//}
//
//void FindFanDlg::listOnInit()
//{
//	//间隔线和行选中
//	m_List.SetExtendedStyle(m_List.GetExtendedStyle() |
//		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
//	
//	//表头
//	m_List.InsertColumn(1,_T("风机型号"),LVCFMT_CENTER);  
//	m_List.InsertColumn(2,_T("最小风量"),LVCFMT_CENTER);
//	m_List.InsertColumn(3,_T("最大风量"),LVCFMT_CENTER);
//	m_List.InsertColumn(4,_T("最小风压"),LVCFMT_CENTER);
//	m_List.InsertColumn(5,_T("最大风压"),LVCFMT_CENTER);
//	m_List.InsertColumn(6,_T("额定功率"),LVCFMT_CENTER);
//
//	CRect rect;  
//	m_List.GetClientRect(rect); //获得当前客户区信息  
//	m_List.SetColumnWidth(0, 2*rect.Width() / 9); //设置列的宽度。  
//	m_List.SetColumnWidth(1, 7*rect.Width() / 45);  
//	m_List.SetColumnWidth(2, 7*rect.Width() / 45);  
//	m_List.SetColumnWidth(3, 7*rect.Width() / 45);  
//	m_List.SetColumnWidth(4, 7*rect.Width() / 45);  
//	m_List.SetColumnWidth(5, 7*rect.Width() / 45);  
//
//	CFont *cFont = new CFont;  
//
//	cFont->CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("宋体")); 
//	m_List.SetFont(cFont,true);//设置字体大小
//	//m_List.SetTextBkColor(RGB(185,193,181));//设置背景颜色
//
//	//m_List.SetTextColor(RGB(80,80,80));//设置字体颜色
//	//COLORREF color = RGB(185,193,181); 
//	//m_List.SetBkColor(color); 
//}
//
//void FindFanDlg::getSelectedValue()
//{
//	int nIdx=m_List.GetNextItem(-1,LVIS_SELECTED);//选中的行的索引
//	m_selItem = nIdx;
//	m_name = m_List.GetItemText(nIdx,0);//选中行的第1项，索引为0
//	m_minQ = m_List.GetItemText(nIdx,1);
//	m_maxQ = m_List.GetItemText(nIdx,2);
//	m_minH = m_List.GetItemText(nIdx,3);
//	m_maxH = m_List.GetItemText(nIdx,4);
//	m_power = m_List.GetItemText(nIdx,5);
//}
//
//void FindFanDlg::OnNMClickFindRetList(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	//CString name,minQ,maxQ,minH,maxH,power;
//	getSelectedValue();
//
//	//GetDlgItem(IDC_FIND_FANNAME_EDIT)->SetWindowText(name);
//	//GetDlgItem(IDC_FIND_MINQ_EDIT)->SetWindowText(minQ);
//	//GetDlgItem(IDC_FIND_MAXQ_EDIT)->SetWindowText(maxQ);
//	//GetDlgItem(IDC_FIND_MINH_EDIT)->SetWindowText(minH);
//	//GetDlgItem(IDC_FIND_MAXH_EDIT)->SetWindowText(maxH);
//	//GetDlgItem(IDC_FIND_POWER_EDIT)->SetWindowText(power);
//	UpdateData(FALSE);
//	GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(FALSE);
//
//	m_allBtn->SetCheck(0);//让查看全部的钩去掉
//	//让其他的都可选
//	m_nameBtn->EnableWindow(TRUE);
//	m_minQBtn->EnableWindow(TRUE);
//	m_maxQBtn->EnableWindow(TRUE);
//	m_minHBtn->EnableWindow(TRUE);
//	m_maxHBtn->EnableWindow(TRUE);
//	m_powerBtn->EnableWindow(TRUE);
//	
//	if(!m_oldValues.isEmpty())
//		m_oldValues.removeAll();
//	m_oldValues.append(m_name);
//	m_oldValues.append(m_minQ);
//	m_oldValues.append(m_maxQ);
//	m_oldValues.append(m_minH);
//	m_oldValues.append(m_maxH);
//	m_oldValues.append(m_power);
//
//	*pResult = 0;
//}
//
//
//void FindFanDlg::OnCbnSelchangeFindMethodCombo()
//{
//	OnBnClickedFind();
//}
//
//void FindFanDlg::OnNMDblclkFindRetList(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	deleteFan();
//	*pResult = 0;
//}
//
//void FindFanDlg::deleteFan()
//{
////	if(IDCANCEL == AfxMessageBox(_T("确定要删除吗?\n(删除之后将无法撤销)"),MB_OKCANCEL | MB_ICONQUESTION))
////		return ;
////	//CString name,minQ,maxQ,minH,maxH,power;
////	getSelectedValue();
////	//CString str = name + _T("\t") + minQ + _T("\t") + maxQ + _T("\t") + minH + _T("\t") + maxH + _T("\t") + power;
////	//AfxMessageBox(str);
////	//return;
////	Sqlite sqlite;
////	CString sql = _T("Name is '") + m_name + \
////		_T("' and MinQ is ") + m_minQ + \
////		_T(" and MaxQ is ") + m_maxQ + \
////		_T(" and MinH is ") + m_minH + \
////		_T(" and MaxH is ") + m_maxH + \
////		_T(" and Power is '") + m_power + _T("'");
////	CString msg;
////	if(sqlite.deleFan(sql)) 
////	{
////		int nIdx=m_List.GetNextItem(-1,LVIS_SELECTED);
////		m_List.DeleteItem(nIdx);
////		msg = _T("删除成功!");
////		setCountFans();
////	}
////	else msg = _T("删除失败!");
////	//if(name.IsEmpty() && minQ.IsEmpty() && maxQ.IsEmpty() && minH.IsEmpty() && maxH.IsEmpty() && power.IsEmpty())
////	//	msg += _T("\n没有选择需要删除的数据");
////	AfxMessageBox(msg);
//}
//void FindFanDlg::OnNMKillfocusFindRetList(NMHDR *pNMHDR, LRESULT *pResult)
//{
////	intArray cheakedIds = getCheckedIDs();
////	if(0 == cheakedIds.length())
////		GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(FALSE);
////	else
////		GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(TRUE);
//
//	*pResult = 0;
//}
//
//void FindFanDlg::OnBnClickedUpdateDbButton()
//{
////	AcStringArray oldValues = m_oldValues;
////	if(oldValues.isEmpty())
////	{
////		AfxMessageBox(_T("没有选择需要修改的风机!"));
////		return;
////	}
////	if(IDCANCEL == AfxMessageBox(_T("确定要修改吗?(修改之后将更新数据库)"),MB_OKCANCEL | MB_ICONQUESTION))
////		return ;
////	AcStringArray newValues;
////	CString method;
////	//CString method,name,minQ,maxQ,minH,maxH,power;
////	m_methodComBox.GetWindowText(method);
////	//GetDlgItem(IDC_FIND_FANNAME_EDIT)->GetWindowText(name);
////	//GetDlgItem(IDC_FIND_MINQ_EDIT)->GetWindowText(minQ);
////	//GetDlgItem(IDC_FIND_MAXQ_EDIT)->GetWindowText(maxQ);
////	//GetDlgItem(IDC_FIND_MINH_EDIT)->GetWindowText(minH);
////	//GetDlgItem(IDC_FIND_MAXH_EDIT)->GetWindowText(maxH);
////	//GetDlgItem(IDC_FIND_POWER_EDIT)->GetWindowText(power);
////	UpdateData(TRUE);
////	newValues.append(m_name);
////	newValues.append(m_minQ);
////	newValues.append(m_maxQ);
////	newValues.append(m_minH);
////	newValues.append(m_maxH);
////	newValues.append(m_power);
////
////	Sqlite sqlite;
////	CString msg;
////	if(_T("压入式") == method) method = _T("PressIn");
////	else method = _T("PullOut");
////	if(sqlite.updateFan(method,oldValues,newValues)) 
////	{
////		msg = _T("数据库更新成功!");
////		UpdateData(TRUE);
////		m_List.SetItemText(m_selItem,0,m_name);
////		m_List.SetItemText(m_selItem,1,m_minQ);
////		m_List.SetItemText(m_selItem,2,m_maxQ);
////		m_List.SetItemText(m_selItem,3,m_minH);
////		m_List.SetItemText(m_selItem,4,m_maxH);
////		m_List.SetItemText(m_selItem,5,m_power);
////
////		//OnBnClickedFind();
////	}
////	else msg = _T("数据库更新失败!");
////	AfxMessageBox(msg);
////	if(!m_oldValues.isEmpty())
////	{
////		m_oldValues.removeAll();
////	}
////	GetDlgItem(IDC_UPDATE_DB_BUTTON)->EnableWindow(FALSE);
////}
////
//CString FindFanDlg::getFindFansSQL(intArray cheakedIDs)
//{
////	CString sql;
////	CString method;
////	GetDlgItem(IDC_FIND_METHOD_COMBO)->GetWindowText(method);
////	if(_T("压入式") == method) method = _T("PressIn");
////	if(_T("抽出式") == method) method = _T("PullOut");
////
////	CString name,minQ,maxQ,minH,maxH,power;
////	int cheakedNum = cheakedIDs.length();
////	if(0 == cheakedNum) return _T("");
////	if ( 1 == cheakedNum && IDC_BYALL_CHECK == cheakedIDs[0])
////	{
////		sql = _T("select * from ") + method;
////		return sql;
////	}
////	for (int i = 0; i < cheakedNum; i++)
////	{
////		switch(cheakedIDs[i])
////		{
////		case IDC_BYNAME_CHECK:
////			GetDlgItem(IDC_FIND_FANNAME_EDIT)->GetWindowText(name);
////			if(name.IsEmpty()) return name;
////			break;
////		case IDC_BYMINQ_CHECK:
////			GetDlgItem(IDC_FIND_MINQ_EDIT)->GetWindowText(minQ);
////			if(minQ.IsEmpty()) return minQ;
////			break;
////		case IDC_BYMAXQ_CHECK:
////			GetDlgItem(IDC_FIND_MAXQ_EDIT)->GetWindowText(maxQ);
////			if(maxQ.IsEmpty()) return maxQ;
////			break;
////		case IDC_BYMINH_CHECK:
////			GetDlgItem(IDC_FIND_MINH_EDIT)->GetWindowText(minH);
////			if(minH.IsEmpty()) return minH;
////			break;
////		case IDC_BYMAXH_CHECK:
////			GetDlgItem(IDC_FIND_MAXH_EDIT)->GetWindowText(maxH);
////			if(maxH.IsEmpty()) return maxH;
////			break;
////		case IDC_BYPOWER_CHECK:
////			GetDlgItem(IDC_FIND_POWER_EDIT)->GetWindowText(power);
////			if(power.IsEmpty()) return power;
////			break;
////		default:
////			break;
////		}
////	}
////
////	if (1 == cheakedNum)
////	{
////		if (!name.IsEmpty())
////		{
////			sql = _T("select * from ") + method + _T(" where Name == '") + name + _T("'"); 
////		}
////		if (!minQ.IsEmpty())
////		{
////			if(-1 == minQ.Find(_T("<")) && -1 == minQ.Find(_T("<=")) && -1 == minQ.Find(_T(">"))&& -1 == minQ.Find(_T(">=")))
////				sql = _T("select * from ") + method + _T(" where MinQ == '") + minQ + _T("'"); 
////			else
////				sql = _T("select * from ") + method + _T(" where MinQ ") + minQ; 
////		}
////		if(!maxQ.IsEmpty())
////		{
////			if(-1 == maxQ.Find(_T("<")) && -1 == maxQ.Find(_T("<=")) && -1 == maxQ.Find(_T(">"))&& -1 == maxQ.Find(_T(">=")))
////				sql = _T( "select * from " ) + method + _T(" where MaxQ == ") + maxQ;
////			else sql = _T( "select * from " ) + method + _T(" where MaxQ ") + maxQ;
////		}
////		if (!minH.IsEmpty())
////		{
////			if(-1 == minH.Find(_T("<")) && -1 == minH.Find(_T("<=")) && -1 == minH.Find(_T(">"))&& -1 == minH.Find(_T(">=")))
////				sql = _T("select * from ") + method + _T(" where MinH == '") + minH + _T("'"); 
////			else
////				sql = _T("select * from ") + method + _T(" where MinH ") + minH; 
////		}
////		if(!maxH.IsEmpty())
////		{
////			if(-1 == maxH.Find(_T("<")) && -1 == maxH.Find(_T("<=")) && -1 == maxH.Find(_T(">"))&& -1 == maxH.Find(_T(">=")))
////				sql = _T( "select * from " ) + method + _T(" where MaxH == ") + maxH;
////			else sql = _T( "select * from " ) + method + _T(" where MaxH ") + maxH;
////		}
////		if (!power.IsEmpty())
////		{
////			sql = _T("select * from ") + method + _T(" where Power == '") + power + _T("'"); 
////		}
////	}
////
////	if (cheakedNum >= 2)
////	{
////		int flag = 0;
////		if (!name.IsEmpty())
////		{
////			flag = 1;
////			sql = _T("select * from ") + method + _T(" where Name == '") + name + _T("'"); 
////		}
////		if (!minQ.IsEmpty())
////		{
////			if(0 == flag)
////			{
////				if( -1 == minQ.Find(_T("<")) && -1 == minQ.Find(_T("<=")) && -1 == minQ.Find(_T(">"))&& -1 == minQ.Find(_T(">=")))
////					sql = _T("select * from ") + method + _T(" where MinQ == '") + minQ + _T("'"); 
////				else
////					sql = _T("select * from ") + method + _T(" where MinQ ") + minQ; 
////			}
////			else
////			{
////				if( -1 == minQ.Find(_T("<")) && -1 == minQ.Find(_T("<=")) && -1 == minQ.Find(_T(">"))&& -1 == minQ.Find(_T(">=")))
////					sql +=  _T(" and MinQ == '") + minQ + _T("'"); 
////				else
////					sql += _T(" and MinQ ") + minQ; 
////
////			}
////			flag = 1;
////		}
////		if(!maxQ.IsEmpty())
////		{
////			if(0 == flag)
////			{
////				if(-1 == maxQ.Find(_T("<")) && -1 == maxQ.Find(_T("<=")) && -1 == maxQ.Find(_T(">"))&& -1 == maxQ.Find(_T(">=")))
////					sql = _T( "select * from " ) + method + _T(" where MaxQ == ") + maxQ;
////				else sql = _T( "select * from " ) + method + _T(" where MaxQ ") + maxQ;
////			}
////			else
////			{
////				if(-1 == maxQ.Find(_T("<")) && -1 == maxQ.Find(_T("<=")) && -1 == maxQ.Find(_T(">"))&& -1 == maxQ.Find(_T(">=")))
////					sql += _T(" and  MaxQ == ") + maxQ;
////				else sql += _T(" and  MaxQ ") + maxQ;
////			}
////			flag = 1;
////		}
////		if (!minH.IsEmpty())
////		{
////			if(0 == flag)
////			{
////				if(-1 == minH.Find(_T("<")) && -1 == minH.Find(_T("<=")) && -1 == minH.Find(_T(">"))&& -1 == minH.Find(_T(">=")))
////					sql = _T("select * from ") + method + _T(" where MinH == '") + minH + _T("'"); 
////				else
////					sql = _T("select * from ") + method + _T(" where MinH ") + minH; 
////			}
////			else
////			{
////				if(-1 == minH.Find(_T("<")) && -1 == minH.Find(_T("<=")) && -1 == minH.Find(_T(">"))&& -1 == minH.Find(_T(">=")))
////					sql +=  _T(" and MinH == '") + minH + _T("'"); 
////				else
////					sql += _T(" and MinH ") + minH; 
////			}
////			flag = 1;
////		}
////		if(!maxH.IsEmpty())
////		{
////			if(0 == flag)
////			{
////				if(-1 == maxH.Find(_T("<")) && -1 == maxH.Find(_T("<=")) && -1 == maxH.Find(_T(">"))&& -1 == maxH.Find(_T(">=")))
////					sql = _T( "select * from " ) + method + _T(" where MaxH == ") + maxH;
////				else sql = _T( "select * from " ) + method + _T(" where MaxH ") + maxH;
////			}
////			else
////			{
////				if(-1 == maxH.Find(_T("<")) && -1 == maxH.Find(_T("<=")) && -1 == maxH.Find(_T(">"))&& -1 == maxH.Find(_T(">=")))
////					sql += _T(" and MaxH == ") + maxH;
////				else sql += _T(" and MaxH ") + maxH;
////
////			}
////		}
////		if (!power.IsEmpty())
////		{
////			sql += _T(" and Power == '") + power + _T("'"); 
////		}
////	}
////	return sql;
//}
////
//void FindFanDlg::OnCustomDraw( NMHDR *pNMHDR, LRESULT *pResult )
//{
////	// Contains information specific to an NM_CUSTOMDRAW 
////	// notification message sent by a list-view control. 
////	// mean:draw each item.set txt color,bkcolor.... 
////	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)(pNMHDR);
////
////	// Take the default processing unless we set this to something else below. 
////	*pResult = CDRF_NEWFONT;
////
////	// First thing - check the draw stage. If it's the control's prepaint 
////	// stage, then tell Windows we want messages for every item. 
////	if ( pLVCD->nmcd.dwDrawStage==CDDS_PREPAINT) 
////	{ 
////		*pResult = CDRF_NOTIFYITEMDRAW; 
////	} 
////	// This is the notification message for an item.   We'll request 
////	// notifications before each subitem's prepaint stage. 
////	else if ( pLVCD->nmcd.dwDrawStage==CDDS_ITEMPREPAINT ) 
////	{ 
////		int nItem = pLVCD->nmcd.dwItemSpec;
////		// 判断使ListCtrl不同颜色现实的条件		
////		if (m_List.GetItemState(nItem,LVIS_FOCUSED) == LVIS_FOCUSED) 
////		{ 
////			pLVCD->clrText =  RGB(255, 0, 0);
////		} 
////
////		*pResult = CDRF_DODEFAULT; 
////	} 
//}