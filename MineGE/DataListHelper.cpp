#include "StdAfx.h"
#include "DataListHelper.h"

#include "config.h"
#include "DataObject.h"

#include "../ArxHelper/HelperClass.h"

void StringListHelper::GetAllNames( AcStringArray& names )
{
    ArxDictHelper::GetAllKeys( STRING_LIST_DICT, names );
}

bool StringListHelper::AddString( const CString& name, const CString& strValue )
{
    return ArxDictHelper::AddEntry( STRING_LIST_DICT, name, strValue );
}

bool StringListHelper::AddStringList( const CString& name, const AcStringArray& strList )
{
    if( ( name.GetLength() == 0 ) || strList.isEmpty() ) return false;

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( STRING_LIST_DICT );
    bool ret = pDictTool->findKey( name );
    if( !ret ) // 如果变量name已存在，则不进行修改
    {
        int len = strList.length();
        for( int i = 0; i < len; i++ )
        {
            pDictTool->addEntry( name, strList[i].kACharPtr() );
        }
    }
    delete pDictTool;
    return !ret;
}

bool StringListHelper::GetStringList( const CString& name, AcStringArray& strList )
{
    return ArxDictHelper::GetAllEntries( STRING_LIST_DICT, name, strList );
}

void StringListHelper::RemoveStringList( const CString& name )
{
    ArxDictHelper::RemoveAllEntries( STRING_LIST_DICT, name );
}

void IntStrListHelper::GetAllNames( AcStringArray& names )
{
    ArxDictHelper::GetAllKeys( INT_LIST_DICT, names );
}

bool IntStrListHelper::AddIntStrPair( const CString& name, int intValue, const CString& strValue )
{
    if( ( name.GetLength() == 0 ) || strValue.GetLength() == 0 ) return false;

    AcStringArray strList;
    AcDbIntArray intList;
    GetIntStrList( name, intList, strList );
    if( intList.contains( intValue ) || strList.contains( strValue ) ) return false;

    intList.append( intValue );
    strList.append( strValue );
    RemoveIntStrList( name );
    return AddIntStrList( name, intList, strList );
}

bool IntStrListHelper::AddIntStrList( const CString& name, const AcDbIntArray& intList, const AcStringArray& strList )
{
    if( ( name.GetLength() == 0 ) || strList.isEmpty() || intList.isEmpty() ) return false;
    if( intList.length() != strList.length() ) return false;

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( INT_LIST_DICT );
    bool ret = pDictTool->findKey( name );
    if( !ret )
    {
        int len = intList.length();
        for( int i = 0; i < len; i++ )
        {
            CString intValue;
            intValue.Format( _T( "%d" ), intList[i] );
            pDictTool->addEntry( name, intValue );             // 偶数位置的元素为整数
            pDictTool->addEntry( name, strList[i].kACharPtr() ); // 奇数位置的元素为字符串
        }
    }
    delete pDictTool;
    return !ret;
}

bool IntStrListHelper::GetIntStrList( const CString& name, AcDbIntArray& intList, AcStringArray& strList )
{
    AcStringArray entries;
    if( !ArxDictHelper::GetAllEntries( INT_LIST_DICT, name, entries ) ) return false;

    int len = entries.length();
    bool ret = ( len > 0 && len % 2 == 0 );
    if( ret ) // 长度必须为偶数
    {
        intList.removeAll();
        strList.removeAll();

        for( int i = 0; i < len; i++ )
        {
            if( i % 2 == 0 ) // 偶数位置的元素为整数
            {
                intList.append( _ttoi( entries[i].kACharPtr() ) );
            }
            else       // 奇数位置的元素为字符串
            {
                strList.append( entries[i] );
            }
        }
    }
    return ret;
}

void IntStrListHelper::RemoveIntStrList( const CString& name )
{
    ArxDictHelper::RemoveAllEntries( INT_LIST_DICT, name );
}

void DataObjectListHelper::GetAllNames( AcStringArray& names )
{
    ArxDictHelper::GetAllKeys( OBJECT_LIST_DICT, names );
}

bool DataObjectListHelper::AddObject( const CString& name, const CString& value )
{
    if( name.GetLength() == 0 || value.GetLength() == 0 ) return false;

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( OBJECT_LIST_DICT );
    bool ret = ( INVALID_ENTRY != pDictTool->addEntry( name, value ) );
    if( ret )
    {
        ArxDictTool2* pDictTool2 = ArxDictTool2::GetDictTool( OBJECT_LIST_DATA_DICT );
        CString key;
        key.Format( _T( "%s_%s" ), name, value );

        DataObject* pDO = new DataObject();
        pDO->setType( name );
        pDO->initData();  // 初始化数据
        ret = pDictTool2->addEntry( key, pDO );
        if( !ret ) // 添加Object失败
        {
            delete pDO;
            pDictTool->removeEntry( name, value );
        }
        delete pDictTool2;
    }
    delete pDictTool;

    return ret;
}

bool DataObjectListHelper::RemoveObject( const CString& name, const CString& value )
{
    if( name.GetLength() == 0 || value.GetLength() == 0 ) return false;

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( OBJECT_LIST_DICT );
    bool ret = ( INVALID_ENTRY != pDictTool->removeEntry( name, value ) );
    delete pDictTool;

    if( ret )
    {
        ArxDictTool2* pDictTool2 = ArxDictTool2::GetDictTool( OBJECT_LIST_DATA_DICT );
        CString key;
        key.Format( _T( "%s_%s" ), name, value );
        ret = pDictTool2->removeEntry( key );
        delete pDictTool2;
    }
    return ret;
}

bool DataObjectListHelper::RemoveAllObject( const CString& name )
{
    if( name.GetLength() == 0 ) return false;

    AcStringArray values;
    GetObjectNameList( name, values );

    bool ret = true;
    int len = values.length();
    for( int i = 0; i < len; i++ )
    {
        ret = ( ret && RemoveObject( name, values[i].kACharPtr() ) );
    }
    ArxDictHelper::RemoveAllEntries( OBJECT_LIST_DICT, name );
    return ret;
}

bool DataObjectListHelper::GetObjectNameList( const CString& name, AcStringArray& values )
{
    if( name.GetLength() == 0 ) return false;

    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( OBJECT_LIST_DICT );
    pDictTool->getAllEntries( name, values );
    delete pDictTool;
    return true;
}

bool DataObjectListHelper::IsObjectListType( const CString& name )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( OBJECT_LIST_DICT );
    bool ret = pDictTool->findKey( name );
    delete pDictTool;
    return ret;
}

bool DataObjectListHelper::GetObjectId( const CString& name, const CString& value, AcDbObjectId& objId )
{
    if( name.GetLength() == 0 || value.GetLength() == 0 ) return false;

    ArxDictTool2* pDictTool2 = ArxDictTool2::GetDictTool( OBJECT_LIST_DATA_DICT );
    CString key;
    key.Format( _T( "%s_%s" ), name, value );
    bool ret = pDictTool2->findEntry( key, objId );
    delete pDictTool2;

    return ret;
}

void SingleDataObjectHelper::GetAllNames( AcStringArray& names )
{
    ArxDictHelper::GetAllKeys( GLOBAL_SINGLE_INFO_DICT, names );
}

bool SingleDataObjectHelper::AddObject( const CString& name )
{
    if( name.GetLength() == 0 ) return false;

    ArxDictTool2* pDictTool2 = ArxDictTool2::GetDictTool( GLOBAL_SINGLE_INFO_DICT );
    DataObject* pDO = new DataObject();
    pDO->setType( name );
    pDO->initData();  // 初始化数据
    bool ret = pDictTool2->addEntry( name, pDO );
    if( !ret ) delete pDO; // 添加Object失败
    delete pDictTool2;
    return ret;
}

bool SingleDataObjectHelper::RemoveObject( const CString& name )
{
    if( name.GetLength() == 0 ) return false;

    ArxDictTool2* pDictTool2 = ArxDictTool2::GetDictTool( GLOBAL_SINGLE_INFO_DICT );
    bool ret = pDictTool2->removeEntry( name );
    delete pDictTool2;
    return ret;
}

bool SingleDataObjectHelper::GetObjectId( const CString& name, AcDbObjectId& objId )
{
    if( name.GetLength() == 0 ) return false;

    ArxDictTool2* pDictTool2 = ArxDictTool2::GetDictTool( GLOBAL_SINGLE_INFO_DICT );
    bool ret = pDictTool2->findEntry( name, objId );
    delete pDictTool2;
    return ret;
}

void ReporterDataObjectHelper::GetAllNames( AcStringArray& names )
{
	ArxDictHelper::GetAllKeys( REPORTER_INFO_DICT, names );
}

bool ReporterDataObjectHelper::AddObject( const CString& name )
{
	if( name.GetLength() == 0 ) return false;

	ArxDictTool2* pDictTool2 = ArxDictTool2::GetDictTool( REPORTER_INFO_DICT );
	DataObject* pDO = new DataObject();
	pDO->initData();  // 初始化数据
	bool ret = pDictTool2->addEntry( name, pDO );
	if( !ret ) delete pDO; // 添加Object失败
	delete pDictTool2;
	return ret;
}

bool ReporterDataObjectHelper::RemoveObject( const CString& name )
{
	if( name.GetLength() == 0 ) return false;

	ArxDictTool2* pDictTool2 = ArxDictTool2::GetDictTool( REPORTER_INFO_DICT );
	bool ret = pDictTool2->removeEntry( name );
	delete pDictTool2;
	return ret;
}

bool ReporterDataObjectHelper::GetObjectId( const CString& name, AcDbObjectId& objId )
{
	if( name.GetLength() == 0 ) return false;

	ArxDictTool2* pDictTool2 = ArxDictTool2::GetDictTool( REPORTER_INFO_DICT );
	bool ret = pDictTool2->findEntry( name, objId );
	delete pDictTool2;
	return ret;
}