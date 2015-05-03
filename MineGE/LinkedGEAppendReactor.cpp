#include "StdAfx.h"
#include "LinkedGEAppendReactor.h"
#include "LinkedGE.h"
#include "DrawHelper.h"

LinkedGEAppendReactor::LinkedGEAppendReactor ( AcDbDatabase* pDb ) : AcDbDatabaseReactor(), mpDatabase( pDb )
{
    if ( pDb )
        pDb->addReactor ( this ) ;
}

LinkedGEAppendReactor::~LinkedGEAppendReactor ()
{
    Detach () ;
}

void LinkedGEAppendReactor::Attach ( AcDbDatabase* pDb )
{
    Detach () ;
    if ( mpDatabase == NULL )
    {
        if ( ( mpDatabase = pDb ) != NULL )
            pDb->addReactor ( this ) ;
    }
}

void LinkedGEAppendReactor::Detach ()
{
    if ( mpDatabase )
    {
        mpDatabase->removeReactor ( this ) ;
        mpDatabase = NULL ;
    }
}

AcDbDatabase* LinkedGEAppendReactor::Subject () const
{
    return ( mpDatabase ) ;
}

bool LinkedGEAppendReactor::IsAttached () const
{
    return ( mpDatabase != NULL ) ;
}

static void DoEdgeGEJunctionClosure( LinkedGE* pEdge )
{
    AcGePoint3d spt, ept;
    pEdge->getSEPoint( spt, ept );

    DrawHelper::LinkedGEJunctionClosure( spt );
    DrawHelper::LinkedGEJunctionClosure( ept );
}

static void DoEdgeGEJunctionClosure2( const AcDbObject* pObj )
{
    LinkedGE* pEdge = LinkedGE::cast( pObj );
    if( pEdge != 0 )
    {
        //DrawHelper::LinkedGEJunctionClosure2(pEdge->objectId());
        DoEdgeGEJunctionClosure( pEdge );
    }
}

void LinkedGEAppendReactor::objectAppended( const AcDbDatabase* db, const AcDbObject* pObj )
{
    //acutPrintf(_T("\noid:%d call objectAppended()...:%s"), pObj->objectId());

    DoEdgeGEJunctionClosure2( pObj );
}
