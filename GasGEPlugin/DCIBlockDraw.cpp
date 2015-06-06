#include "StdAfx.h"
#include "DCIBlockDraw.h"

ACRX_CONS_DEFINE_MEMBERS( CSDCIBlockDraw, BlockDraw, 1 )
ACRX_CONS_DEFINE_MEMBERS( WDCI1BlockDraw, BlockDraw, 1 )
ACRX_CONS_DEFINE_MEMBERS( WDCI2BlockDraw, BlockDraw, 1 )

CSDCIBlockDraw::CSDCIBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "煤层区域突出危险性预测指标" );
}

void CSDCIBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "瓦斯放散初速度" ) );
    names.append( _T( "煤的坚固性系数" ) );
    // 实际只有2个字段，该字段并不存在!!!
    // 注册一个不存在的字段，返回值为""
    names.append( _T( "瓦斯突出危险性综合指标K值" ) );
}

// 删除尾部多余的0
// 只适用于浮点数字符串，必须有小数点.
// 例如123.45000
//static CString RemoveExtraZero(const CString& str)
//{
//	if(str.Right(1) = _T("0"))
//	{
//		return str.Left(str.ReverseFind(_T('0')));
//	}
//	else
//	{
//		return str;
//	}
//}

void CSDCIBlockDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    BlockDraw::readPropertyDataFromGE( values );

    // 最后 一个数据并不不是从字段中得到，需要计算
    double v = _tstof( values[0].kACharPtr() );
    double f = _tstof( values[1].kACharPtr() );
    acutPrintf( _T( "\n计算的f=%.3f" ), f );
    if( abs( f ) > 0 )
    {
        double k = v / f;
        CString str;
        str.Format( _T( "%.3f" ), k );
        //m_attValues[2] = RemoveExtraZero(str);
        m_attValues[2] = str;
    }
}

WDCI1BlockDraw::WDCI1BlockDraw () : BlockDraw ()
{
    m_blockName = _T( "工作面突出危险性预测指标1" );
}

void WDCI1BlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "钻屑解吸指标" ) );
    names.append( _T( "钻孔最大钻屑量" ) );
}

WDCI2BlockDraw::WDCI2BlockDraw () : BlockDraw ()
{
    m_blockName = _T( "工作面突出危险性预测指标2" );
}

void WDCI2BlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "钻孔最大瓦斯涌出初速度" ) );
    names.append( _T( "钻孔最大钻屑量" ) );
    names.append( _T( "R值指标" ) );
}