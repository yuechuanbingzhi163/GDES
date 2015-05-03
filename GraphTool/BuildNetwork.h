#pragma once

#include "dlimexp.h"

#undef OUT
#undef IN

#include "../GraphDef.h"
#include <vector>

typedef std::vector<Digraph::Arc> EdgeArray;
typedef std::vector<Digraph::Node> NodeArray;

typedef Digraph::ArcMap<bool> ArcFilter;
typedef Digraph::NodeMap<bool> NodeFilter;

// 分支对应的实体对象id
typedef Digraph::ArcMap<AcDbObjectId> ObjectIdMap;

// 节点坐标
typedef Digraph::NodeMap<AcGePoint3d> NodeGeoDataMap;

/*
 *	创建网络
 *  同时额外处理掘进工作面(增加虚拟分支)
 */
extern "C" GRAPH_EXPORT_API bool BuildNetwork(Digraph& dg, ObjectIdMap& om);

/*
 * 处理多源多汇
 * 添加一个虚拟源点和虚拟汇点
 * 在虚拟源点和源点之间添加虚拟分支
 * 汇点也是同样处理
 * 虚拟分支的id为AcDbObjectId::kNull
 */
extern "C" GRAPH_EXPORT_API bool AddVirtualSTNode(Digraph& dg, ObjectIdMap& om, Digraph::Node& sn, Digraph::Node& tn);

/*
 * 过滤包含阻断构筑物的分支
 * block filter
 */
extern "C" GRAPH_EXPORT_API void FilterBlockEdges(Digraph& dg, ObjectIdMap& om, ArcFilter& bf);

/*
 * 删除包含阻断构筑物的分支
 * 注：删除阻断分支，可能会额外增加源点或汇点
 */
extern "C" GRAPH_EXPORT_API void EraseBlockEdges(Digraph& dg, ArcFilter& bf);

/* 
 * 获取节点和分支的几何坐标
 * 注：没有处理虚拟分支
 *     如果调用了DealWithSourceTarget方法，
 *     那么虚拟源点和汇点的坐标默认为原点(0,0,0)
 */
extern "C" GRAPH_EXPORT_API bool GetNodeGeoData(Digraph& dg, ObjectIdMap& om, NodeGeoDataMap& ngm);