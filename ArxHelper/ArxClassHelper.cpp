#include "StdAfx.h"
#include "ArxClassHelper.h"

// 查找从根类型派生的图元
void ArxClassHelper::GetArxClassTypes( const CString& root, AcStringArray& types, bool findAbstractType )
{
    // 如果根类型不存在，默认为AcDbObject
    AcRxClass* pParentClass = AcRxClass::cast( acrxClassDictionary->at( root ) );
    if( pParentClass == 0 ) pParentClass = AcDbObject::desc();

    AcRxDictionaryIterator* iter = acrxClassDictionary->newIterator();
    if( iter == 0 ) return;

    for( ; !iter->done(); iter->next() )
    {
        AcRxClass* pClass;
        if( ( pClass = AcRxClass::cast( iter->object() ) ) != NULL )
        {
            if( pClass->isDerivedFrom( pParentClass ) )
            {
                const ACHAR* text = pClass->name();
                if( text == NULL ) continue;

                if( !findAbstractType && ( pClass->dxfName() == NULL ) || ( pClass->appName() == NULL ) ) continue;

                //acutPrintf(_T("appName:%s\ndxfName:%s\nname:%s\n"), pClass->appName(), pClass->dxfName(), pClass->name());
                types.append( text );
            }
        }
    }
    delete iter;
}

bool ArxClassHelper::IsValidClass( const CString& type )
{
    AcRxClass* pClass = AcRxClass::cast( acrxClassDictionary->at( type ) );
    return ( pClass != 0 );
}

bool ArxClassHelper::IsAbstractClass( const CString& type )
{
    AcRxClass* pClass = AcRxClass::cast( acrxClassDictionary->at( type ) );
    if( pClass == 0 ) return false; // 类型不存在

    // 抽象类，使用ACRX_NO_CONS_DEFINE_MEMBERS宏定义
    return ( ( pClass->dxfName() == NULL ) || ( pClass->appName() == NULL ) );
}

bool ArxClassHelper::IsDerivedFrom( const CString& type, const CString& parentType )
{
    AcRxClass* pClass1 = AcRxClass::cast( acrxClassDictionary->at( type ) );
    if( pClass1 == 0 ) return false; // 类型不存在

    AcRxClass* pClass2 = AcRxClass::cast( acrxClassDictionary->at( parentType ) );
    if( pClass2 == 0 ) return false; // 类型不存在

    return pClass1->isDerivedFrom( pClass2 );
}

// 获取图元的dxf名称
// 和类名称一样，dxf名称也必须是唯一的
bool ArxClassHelper::GetDxfName( const CString& type, CString& dxfName )
{
    AcRxClass* pClass = AcRxClass::cast( acrxClassDictionary->at( type ) );
    if( pClass == 0 ) return false;

    dxfName = pClass->dxfName();
    return true;
}

void ArxClassHelper::GetClassHierarchy( const CString& type, AcStringArray& types, bool findAbstractType )
{
    AcRxClass* pClass = AcRxClass::cast( acrxClassDictionary->at( type ) );
    if( pClass == 0 ) return;

    while( pClass != AcDbEntity::desc() )
    {
        // 排除抽象类型
        if( !findAbstractType && ( ( pClass->dxfName() == NULL ) || ( pClass->appName() == NULL ) ) )
        {
            pClass = pClass->myParent();
            continue;
        }

        types.append( pClass->name() );
        pClass = pClass->myParent();
    }
    types.reverse();
}

bool ArxClassHelper::GetChildClass( const CString& type, AcStringArray& types )
{
    AcRxClass* pParentClass = AcRxClass::cast( acrxClassDictionary->at( type ) );
    if( pParentClass == 0 ) return false; // 类型不存在

    AcRxDictionaryIterator* iter = acrxClassDictionary->newIterator();
    if( iter == 0 ) return false;

    AcRxClass* pClass;
    for( ; !iter->done(); iter->next() )
    {
        if( ( pClass = AcRxClass::cast( iter->object() ) ) != NULL )
        {
            const ACHAR* text = pClass->name();
            if( ( text != NULL ) && ( pClass->myParent() == pParentClass ) )
            {
                types.append( text );
            }
        }
    }
    delete iter;
    return true;
}

bool ArxClassHelper::GetTopParentClass( const CString& type, CString& parentType )
{
    AcStringArray types;
    GetClassHierarchy( type, types, false );
    if( types.isEmpty() ) return false;	   // type本身就是抽象类

    parentType = types.first().kACharPtr();
    return true;
}

bool ArxClassHelper::GetAllTopParentClass( const CString& root, AcStringArray& types )
{
    AcStringArray allGETypes;
    ArxClassHelper::GetArxClassTypes( root, allGETypes, false );
    if( allGETypes.isEmpty() ) return false;

    int len = allGETypes.length();
    for( int i = 0; i < len; i++ )
    {
        CString parentType;
        if( !ArxClassHelper::GetTopParentClass( allGETypes[i].kACharPtr(), parentType ) ) continue;
        if( allGETypes[i].compareNoCase( parentType ) != 0 ) continue;
        if( !types.contains( parentType ) )
        {
            types.append( parentType );
        }
    }
    return true;
}

AcDbObject* ArxClassHelper::CreateObjectByType( const CString& type )
{
    AcRxClass* pClass = AcRxClass::cast( acrxClassDictionary->at( type ) );
    if( pClass == 0 ) return 0; // 类型不存在

    return AcDbObject::cast( pClass->create() );
}