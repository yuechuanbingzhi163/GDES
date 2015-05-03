#include "StdAfx.h"

#include "GraphAlgo.h"
//网络解算代码测试头文件
#include "DfsNetWork.h"
#include "../ArxHelper/ArxUtilHelper.h"
#include "../ArxHelper/ArxEntityHelper.h"

static void SetGasTubeColor(AcDbObjectIdArray objIds)
{
	// 记录原来的颜色
	AcArray<Adesk::UInt16> colors;
	if( !ArxEntityHelper::GetEntitiesColor( objIds, colors ) ) return;

	// 高亮显示标签图元
	ArxEntityHelper::SetEntitiesColor( objIds, 4 );

	// 中断
	ArxUtilHelper::Pause();

	// 恢复原有颜色
	ArxEntityHelper::SetEntitiesColor2( objIds, colors );

	//ArxEntityHelper::SetEntitiesColor( objIds, 4 ); // 蓝色高亮显示路径

}

static void DealPaths(DfsNetWork& dn, IntArray& pathNums, EdgeArray& allPath)
{
	int st = 0;
	for(int i=0;i<pathNums.size();i++)
	{
		//第i条路径的分支个数
		int n = pathNums[i];
		//acutPrintf(_T("\n第%d条路径的分支数:%d"),i+1,n);
		//第i条路径
		EdgeArray p;
		//从allPath中复制到数组p中
		std::copy(allPath.begin()+st, allPath.begin()+st+n, std::back_inserter(p));
		//下一条路径的开始位置
		st += n;
		//GetObjIdsByArc(dn,objIds,p);
		AcDbObjectIdArray objIds;
		int len = p.size();
		for( int i = 0; i < len; i++ )
		{
			objIds.append( dn.getIdByEdge( p[i] ) );
		}
		if(!objIds.isEmpty())
		{
			SetGasTubeColor(objIds);
		}
	}
}

void GraphAlgo::FindGasTubePaths(const AcDbObjectId& sObjId, const AcDbObjectId& tObjId, AcDbIntArray& pathNums, AcDbObjectIdArray& allPaths)
{
	DfsNetWork dn;
	//acutPrintf(_T("\n开始搜索路径....始点:%ld, 末点:%ld"), sObjId.asOldId(), tObjId.asOldId());
	
	Digraph::Arc se = dn.getEdgeById( sObjId );
	Digraph::Arc te = dn.getEdgeById( tObjId );

	//acutPrintf(_T("\n开始搜索路径....始点分支内部编号:%d"), se == INVALID ? -1 : Digraph::id(se));
	//acutPrintf(_T("\n开始搜索路径....末点分支内部编号:%d"), te == INVALID ? -1 : Digraph::id(te));

	IntArray g_pathNums;
	EdgeArray g_allPaths;
	dn.getAllPath(dn.getEdgeById( sObjId ), dn.getEdgeById( tObjId ), g_pathNums, g_allPaths);

	//将lemon分支转换成对应的objectid
	//acutPrintf(_T("\n路径个数:%d\n"),g_pathNums.size());
	if(!g_pathNums.empty())
	{
		for(int i=0;i<g_pathNums.size();i++)
		{
			//acutPrintf(_T("%d "), g_pathNums[i]);
			pathNums.append(g_pathNums[i]);
		}
		//acutPrintf(_T("\n"));
		for(int i=0;i<g_allPaths.size();i++)
		{
			allPaths.append(dn.getIdByEdge(g_allPaths[i]));
		}
	}
}