#include "StdAfx.h"
#include "SelectedGE_EditorReactor.h"

#include "DataInput_DockBar.h"
extern DataInput_DockBar* pDataInput_DockBar;

#include "../ArxHelper/HelperClass.h"
#include "../MineGE/DataHelper.h"

SelectedGE_EditorReactor::SelectedGE_EditorReactor ( const bool autoInitAndRelease )
    : AcEditorReactor(), mbAutoInitAndRelease( autoInitAndRelease )
{  
	if( autoInitAndRelease )
    {
        if ( acedEditor )
            acedEditor->addReactor ( this ) ;
        else
            mbAutoInitAndRelease = false ;
    }
}

SelectedGE_EditorReactor::~SelectedGE_EditorReactor ()
{
    Detach () ;
}

void SelectedGE_EditorReactor::Attach ()
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

void SelectedGE_EditorReactor::Detach ()
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

AcEditor* SelectedGE_EditorReactor::Subject () const
{
    return ( acedEditor ) ;
}

bool SelectedGE_EditorReactor::IsAttached () const
{
    return ( mbAutoInitAndRelease ) ;
}

static void GetDataInputType( const AcDbObjectId& objId, DI_TYPE& di )
{
    if( objId.isNull() )
    {
        di = DI_HELP;

    }
	//else if (ArxUtilHelper::IsEqualType( _T( "Tunnel" ), objId ) )
	//{
	//	di = DI_TUNNEL;
	//}

	//else if (ArxUtilHelper::IsEqualType( _T( "TTunnel" ), objId ) )
	//{
	//	di = DI_TUNNEL;
	//}

	//else if (ArxUtilHelper::IsEqualType( _T( "Chimney" ), objId ) )
	//{
	//	di = DI_TUNNEL;
	//}

	//else if (ArxUtilHelper::IsEqualType( _T( "LocalFan" ), objId ) )
	//{
	//	di = DI_TUNNEL;
	//}

	//else if (ArxUtilHelper::IsEqualType( _T( "WindLibrary" ), objId ) )
	//{
	//	di = DI_TUNNEL;
	//}

    else
    {
        di = DI_TUNNEL;
    }
}

void SelectedGE_EditorReactor::pickfirstModified( void )
{
    //acutPrintf(_T("\nVentEvaluate::pickfirstModified()...\n"));

    if( pDataInput_DockBar == 0 ) return;

    DI_TYPE di;
    AcDbObjectId objId;

    if( pDataInput_DockBar->IsWindowVisible() )
    {
        AcDbObjectIdArray objIds;
        ArxUtilHelper::GetPickSetEntity( objIds );
        if( objIds.length() == 1 )
        {
            objId = objIds[0];
        }
    }
    GetDataInputType( objId, di );

    CAcModuleResourceOverride resourceOverride;
	AcDbObjectId dbObjId;
	DataHelper::GetDataObject(objId, dbObjId);
    pDataInput_DockBar->update( di, dbObjId );
    //AcEditorReactor::pickfirstModified();
}
