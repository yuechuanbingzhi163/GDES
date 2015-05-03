#include "StdAfx.h"
#include "MineGEDraw.h"

// 使用ACRX_NO_CONS_DEFINE_MEMBERS宏，使得MineGEDraw不需要默认构造函数
// 也使得MineGEDraw可以成为抽象类
ACRX_NO_CONS_DEFINE_MEMBERS( MineGEDraw, AcDbEntity )

MineGEDraw::MineGEDraw()
{

}

void MineGEDraw::caclBackGroundMinPolygon( AcGePoint3dArray& pts )
{
    return;
}
