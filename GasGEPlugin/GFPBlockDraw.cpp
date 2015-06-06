#include "StdAfx.h"
#include "GFPBlockDraw.h"

ACRX_CONS_DEFINE_MEMBERS( WGFPBlockDraw, BlockDraw, 1 )
ACRX_CONS_DEFINE_MEMBERS( TGFPBlockDraw, BlockDraw, 1 )

WGFPBlockDraw::WGFPBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "回采工作面瓦斯涌出量点" );
}

void WGFPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "绝对瓦斯涌出量" ) );
    names.append( _T( "相对瓦斯涌出量" ) );
    names.append( _T( "工作面日产量" ) );
    names.append( _T( "回采年月" ) );
}

TGFPBlockDraw::TGFPBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "掘进工作面绝对瓦斯涌出量点" );
}

void TGFPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "绝对瓦斯涌出量" ) );
    names.append( _T( "掘进年月" ) );
}