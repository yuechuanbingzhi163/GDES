#include "StdAfx.h"
#include "GPPBlockDraw.h"

ACRX_CONS_DEFINE_MEMBERS( GPPBlockDraw, BlockDraw, 1 )

GPPBlockDraw::GPPBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "瓦斯压力点" );
}

void GPPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "瓦斯压力值" ) );
    names.append( _T( "测点标高" ) );
    names.append( _T( "埋深" ) );
}