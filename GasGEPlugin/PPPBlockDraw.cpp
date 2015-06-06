#include "StdAfx.h"
#include "PPPBlockDraw.h"

ACRX_CONS_DEFINE_MEMBERS( PPPBlockDraw, BlockDraw, 1 )

PPPBlockDraw::PPPBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "动力现象点" );
}

void PPPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "突出煤岩量" ) );
    names.append( _T( "涌出瓦斯量" ) );
    names.append( _T( "标高" ) );
    names.append( _T( "发生年月" ) );
}