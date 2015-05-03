#include "StdAfx.h"
#include "UIHelper.h"

#include "DrawManagerDlg.h"
#include "FieldManagerDlg.h"
#include "DataObjectStateDlg.h"
#include "VarListDlg.h"

#include "PropertyDataDlgHelper.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/DataListHelper.h"

void UIHelper::ShowDrawManagerDlg()
{
    // 切换资源
    CAcModuleResourceOverride myResources;

    DrawManagerDlg dmd;
    dmd.DoModal(); // 可视化效果切换和配置
}

void UIHelper::ShowFieldManagerDlg()
{
    // 切换资源
    CAcModuleResourceOverride myResources;

    FieldManagerDlg fmd;
    fmd.DoModal(); // 属性数据字段管理
}

//void UIHelper::ShowPropertyDataDlg()
//{
//
//}

void UIHelper::ShowVarListDlg()
{
    CAcModuleResourceOverride myResources;

    VarListDlg vld;
    vld.DoModal();
}

void UIHelper::ShowDataObjectStateDlg()
{
    CAcModuleResourceOverride myResources;

    DataObjectStateDlg dosd;
    dosd.DoModal();
}

void UIHelper::DisplayData()
{
    // 显示图元的属性数据
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个图元:" ) );
    if( objId.isNull() ) return;
	//if( ArxUtilHelper::IsEqualType( _T( "GasFlowTagGE" ), objId ) ) return;
	if( ArxUtilHelper::IsEqualType( _T( "TagGE" ), objId ) && !ArxUtilHelper::IsEqualType( _T( "GasPumpGE" ), objId )) return;

	PropertyDataDlgHelper::DisplayPropertyDataDlg( objId );
}

void UIHelper::DisplayDrillDataByFunc(const CString& funcName)
{
	// 显示图元的属性数据
	AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个钻孔:" ) );
	if( objId.isNull() ) return;
	if( !ArxUtilHelper::IsEqualType( _T( "DrillGE" ), objId )) return;

	PropertyDataDlgHelper::DisplayPropertyByFunName(objId,funcName);
}

void UIHelper::DisplayGasTubeDataByFunc(const CString& funcName)
{
	// 显示图元的属性数据
	AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一条管路:" ) );
	if( objId.isNull() ) return;
	if( !ArxUtilHelper::IsEqualType( _T( "GasTube" ), objId )) return;

	PropertyDataDlgHelper::DisplayPropertyByFunName( objId,funcName );
}

void UIHelper::DisplayGasPumpDataByFunc(const CString& funcName)
{
	// 显示图元的属性数据
	AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个瓦斯泵:" ) );
	if( objId.isNull() ) return;
	if( !ArxUtilHelper::IsEqualType( _T( "GasPumpGE" ), objId )) return;

	PropertyDataDlgHelper::DisplayPropertyByFunName( objId,funcName );
}

void UIHelper::DisplayDataByDoubleClick()
{
    //acutPrintf(_T("\n双击自定义..."));
    AcDbObjectIdArray objIds;
    ArxUtilHelper::GetPickSetEntity( objIds );
    if( objIds.length() != 1 ) return;

	//if( ArxUtilHelper::IsEqualType( _T( "DrillGE" ), objIds[0] ))
	//{
	//	PropertyDataDlgHelper::DisplayPropertyByFunName(objIds[0],_T("高位钻孔参数计算"));
	//}

	//else if( ArxUtilHelper::IsEqualType( _T( "GasTube" ), objIds[0] ))
	//{
	//	PropertyDataDlgHelper::DisplayPropertyByFunName(objIds[0],_T("管路计算"));
	//}

	//else if( ArxUtilHelper::IsEqualType( _T( "GasPumpGE" ), objIds[0] ))
	//{
	//	PropertyDataDlgHelper::DisplayPropertyByFunName(objIds[0],_T("瓦斯泵计算"));
	//}

	//else
	//{
		// 显示属性对话框
		PropertyDataDlgHelper::DisplayPropertyDataDlg( objIds[0] );
	//}
}

void UIHelper::DisplayDataByFuncName(const CString& funcName)
{
	//acutPrintf(_T("\n双击自定义..."));
	//AcDbObjectIdArray objIds;
	//ArxUtilHelper::GetPickSetEntity( objIds );
	//if( objIds.length() != 1 ) return;
	AcDbObjectId objId;
	SingleDataObjectHelper::GetObjectId(_T("计算参数"),objId);
	//AcStringArray funcs;
	//CString funcName = _T("基本信息");
	//funcs.append(funcName);
	// 显示属性对话框
	PropertyDataDlgHelper::DisplayPropertyByFunName( objId, funcName );
}