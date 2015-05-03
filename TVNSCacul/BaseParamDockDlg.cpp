// BaseParamDockDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BaseParamDockDlg.h"
//#include "commCtrl.h"
#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"
#include "../MineGE/PropertyDataUpdater.h"
#include "../MineGE/FuncFieldHelper.h"
#include "../MineGE/DataHelper.h"
#include "../MineGE/FieldHelper.h"

IMPLEMENT_DYNAMIC(BaseParamDockDlg, CAcUiDialog)

BaseParamDockDlg::BaseParamDockDlg(CWnd* pParent /*=NULL*/)
: CAcUiDialog(BaseParamDockDlg::IDD, pParent)
{

}

BaseParamDockDlg::~BaseParamDockDlg()
{
}

void BaseParamDockDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BaseParamDockDlg, CAcUiDialog)
	ON_REGISTERED_MESSAGE( AFX_WM_PROPERTY_CHANGED, OnPropertyChanged )
END_MESSAGE_MAP()

void BaseParamDockDlg::readPropertyData()
{
	// 获取类型名称
	CString type;
	DataHelper::GetTypeName( m_objId, type );
	//acutPrintf(_T("\n类型:%s"),type);

	AcStringArray funcs,funcFieldsInfo;
	FuncFieldHelper::GetFunctions(funcs);
	int funcNum = funcs.length();
	for (int i = 0; i < funcNum; i++)
	{
		AcStringArray fields;
		if(!FuncFieldHelper::GetFields(funcs[i].kACharPtr(),type,fields)) return;
		if(fields.isEmpty()) continue;
		//acutPrintf(_T("\n功能:%s"),funcs[i].kACharPtr());
		funcFieldsInfo.append(_T("$"));
		funcFieldsInfo.append(funcs[i]);
		for(int j = 0; j < fields.length(); j++)
		{
			funcFieldsInfo.append(fields[j]);
			//acutPrintf(_T("\n字段名称:%s"),fields[j].kACharPtr());
		}
	}

	// 创建属性数据控件
	PropertyDataUpdater::BuildPropGridCtrl( &m_propertyDataList, type,funcFieldsInfo );

	// 填充属性数据
	bool ret = PropertyDataUpdater::ReadDataFromGE( &m_propertyDataList, m_objId );
	
	 //填充数据失败
	if( !ret )
	{
		//acutPrintf(_T("\n失败~~~~~~~~~"));
		acutPrintf(_T("\n填充数据失败(ReadDataFromGE)"));
		m_propertyDataList.EnableWindow( FALSE );
		m_propertyDataList.ShowWindow( SW_HIDE );
	}
	//UpdateData(FALSE);
	m_propertyDataList.ExpandAll(FALSE);

}

void BaseParamDockDlg::writePropertyData()
{
	bool ret = PropertyDataUpdater::WriteDataToGE( &m_propertyDataList, m_objId );
	int pPropCount = m_propertyDataList.GetPropertyCount();
	//acutPrintf(_T("\n数据保存%s"),ret?_T("成功"):_T("失败"));
}

BOOL BaseParamDockDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	if(!SingleDataObjectHelper::GetObjectId(_T("计算参数"),m_objId)) return FALSE;
	//创建, 定位, 显示CMFCPropertyGridCtrl
	CRect rect;
	GetDlgItem( IDC_BASE_PARAM_GRID )->GetWindowRect( &rect );
	ScreenToClient( &rect );
	m_propertyDataList.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rect, this, ( UINT ) - 1 );
	//让bool值变成“是/否”
	//m_propertyDataList.SetBoolLabels(_T("是"),_T("否"));
	updateProperty(false);
	return TRUE;
}

LRESULT BaseParamDockDlg::OnPropertyChanged( WPARAM,LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;
	//int i = (int)pProp->GetData();
	CString filedName = pProp->GetName();
	COleVariant changedValue = pProp->GetValue();
	//t?=?pProp->GetOriginalValue();??//改变之前的值??
	int groupCount = m_propertyDataList.GetPropertyCount();
	for( int i = 0; i < groupCount; i++)
	{
		CMFCPropertyGridProperty* pGroup = m_propertyDataList.GetProperty(i);
		int propCount = pGroup->GetSubItemsCount();
		for( int j = 0; j < propCount; j++ )
		{
			CMFCPropertyGridProperty* pPro = pGroup->GetSubItem(j);
			if (pPro->GetName() == filedName && pPro != pProp)
			{
				pPro->SetValue(changedValue);
			}
		}
	}
	updateProperty(true);
	return 0;
}

void BaseParamDockDlg::onClosing()
{
	updateProperty(true);
}

void BaseParamDockDlg::updateProperty(bool writeOrRead)
{
	acDocManager->lockDocument( curDoc() );
	if(writeOrRead)
	{
		UpdateData(TRUE);
		// 保存数据
		writePropertyData(  );
	}
	else
	{
		// 读取数据
		readPropertyData(  );
		UpdateData(FALSE);
	}
	acDocManager->unlockDocument( curDoc() );
}
