#include "StdAfx.h"
#include "TagGE.h"

#include "../ArxHelper/HelperClass.h"

Adesk::UInt32 TagGE::kCurrentVersionNumber = 1 ;

Adesk::UInt32 DirectionTagGE::kCurrentVersionNumber = 1 ;

//Adesk::UInt32 TextTagGE::kCurrentVersionNumber =1 ;

// 有修改，使得TagGE成为抽象类
ACRX_NO_CONS_DEFINE_MEMBERS ( TagGE, MineGE )

// 有修改，使得TagGE成为抽象类
ACRX_NO_CONS_DEFINE_MEMBERS ( DirectionTagGE, TagGE )

ACRX_NO_CONS_DEFINE_MEMBERS ( FacilityGE, TagGE )

//ACRX_NO_CONS_DEFINE_MEMBERS (TextTagGE, TagGE)

TagGE::TagGE()
{
}

DirectionTagGE::DirectionTagGE()
{

}

DirectionTagGE::DirectionTagGE( const AcGePoint3d& insertPt, double angle )
    : m_insertPt( insertPt ), m_angle( angle )
{

}
//TextTagGE::TextTagGE()
//{
//
//}

AcDbObjectId TagGE::getRelatedGE() const
{
    assertReadEnabled();
    return m_objId;
}

void TagGE::setRelatedGE( const AcDbObjectId& objId )
{
    assertWriteEnabled();
    if( objId.isNull() || ( objId == m_objId ) ) return;
    if( objId == objectId() ) return;                            // 不能和图元自身关联
    if( !ArxUtilHelper::IsEqualType( _T( "MineGE" ), objId ) ) return; // 必须从MineGE派生

    m_objId = objId;
}

Acad::ErrorStatus TagGE::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = MineGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( TagGE::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeHardPointerId( m_objId );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus TagGE::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = MineGE::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > TagGE::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    AcDbHardPointerId objId;
    pFiler->readHardPointerId( &objId );
    m_objId = objId;

    return ( pFiler->filerStatus () ) ;
}

Adesk::Boolean TagGE::subWorldDraw( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    if( m_objId.isNull() ) return Adesk::kTrue;

    return MineGE::subWorldDraw( mode );
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
AcGePoint3d DirectionTagGE::getInsertPt() const
{
    assertReadEnabled();
    return m_insertPt;
}

void DirectionTagGE::setInsertPt( const AcGePoint3d& pt )
{
    assertWriteEnabled();
    m_insertPt = pt;
}

double DirectionTagGE::getDirectionAngle() const
{
    assertReadEnabled();
    return m_angle;
}

void DirectionTagGE::setDirectionAngle( double angle )
{
    assertWriteEnabled();
    m_angle = angle;
}

void DirectionTagGE::readKeyParam( DrawParamReader& reader )
{
    reader.readPoint( m_insertPt );
    reader.readDouble( m_angle );
}

void DirectionTagGE::writeKeyParam( DrawParamWriter& writer ) const
{
    writer.writePoint( m_insertPt );
    writer.writeDouble( m_angle );
}

Acad::ErrorStatus DirectionTagGE::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = TagGE::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( TagGE::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    pFiler->writeItem( m_insertPt );
    pFiler->writeItem( m_angle );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus DirectionTagGE::dwgInFields ( AcDbDwgFiler* pFiler )
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
    if ( version > TagGE::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;

    pFiler->readItem( &m_insertPt );
    pFiler->readItem( &m_angle );

    return ( pFiler->filerStatus () ) ;
}
//void TextTagGE::addData( const CString& field )
//{
//	if(m_fields.contains(field)) return;
//
//	m_fields.append(field);
//}
//
//void TextTagGE::removeData( const CString& field )
//{
//	m_fields.remove(field);
//}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
AcGePoint3d FacilityGE::getInsertPt() const
{
	assertReadEnabled();
	return m_insertPt;
}

void FacilityGE::setInsertPt( const AcGePoint3d& pt )
{
	assertWriteEnabled();
	m_insertPt = pt;
}

double FacilityGE::getDirectionAngle() const
{
	assertReadEnabled();
	return m_angle;
}

void FacilityGE::setDirectionAngle( double angle )
{
	assertWriteEnabled();
	m_angle = angle;
}

void FacilityGE::readKeyParam( DrawParamReader& reader )
{
	reader.readPoint( m_insertPt );
	reader.readDouble( m_angle );
}

void FacilityGE::writeKeyParam( DrawParamWriter& writer ) const
{
	writer.writePoint( m_insertPt );
	writer.writeDouble( m_angle );
}

Acad::ErrorStatus FacilityGE::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es = DirectionTagGE::dwgOutFields ( pFiler ) ;
	if ( es != Acad::eOk )
		return ( es ) ;
	//----- Object version number needs to be saved first
	if ( ( es = pFiler->writeUInt32 ( DirectionTagGE::kCurrentVersionNumber ) ) != Acad::eOk )
		return ( es ) ;

	pFiler->writeItem( m_insertPt );
	pFiler->writeItem( m_angle );

	return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus FacilityGE::dwgInFields ( AcDbDwgFiler* pFiler )
{
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es = DirectionTagGE::dwgInFields ( pFiler ) ;
	if ( es != Acad::eOk )
		return ( es ) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0 ;
	if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
		return ( es ) ;
	if ( version > DirectionTagGE::kCurrentVersionNumber )
		return ( Acad::eMakeMeProxy ) ;

	pFiler->readItem( &m_insertPt );
	pFiler->readItem( &m_angle );

	return ( pFiler->filerStatus () ) ;
}

FacilityGE::FacilityGE()
{

}

FacilityGE::FacilityGE( const AcGePoint3d& insertPt, double angle ):m_insertPt(insertPt),m_angle(angle)
{

}