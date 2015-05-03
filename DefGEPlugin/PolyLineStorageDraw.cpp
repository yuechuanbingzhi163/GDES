#include "StdAfx.h"
#include "PolyLineStorageDraw.h"

ACRX_CONS_DEFINE_MEMBERS ( PolyLinePowderStorageDraw, PolyLineStorageDrawBase, 1 )
ACRX_CONS_DEFINE_MEMBERS ( PolyLineMachineRoomDraw, PolyLineStorageDrawBase, 1 )
ACRX_CONS_DEFINE_MEMBERS ( PolyLineChargeRoomDraw, PolyLineStorageDrawBase, 1 )

PolyLinePowderStorageDraw::PolyLinePowderStorageDraw( void ) : PolyLineStorageDrawBase( _T( "±¬Õ¨²ÄÁÏ¿â" ) )
{
}

PolyLineMachineRoomDraw::PolyLineMachineRoomDraw( void ) : PolyLineStorageDrawBase( _T( "»úµçíÏÊÒ" ) )
{
}

PolyLineChargeRoomDraw::PolyLineChargeRoomDraw( void ) : PolyLineStorageDrawBase( _T( "³äµçÊÒ" ) )
{
}
