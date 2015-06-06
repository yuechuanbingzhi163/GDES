#include "StdAfx.h"
#include "GCPBlockDraw.h"

ACRX_CONS_DEFINE_MEMBERS( GCPBlockDraw, BlockDraw, 1 )

GCPBlockDraw::GCPBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "瓦斯含量点" );
}

void GCPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "瓦斯含量" ) );
    names.append( _T( "测点标高" ) );
    names.append( _T( "埋深" ) );
}