#include "StdAfx.h"
#include "ModelGE.h"

#include "../ArxHelper/HelperClass.h"

Adesk::UInt32 ModelGE::kCurrentVersionNumber = 1 ;

ACRX_NO_CONS_DEFINE_MEMBERS ( ModelGE, AcDbEntity )

ModelGE::ModelGE () : AcDbEntity ()
{
}

CString ModelGE::getTypeName() const
{
    // 返回类型名称
    // 使用了虚函数方法isA()
    return this->isA()->name();
}

AcDbObjectId ModelGE::getDataObject() const
{
    assertReadEnabled();
    return m_objId;
}

void ModelGE::setDataObject( const AcDbObjectId& objId )
{
    assertWriteEnabled();
    if( objId.isNull() || ( objId == m_objId ) || objId == objectId() ) return;

    if( ArxUtilHelper::IsEqualType( _T( "DataObject" ), objId ) )
    {
        m_objId = objId;
    }
}

Acad::ErrorStatus ModelGE::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = AcDbEntity::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( ModelGE::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    // 保存id
    pFiler->writeHardPointerId( m_objId );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus ModelGE::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = AcDbEntity::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > ModelGE::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    // 读取id
    AcDbHardPointerId objId;
    pFiler->readHardPointerId( &objId );
    m_objId = objId;

    return ( pFiler->filerStatus () ) ;
}
