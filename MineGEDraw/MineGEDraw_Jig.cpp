#include "StdAfx.h"
#include "MineGEDraw_Jig.h"
#include "MineGEDraw.h"

MineGEDraw_Jig::MineGEDraw_Jig( const CString& geType, const CString& drawName )
    : m_geType( geType ), m_drawName( drawName )
{
}

Adesk::Boolean MineGEDraw_Jig::startJig( MineGEDraw* pMineGEDraw )
{
    return doJig( pMineGEDraw );
}