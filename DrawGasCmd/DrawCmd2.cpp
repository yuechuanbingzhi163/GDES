#include "stdafx.h"
#include "DrawCmd.h"

#include "../ArxHelper/HelperClass.h"

#include "../GasGE/GOP.h"
#include "../GasGE/GCP.h"
#include "../GasGE/GPP.h"
#include "../GasGE/PPP.h"
#include "../GasGE/DCI.h"
#include "../GasGE/GFP.h"
#include "../GasGE/CDH.h"

#include "GOPDlg.h"
#include "GCPDlg.h"
#include "GPPDlg.h"
#include "PPPDlg.h"
#include "CSDCIDlg.h"
#include "WDCI1Dlg.h"
#include "WDCI2Dlg.h"
#include "WGFPDlg.h"
#include "TGFPDlg.h"
#include "CDHDlg.h"

// flag = 1 --> 小型突出点
// flag = 2 --> 中型突出点
// flag = 3 --> 大型突出点
// flag = 4 --> 特大型突出点
void DrawCmd::DrawGOP2()
{
    CAcModuleResourceOverride myResources;

    GOPDlg dlg;
    dlg.DoModal();
}

void DrawCmd::DrawGCP2()
{
    CAcModuleResourceOverride myResources;

    GCPDlg dlg;
    dlg.DoModal();
}

void DrawCmd::DrawGPP2()
{
    CAcModuleResourceOverride myResources;

    GPPDlg dlg;
    dlg.DoModal();
}

void DrawCmd::DrawPPP2()
{
    CAcModuleResourceOverride myResources;

    PPPDlg dlg;
    dlg.DoModal();
}

// flag = 1 --> 煤层预测指标
// flag = 2 --> 工作面预测指标1
// flag = 3 --> 工作面预测指标2
void DrawCmd::DrawDCI2( int flag )
{
    CAcModuleResourceOverride myResources;
    if( flag == 1 )
    {
        CSDCIDlg dlg;
        dlg.DoModal();
    }
    else if( flag == 2 )
    {
        WDCI1Dlg dlg;
        dlg.DoModal();
    }
    else if( flag == 3 )
    {
        WDCI2Dlg dlg;
        dlg.DoModal();
    }
}

// flag = 1 --> 工作面瓦斯涌出
// flag = 2 --> 掘进工作面瓦斯涌出
void DrawCmd::DrawGFP2( int flag )
{
    CAcModuleResourceOverride myResources;

    if( flag == 1 )
    {
        WGFPDlg dlg;
        dlg.DoModal();
    }
    else if( flag == 2 )
    {
        TGFPDlg dlg;
        dlg.DoModal();
    }
}

void DrawCmd::DrawCDH2()
{
    CAcModuleResourceOverride myResources;
    CDHDlg dlg;
    dlg.DoModal();
}

static void DisplayGasPropertyData( const AcDbObjectId& objId )
{
    CAcModuleResourceOverride myResources;

    if( ArxUtilHelper::IsEqualType( _T( "GOP" ), objId ) )
    {
        GOPDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "GCP" ), objId ) )
    {
        GCPDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "GPP" ), objId ) )
    {
        GPPDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "PPP" ), objId ) )
    {
        PPPDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "CSDCI" ), objId ) )
    {
        CSDCIDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "WDCI1" ), objId ) )
    {
        WDCI1Dlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "WDCI2" ), objId ) )
    {
        WDCI2Dlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "WGFP" ), objId ) )
    {
        WGFPDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "TGFP" ), objId ) )
    {
        TGFPDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
    else if( ArxUtilHelper::IsEqualType( _T( "CDH" ), objId ) )
    {
        CDHDlg dlg;
        dlg.m_objId = objId;
        dlg.DoModal();
    }
}

void DrawCmd::DisplayDataByDoubleClick()
{
    //acutPrintf(_T("\n双击自定义..."));
    AcDbObjectIdArray objIds;
    ArxUtilHelper::GetPickSetEntity( objIds );
    if( objIds.length() != 1 ) return;

    // 显示属性对话框
    DisplayGasPropertyData( objIds[0] );
}