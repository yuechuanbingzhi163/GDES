#ifndef VNO_GRAPH_DEF_h
#define VNO_GRAPH_DEF_h

#undef OUT
#undef IN

/**
 * 使用lemon库提供的数据结构定义通用的图(或网络)以及常用的算法.
 */
#include <lemon/list_graph.h>
 
/** 
 * 有向图.
 * Digraph::Arc   -- 分支(有向边)
 * Digraph::Node  -- 节点
 */
typedef lemon::ListDigraph Digraph;
//
/**
 * 分支过滤器.
 * 过滤器是lemon图论算法库的一个特色功能,通过给分支和结点附加一个bool值来过滤分支和结点
 * 例如:某个算法希望在搜索的时候不经过这些分支和结点,使用过滤器可以非常方便的排除掉这些分支和结点,
 *      并且不需要修改原有搜索算法的代码(dfs、bfs)
 * 详情参见LEMON官方教程中"Adatpro"一节以及<lemon/adators.h>头文件
 */
typedef Digraph::ArcMap<bool> ArcFilter;
//
/** 节点过滤器. */
typedef Digraph::NodeMap<bool> NodeFilter;

/** 分支权重. */
typedef Digraph::ArcMap<double> Weight;

/** 节点ID. */
typedef Digraph::NodeMap<int> NodeIdMap;

/** 分支ID. */
typedef Digraph::ArcMap<int> EdgeIdMap;

/** 父节点(前驱节点). */
typedef Digraph::NodeMap<Digraph::Node> PredMap;

/** 节点距离. */
typedef Digraph::NodeMap<double> DistMap;

/**
 * 定义常用的数组类型.
 * 使用STL提供的vector模板类定义数组,数组的大小可以任意变化
 * 可通过[]操作符访问和修改数据,详情参见STL中的vector类.
 */
#include <vector>

/** 
 * 分支数组. 
 * 在算法中会经常用到这个数据结构,比如存储搜索算法找到的一条由分支组成的路径.
 */
typedef std::vector<Digraph::Arc>    EdgeArray;
/** 节点数组. */
typedef std::vector<Digraph::Node>   NodeArray;
/** 浮点型数组. */
typedef std::vector<double>          DoubleArray;
/** 整型数组. */
typedef std::vector<int>             IntArray;

/**
 * 常用图论算法.
 * (1) 适配器：有向图-->无向图、有向图反向等等(adaptors.h)
 * (2) 搜索算法：深度优先(dfs.h)、宽度优先(bfs.h)
 * (3) 连通性算法：连通性判定、单向回路、强连通分量等等(connectivity.h)
 * (4) 最小生成树：克鲁斯卡尔算法(kruskal.h)
 * (5) 最短路径：迪杰斯特拉算法、贝尔福特曼算法等(dijkstra.h、bellman_ford.h)
 * (6) 最大流:预流算法(preflow.h,目前已知最快的最大流算法)
 * (7) 二分匹配算法:matching.h
 */
#include <lemon/adaptors.h>
#include <lemon/dfs.h>
#include <lemon/bfs.h>
#include <lemon/connectivity.h>
#include <lemon/kruskal.h>
#include <lemon/dijkstra.h>
#include <lemon/bellman_ford.h>
#include <lemon/preflow.h>
#include <lemon/matching.h>
using namespace lemon;

/**
 * STL提供的常用数据结构.
 */
#include <algorithm>
#include <iterator>
#include <stack>
#include <queue>
#include <list>
#include <set>
#include <map>

typedef std::map<int, Digraph::Arc> IntArcMap;
typedef std::map<int, Digraph::Node> IntNodeMap;

#endif // VNO_GRAPH_DEF_h
