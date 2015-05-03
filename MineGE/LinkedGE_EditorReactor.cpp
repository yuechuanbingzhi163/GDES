#include "StdAfx.h"
#include "LinkedGE_EditorReactor.h"
#include "LinkedGE.h"
#include "DrawHelper.h"

LinkedGE_EditorReactor::LinkedGE_EditorReactor ( const bool autoInitAndRelease ) : AcEditorReactor(), mbAutoInitAndRelease( autoInitAndRelease )
{
    if( autoInitAndRelease )
    {
        if( acedEditor )
        {
            //acutPrintf(_T("\nLinkedGE_EditorReactor : %ld"), (long)acedEditor);
            acedEditor->addReactor ( this ) ;
        }
        else
            mbAutoInitAndRelease = false ;
    }
}

LinkedGE_EditorReactor::~LinkedGE_EditorReactor ()
{
    Detach () ;
}

void LinkedGE_EditorReactor::Attach ()
{
    Detach () ;
    if ( !mbAutoInitAndRelease )
    {
        if ( acedEditor )
        {
            acedEditor->addReactor ( this ) ;
            mbAutoInitAndRelease = true ;
        }
    }
}

void LinkedGE_EditorReactor::Detach ()
{
    if ( mbAutoInitAndRelease )
    {
        if ( acedEditor )
        {
            acedEditor->removeReactor ( this ) ;
            mbAutoInitAndRelease = false ;
        }
    }
}

AcEditor* LinkedGE_EditorReactor::Subject () const
{
    return ( acedEditor ) ;
}

bool LinkedGE_EditorReactor::IsAttached () const
{
    return ( mbAutoInitAndRelease ) ;
}

static void MergeSSet( const ads_name& sset1, ads_name& sset )
{
    long length = 0;
    acedSSLength( sset1, &length );
    for( long i = 0; i < length; i++ )
    {
        ads_name en;
        acedSSName( sset1, i, en );
        acedSSAdd( en, sset, sset );
    }
}

static void MakeSSet( ads_name& sset )
{
    // 'P'并不一定能得到上一次操作的图形
    // get entities just operated on
    ads_name sset1, sset2;
    acedSSGet( _T( "P" ), NULL, NULL, NULL, sset1 );
    acedSSGet( _T( "I" ), NULL, NULL, NULL, sset2 );

    MergeSSet( sset1, sset );
    acedSSFree( sset1 );

    MergeSSet( sset2, sset );
    acedSSFree( sset2 );
}

static void GetEdgeOnOperate1( AcDbObjectIdArray& objIds )
{
    // 构造选择集
    ads_name sset;
    acedSSAdd( NULL, NULL, sset );
    MakeSSet( sset );

    long length = 0;
    acedSSLength( sset, &length );

    bool ret = true;
    for ( long i = 0; i < length; i++ )
    {
        ads_name en;
        acedSSName( sset, i, en );

        AcDbObjectId eId;
        if( acdbGetObjectId( eId, en ) != Acad::eOk )
        {
            ret = false;
            break;
        }
        if( !objIds.contains( eId ) )
        {
            objIds.append( eId );
        }
    }
    if( !ret ) objIds.removeAll();

    acedSSFree( sset );
}

static void GetEdgeOnOperate2( AcDbObjectIdArray& objIds )
{
    acedGetCurrentSelectionSet( objIds );
}

static void GetPtsOfEdgeOnOperate( const AcDbObjectIdArray& objIds, AcGePoint3dArray& pts )
{
    if( objIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for ( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj = 0;
        if( Acad::eOk == pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) )
        {
            LinkedGE* pEdge = LinkedGE::cast( pObj );
            if ( pEdge != 0 )
            {
                //acutPrintf(_T("\n分支图元id:%d需要处理..."), eId);
                AcGePoint3d startPt, endPt;
                pEdge->getSEPoint( startPt, endPt );
                if( !pts.contains( startPt ) ) pts.append( startPt );
                if( !pts.contains( endPt ) ) pts.append( endPt );
            }
        }
    }
    actrTransactionManager->endTransaction();
}

static void EncloseEdge( AcGePoint3dArray& pts )
{
    int len = pts.length();
    for( int i = 0; i < len; i++ )
    {
        DrawHelper::LinkedGEJunctionClosure( pts.at( i ) );
    }
}

// 需要定制的命令
static ACHAR* _cmds[] =
{
    _T( "MOVE" ),
    _T( "ROTATE" ),
    _T( "SCALE" ),
    _T( "STRECTCH" ),
    _T( "GRIP_STRETCH" )
};

void LinkedGE_EditorReactor::commandEnded( const ACHAR* cmdStr )
{
    // If AutoCAD is shutting down, then do nothing.
    if ( !acdbHostApplicationServices()->workingDatabase() )
        return;

    AcStringArray cmds;
    int len = sizeof( _cmds ) / sizeof( _cmds[0] );
    for( int i = 0; i < len; i++ )
    {
        cmds.append( _cmds[i] );
    }

    if( cmds.contains( cmdStr ) )
    {
        //acutPrintf(_T("\n有效的命令执行: %s"), cmdStr);
        AcDbObjectIdArray objIds;
        GetEdgeOnOperate1( objIds );
        //GetEdgeOnOperate2(objIds);

        AcGePoint3dArray pts;
        GetPtsOfEdgeOnOperate( objIds, pts );
        EncloseEdge( pts );
    }

    AcEditorReactor::commandEnded ( cmdStr ) ;
}