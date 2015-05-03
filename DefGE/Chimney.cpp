#include "StdAfx.h"
#include "Chimney.h"

Adesk::UInt32 Chimney::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    Chimney, TagGE,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation,
    ·çÍ², DEFGEAPP
)

Chimney::Chimney () : TagGE ()
{
}

Chimney::~Chimney ()
{
}

void Chimney::writeKeyParam( DrawParamWriter& writer ) const
{
    //assertReadEnabled();

    int len = m_pts.length();
    writer.writeInt( len );
    for( int i = 0; i < len; i++ )
    {
        writer.writePoint( m_pts[i] );
    }
}

void Chimney::readKeyParam( DrawParamReader& reader )
{
    //assertWriteEnabled();

    m_pts.removeAll();

    int len = 0;
    reader.readInt( len );
    for( int i = 0; i < len; i++ )
    {
        AcGePoint3d pt;
        reader.readPoint( pt );
        m_pts.append( pt );
    }
}

Acad::ErrorStatus Chimney::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = TagGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( Chimney::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;
    //----- Output params
    //.....

    int len = m_pts.length();
    pFiler->writeInt32( ( long )len );
    for( int i = 0; i < len; i++ )
    {
        pFiler->writePoint3d( m_pts[i] );
    }

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus Chimney::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = TagGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > Chimney::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;
    //- Uncomment the 2 following lines if your current object implementation cannot
    //- support previous version of that object.
    //if ( version < Chimney::kCurrentVersionNumber )
    //	return (Acad::eMakeMeProxy) ;
    //----- Read params
    //.....

    m_pts.removeAll();
    long len = 0;
    pFiler->readInt32( &len );
    for( int i = 0; i < ( int )len; i++ )
    {
        AcGePoint3d pt;
        pFiler->readPoint3d( &pt );
        m_pts.append( pt );
    }

    return ( pFiler->filerStatus () ) ;
}

void Chimney::addControlPoint( const AcGePoint3d& pt )
{
    if( !m_pts.contains( pt ) ) m_pts.append( pt );
}

AcGePoint3dArray Chimney::getControlPoint()
{
	return m_pts;
}

void Chimney::setControlPoint( AcGePoint3dArray pts )
{
	m_pts = pts;
}