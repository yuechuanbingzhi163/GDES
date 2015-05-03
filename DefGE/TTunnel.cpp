#include "StdAfx.h"
#include "TTunnel.h"

Adesk::UInt32 TTunnel::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    TTunnel, LinkedGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    掘进工作面, DEFGEAPP
)

TTunnel::TTunnel () : LinkedGE()
{
}

TTunnel::TTunnel( const AcGePoint3d& startPt, const AcGePoint3d& endPt ) : LinkedGE( startPt, endPt )
{

}
TTunnel::~TTunnel ()
{
}

//Acad::ErrorStatus TTunnel::dwgOutFields ( AcDbDwgFiler* pFiler ) const
//{
//    assertReadEnabled () ;
//
//    //acutPrintf(_T("\TTunnel::dwgOutFields"));
//    Acad::ErrorStatus es = LinkedGE::dwgOutFields ( pFiler ) ;
//    if ( es != Acad::eOk )
//        return ( es ) ;
//
//    if ( ( es = pFiler->writeUInt32 ( TTunnel::kCurrentVersionNumber ) ) != Acad::eOk )
//        return ( es ) ;
//
//    // 保存"导风巷道"
//    pFiler->writeHardPointerId( m_inTunnel );
//
//    return ( pFiler->filerStatus () ) ;
//}
//
//Acad::ErrorStatus TTunnel::dwgInFields ( AcDbDwgFiler* pFiler )
//{
//    assertWriteEnabled () ;
//
//    //acutPrintf(_T("\TTunnel::dwgInFields"));
//    Acad::ErrorStatus es = LinkedGE::dwgInFields ( pFiler ) ;
//    if ( es != Acad::eOk )
//        return ( es ) ;
//
//    Adesk::UInt32 version = 0 ;
//    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
//        return ( es ) ;
//    if ( version > TTunnel::kCurrentVersionNumber )
//        return ( Acad::eMakeMeProxy ) ;
//
//    // 读取"导风巷道"
//    AcDbHardPointerId objId;
//    pFiler->readHardPointerId( &objId );
//    m_inTunnel = objId;
//
//    return ( pFiler->filerStatus () ) ;
//}
//
//Adesk::Boolean TTunnel::subWorldDraw( AcGiWorldDraw* mode )
//{
//    assertReadEnabled();
//
//    Adesk::Boolean ret = Adesk::kTrue;
//    if( !m_inTunnel.isNull() )
//    {
//        ret = LinkedGE::subWorldDraw( mode );
//    }
//    return ret;
//}
//
//void TTunnel::setInTunnel( const AcDbObjectId& objId )
//{
//    assertWriteEnabled();
//    if( objId.isNull() ) return;
//    if( m_inTunnel == objId ) return;
//    m_inTunnel = objId;
//
//}
//
//AcDbObjectId TTunnel::getInTunnel() const
//{
//    assertReadEnabled();
//    return m_inTunnel;
//}