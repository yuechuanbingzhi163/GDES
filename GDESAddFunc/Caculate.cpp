#include "StdAfx.h"
#include "Caculate.h"

/*
 * 参数1代表
 *	|-矿井相对瓦斯涌出量
 *	|-矿井总回风巷瓦斯浓度
 *	|-当地大气压
 *	|-矿井最大总回风风量
 *	|-抽采泵富余系数
 *	|-抽采系统工况系数
 *	|-瓦斯泵台数
 * 参数2代表单台瓦斯泵额定流量、瓦斯抽采浓度、瓦斯泵压力
 * 参数3代表计算结果
*/
bool Calculate::PumpCapacityCacul( const AcStringArray& baseDatas,const AcStringArray& pumpDatas,CString& strRet )
{

	if(baseDatas.isEmpty() || pumpDatas.isEmpty()) return false;
	return true;
}