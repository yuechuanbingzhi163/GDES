#include "StdAfx.h"
#include "UIHelper.h"

#include "DrawManagerDlg.h"
#include "FieldManagerDlg.h"
//#include "PropertyDataDlg.h"
#include "VarListDlg.h"
#include "TypeNameMapDlg.h"
#include "DataObjectStateDlg.h"

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

void UIHelper::ShowTypeNameMapDlg()
{
    CAcModuleResourceOverride myResources;

    TypeNameMapDlg tnmd;
    tnmd.DoModal();
}

void UIHelper::ShowDataObjectStateDlg()
{
    CAcModuleResourceOverride myResources;

    DataObjectStateDlg dosd;
    dosd.DoModal();
}
