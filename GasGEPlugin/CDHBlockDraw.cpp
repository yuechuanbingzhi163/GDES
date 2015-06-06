#include "StdAfx.h"
#include "CDHBlockDraw.h"

ACRX_CONS_DEFINE_MEMBERS( CDHBlockDraw, BlockDraw, 1 )

CDHBlockDraw::CDHBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "见煤钻孔" );
}

void CDHBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "孔号" ) );
    names.append( _T( "地面标高" ) );
    names.append( _T( "煤层底板标高" ) );
    names.append( _T( "煤厚" ) );
}