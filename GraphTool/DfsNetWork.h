#pragma once

#include "BuildVentNetwork.h"
#include "VentEdgeData.h"

typedef Digraph::ArcMap<VentEdgeData*> DFS_EdgeDataMap;

class DfsNetWork
{
public:
	DfsNetWork(void);
	~DfsNetWork(void);

	Digraph::Arc getEdgeById(const AcDbObjectId& objId);       // 根据id查找分支
	AcDbObjectId getIdByEdge(Digraph::Arc e);                  // 根据分支查找id

	void doDFS(Digraph::Arc se, Digraph::Arc te, EdgeArray& p);
	void getAllPath(Digraph::Arc se,Digraph::Arc te,IntArray& pathNums,EdgeArray& allPath);

private:
	bool initNetwok();	                 // 构建网络并提取图元属性数据
	bool isInitNetworkOk() const;        // 初始化网络是否成功
	void setInitNetwokState(bool bState);// 设置网络初始化状态(成功/失败)
	void initAirEdges();                 // 初始化用风地点分支

private:
	Digraph dg;                       // 通风网络(有向图)
	ArcFilter ef;                     // 阻断分支过滤
	Digraph::Node sn, tn;             // 网络虚拟源点和汇点
	//NodeGeoDataMap ngm;

	DFS_EdgeDataMap datas;         // 分支的数据(分支的通风类型，非字段)
	EdgeArray airEdges;                 // 用风地点分支集合
	bool bInitNetwokState;            // 网络初始化是否成功(默认成功)
};
