#include "StdAfx.h"
#include "PropertyDataDlgHelper.h"

#include "PropertyDataDlg.h"
#include "PropertyDataDlgHelper.h"
#include "WcyPropertyDlg.h"

#include "GasWidthCaculDlg.h"
//#include "NearByGasDlg.h"
//#include "ValueHelper.h"

#include "../MineGE/FieldHelper.h"
#include "../MineGE/DataHelper.h"
#include "../MineGE/FuncFieldHelper.h"

static void setFieldInfo(CString type,CString field, FieldInfo fieldInfo)
{

	// 新增的字段
	int index = FieldHelper::FindField(type,field);
	if( index < 1 || !FieldInfoHelper::FindFieldInfo( type, field ) )
	{
		FieldInfoHelper::AddFieldInfo( type, field, fieldInfo );
	}
	else
	{
		// 更新已有的字段信息
		FieldInfoHelper::RemoveFieldInfo( type, field );
		FieldInfoHelper::AddFieldInfo( type, field, fieldInfo );
	}

}

static bool AddNearByFields( const AcDbObjectId& objId, const CString& func)
{
	CString type = _T("计算参数");
	CString numMine;
	DataHelper::GetPropertyData(objId,_T("邻近层煤层层数"),numMine);
	int dNumMine = _ttoi(numMine);
	if(dNumMine <= 0)
	{
		AfxMessageBox(_T("没有邻近层"));
		return false;
	}

	for (int i = 0; i < dNumMine; i++)
	{
		CString strmi,strX0i,strXi,strEntai;
		strmi.Format(_T("第%d邻近层的煤厚度"),i+1);
		strX0i.Format(_T("第%d邻近层的原始瓦斯含量"),i+1);
		strXi.Format(_T("第%d邻近层的残余瓦斯含量"),i+1);
		strEntai.Format(_T("第%d邻近层的瓦斯涌出率"),i+1);

		FieldInfo fieldInfo;
		fieldInfo.m_dt = DT_NUMERIC;
		fieldInfo.m_minValue = 0;
		fieldInfo.m_maxValue = 999999;
		fieldInfo.m_enable = true;
		fieldInfo.m_showInTooltip = true;
		fieldInfo.m_descr = _T("单位:m");

		FuncFieldHelper::AddField(func,type,strmi);
		FieldHelper::AddField(type,strmi);
		setFieldInfo(type,strmi,fieldInfo);

		FuncFieldHelper::AddField(func,type,strX0i);
		FieldHelper::AddField(type,strX0i);
		fieldInfo.m_descr = _T("单位:m3/t");
		setFieldInfo(type,strX0i,fieldInfo);

		FuncFieldHelper::AddField(func,type,strXi);
		FieldHelper::AddField(type,strXi);
		fieldInfo.m_descr = _T("单位:m3/t");
		setFieldInfo(type,strXi,fieldInfo);

		FuncFieldHelper::AddField(func,type,strEntai);
		FieldHelper::AddField(type,strEntai);
		fieldInfo.m_descr = _T("单位:%");
		fieldInfo.m_enable = false;
		setFieldInfo(type,strEntai,fieldInfo);
	}

	while(dNumMine > 0)
	{
		CString strmi,strX0i,strXi,strEntai;
		strmi.Format(_T("第%d邻近层的煤厚度"),dNumMine+1);
		strX0i.Format(_T("第%d邻近层的原始瓦斯含量"),dNumMine+1);
		strXi.Format(_T("第%d邻近层的残余瓦斯含量"),dNumMine+1);
		strEntai.Format(_T("第%d邻近层的瓦斯涌出率"),dNumMine+1);
		if(FuncFieldHelper::RemoveField(func,type,strmi) 
			&& FuncFieldHelper::RemoveField(func,type,strX0i) 
			&& FuncFieldHelper::RemoveField(func,type,strXi)
			&& FuncFieldHelper::RemoveField(func,type,strEntai))
		{
			dNumMine += 1;
		}
		else
		{
			dNumMine = 0;
		}
	}

	return true;

}
void PropertyDataDlgHelper::DisplayAllPropertyDataDlg( const AcDbObjectId& objId )
{
    if( objId.isNull() ) return;

    // 切换资源
    CAcModuleResourceOverride myResources;

    PropertyDataDlg pdd;
    pdd.showAllData( true );
    pdd.setMineGE( objId );
    pdd.DoModal();
}


void PropertyDataDlgHelper::DisplayPartialPropertyDataDlg( const AcDbObjectId& objId, const AcStringArray& fields )
{
    if( objId.isNull() ) return;
    if( fields.isEmpty() ) return;

    // 切换资源
    CAcModuleResourceOverride myResources;

    PropertyDataDlg pdd;
    pdd.showAllData( true );
    pdd.setMineGE( objId );
    int len = fields.length();
    for( int i = 0; i < len; i++ )
    {
        pdd.addField( fields[i].kACharPtr() );
		//acutPrintf(_T("\n字段:%s"),fields[i].kACharPtr());
    }
    pdd.DoModal();
}

bool PropertyDataDlgHelper::DisplayPropertyDataDlg( const AcDbObjectId& objId )
{
    if( objId.isNull() ) return false;

    AcDbObjectId dObjId;
    if( !DataHelper::GetDataObject( objId, dObjId ) ) return false;

    CString type;
    if( !DataHelper::GetTypeName( objId, type ) ) return false;

    AcStringArray fields;
    FieldHelper::GetAllFields( type, fields );
	//acutPrintf(_T("\n类型：%s"),type);
    if( fields.isEmpty() )
    {
        CString msg;
        //msg.Format(_T("类型:%s的字段个数为0，没有数据可以显示..."), type);
        msg.Format( _T( "没有数据可以显示..." ) );
        AfxMessageBox( msg );
        return false;
    }
    else
    {
		DisplayPartialPropertyDataDlg( dObjId, fields );
        return true;
    }
}

void PropertyDataDlgHelper::InputPropertyDataDlg( const AcDbObjectId& objId, const AcStringArray& fields, const CString& func )
{
	if( objId.isNull() ) return;
	if( fields.isEmpty() ) return;

	// 切换资源
	CAcModuleResourceOverride myResources;
	if (_T("参与评价的煤量") == func)
	{
		GasWidthCaculDlg gaswidthdlg;
		gaswidthdlg.setMineGE(objId);
		int ret = gaswidthdlg.DoModal();
		if (IDCANCEL == ret) return;
	}

	if (_T("邻近层瓦斯涌出量") == func)
	{
		if(!AddNearByFields(objId,func)) return;
	}

	//if (_T("管路阻力") == func)
	//{
	//	ValueHelper::setGasAirValue(objId);
	//}

	//if(_T("通过瓦斯抽采率计算") == func || _T("通过残余瓦斯压力反算") == func)
	//{
	//	WcyPropertyDlg pdd(NULL,func);
	//	pdd.showAllData( true );
	//	pdd.setMineGE( objId );
	//	int len = fields.length();
	//	for( int i = 0; i < len; i++ )
	//	{
	//		pdd.addField( fields[i].kACharPtr() );
	//		//acutPrintf(_T("\n字段:%s"),fields[i].kACharPtr());
	//	}
	//	pdd.DoModal();
	//}
	//else
	//{
		PropertyDataDlg pdd(NULL,func);
		pdd.showAllData( true );
		pdd.setMineGE( objId );
		int len = fields.length();
		for( int i = 0; i < len; i++ )
		{
			pdd.addField( fields[i].kACharPtr() );
			//acutPrintf(_T("\n字段:%s"),fields[i].kACharPtr());
		}
		pdd.DoModal();
	//}
}

bool PropertyDataDlgHelper::DisplayPropertyByFunName( const AcDbObjectId& objId, const CString funName )
{
	if( objId.isNull() ) return false;

	AcDbObjectId dObjId;
	if( !DataHelper::GetDataObject( objId, dObjId ) ) return false;

	CString type;
	if( !DataHelper::GetTypeName( objId, type ) ) return false;

	AcStringArray fields;
	FuncFieldHelper::GetFields(funName,type,fields);
	//for (int i = 0; i < fields.length(); i++)
	//{
	//	acutPrintf(_T("\n功能:%s\t字段:%s"),funName,fields[i]);
	//}
	//FieldHelper::GetAllFields( type, fields );
	if( fields.isEmpty() )
	{
		CString msg;
		//msg.Format(_T("类型:%s的字段个数为0，没有数据可以显示..."), type);
		msg.Format( _T( "没有数据可以显示..." ) );
		AfxMessageBox( msg );
		return false;
	}
	InputPropertyDataDlg( dObjId, fields,funName );
	return true;
}