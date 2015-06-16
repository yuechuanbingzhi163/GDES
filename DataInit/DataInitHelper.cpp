#include "StdAfx.h"
#include "DataInitHelper.h"

#include "../MineGE/HelperClass.h"
#include "../MineGE/FuncFieldHelper.h"
#include "../MineGE/FieldHelper.h"

#include <fstream>
// 需要包含<fstream>才能使用
#include "AcFStream.h"

void DataInitHelper::InitStrList( const CString& fileName )
{
    acutPrintf( _T( "\n从路径: %s加载字符串列表..." ), fileName );
    AcIfstream inFile( fileName );
    if( !inFile ) return;

    while( !inFile.eof() )
    {
        ACHAR name[_MAX_PATH], strValue[_MAX_PATH];
        inFile >> name >> strValue;
		if(inFile.fail()) break;
        StringListHelper::AddString( name, strValue );
    }
    inFile.close();
}

// 初始化"字符串-整数"列表
void DataInitHelper::InitIntStrList( const CString& fileName )
{
    acutPrintf( _T( "\n从路径: %s加载字符串-整数列表..." ), fileName );
    AcIfstream inFile( fileName );
    if( !inFile ) return;

    while( !inFile.eof() )
    {
        ACHAR name[_MAX_PATH], strValue[_MAX_PATH];
        int intValue;
        inFile >> name >> intValue >> strValue;
		if(inFile.fail()) break;
        IntStrListHelper::AddIntStrPair( name, intValue, strValue );
    }
    inFile.close();
}

void DataInitHelper::InitDataObjectList( const CString& fileName )
{
    acutPrintf( _T( "\n从路径: %s加载数据对象列表..." ), fileName );

    AcIfstream inFile( fileName );
    if( !inFile ) return;

    while( !inFile.eof() )
    {
        ACHAR name[_MAX_PATH], value[_MAX_PATH];
        inFile >> name >> value;
		if(inFile.fail()) break;
        DataObjectListHelper::AddObject( name, value );
    }
    inFile.close();
}


void DataInitHelper::InitSingleDataObject( const CString& fileName )
{
    acutPrintf( _T( "\n从路径: %s加载全局信息..." ), fileName );

    AcIfstream inFile( fileName );
    if( !inFile ) return;

    while( !inFile.eof() )
    {
        ACHAR name[_MAX_PATH];
        inFile >> name;
		if(inFile.fail()) break;
        SingleDataObjectHelper::AddObject( name );
    }
    inFile.close();
}

static void SetFieldInfo(FieldInfo& info,const CString& fieldType, const CString& defaultValue,const CString& tole,const CString& editable,const CString& toolTips)
{
	bool enable = _ttoi(editable) ? true:false;
	info.m_enable = enable;
	info.m_showInTooltip = true;
	info.m_descr = toolTips;
	if (_T("DT_STRING") == fieldType)
	{
		info.m_dt = DT_STRING;
	}

	if (_T("DT_INT") == fieldType)
	{
		info.m_dt = DT_INT;
		int pos = defaultValue.Find(_T("-"));
		int minInt = _ttoi(defaultValue.Left(pos));
		int maxInt = _ttoi(defaultValue.Mid(pos+1,defaultValue.GetLength()));
		//acutPrintf(_T("\n最小值:%d->最大值:%d"),minInt,maxInt);
		info.m_minValue2 = minInt;
		info.m_maxValue2 = maxInt;
	}

	if (_T("DT_NUMERIC") == fieldType)
	{
		info.m_dt = DT_NUMERIC;
		int pos = defaultValue.Find(_T("-"));
		double minDouble = _tstof(defaultValue.Left(pos));
		double maxDouble = _tstof(defaultValue.Mid(pos+1,defaultValue.GetLength()));
		//acutPrintf(_T("\n最小值:%lf->最大值:%lf"),minDouble,maxDouble);
		info.m_minValue = minDouble;
		info.m_maxValue = maxDouble;
		info.m_tolrance = _ttoi(tole);
	}

	if (_T("DT_LIST") == fieldType)
	{
		info.m_dt = DT_LIST;
		int pos = defaultValue.Find(_T("-"));
		CString ltType = defaultValue.Left(pos);
		CString ltValue = defaultValue.Mid(pos+1,defaultValue.GetLength());
		if(_T("LT_INT") == ltType)	info.m_lt = LT_INT;
		if(_T("LT_STRING") == ltType)	info.m_lt = LT_STRING;
		if(_T("LT_OBJECT") == ltType)	info.m_lt = LT_OBJECT;
		info.m_varName = ltValue;
	}

	if (_T("DT_BOOL") == fieldType)
	{
		info.m_dt = DT_BOOL;
	}

	if (_T("DT_DATE") == fieldType)
	{
		info.m_dt = DT_DATE;
	}
}
// 初始化图元字段
// 从文件中读取数据，格式
// 图元类型	字段1
// 图元类型	字段2
// ...
void DataInitHelper::InitDataFieldWithMoreInfo( const CString& fileName )
{
    acutPrintf( _T( "\n从路径: %s加载字段" ), fileName );

    AcIfstream inFile( fileName );
    if( !inFile ) return;
	
    while( !inFile.eof() )
    {
        ACHAR func[_MAX_PATH],getype[_MAX_PATH], field[_MAX_PATH],fieldType[_MAX_PATH],defaultValue[_MAX_PATH],tole[_MAX_PATH],editable[_MAX_PATH],toolTips[_MAX_PATH];
        inFile >> func >> getype >> field >> fieldType >> defaultValue >> tole >> editable >> toolTips;
		if(inFile.fail()) break;
		
		FieldInfo fieldInfo;
		SetFieldInfo(fieldInfo,fieldType,defaultValue,tole,editable,toolTips);
		FuncFieldHelper::AddField(func,getype,field);
		FieldHelper::AddField( getype, field );
		
		// 新增的字段
		int index = FieldHelper::FindField(getype,field);
		if( index < 1 || !FieldInfoHelper::FindFieldInfo( getype, field ) )
		{
			FieldInfoHelper::AddFieldInfo( getype, field, fieldInfo );
		}
		else
		{
			// 更新已有的字段信息
			FieldInfoHelper::RemoveFieldInfo( getype, field );
			FieldInfoHelper::AddFieldInfo( getype, field, fieldInfo );
		}
	}
	inFile.close();
}

void DataInitHelper::InitDataField( const CString& fileName )
{
	acutPrintf( _T( "\n从路径: %s加载字段" ), fileName );

	AcIfstream inFile( fileName );
	if( !inFile ) return;

	while( !inFile.eof() )
	{
		ACHAR getype[_MAX_PATH], field[_MAX_PATH];
		inFile >> getype >> field;
		if(inFile.fail()) break;

		FieldHelper::AddField( getype, field );
	}
	inFile.close();
}
