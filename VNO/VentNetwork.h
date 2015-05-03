#ifndef VENT_NETWORK_H
#define VENT_NETWORK_H

//图数据结构声明和定义
#include "Graph.h"
//节点/分支/构筑物/风机等数据类的声明和定义
#include "GraphData.h"
//图数据结构相关的辅助方法类
#include "GraphUtils.h"

/** 通风网络. */
class  VentNetwork
{
public:
	/**
	 * 构造函数.
	 */
	VentNetwork();
	/**
	 * 析构函数.
	 */
	~VentNetwork();

	/** 
	 * 通风网络的有向图表示.
	 * @return 有向图.
	 */
	Digraph& graph();

	/** 
	 * 返回虚拟的源点.
	 * @return 虚拟的源点.
	 */
	Digraph::Node vSource() const;

	/** 
	 * 返回虚拟的汇点.
	 * @return 虚拟的汇点.
	 */
	Digraph::Node vTarget() const;

	/** 
	 * 返回虚拟的大气分支.
	 * @return 虚拟的大气分支.
	 */
	Digraph::Arc vAir() const;

	/**
	 * 获取分支的属性数据.
	 * @param e [in] 分支.
	 * @return 分支的属性数据对象.
	 */
	EdgeData*& edge(Digraph::Arc e);

	/**
	 * 获取分支上的风机数据.
	 * @param e [in] 分支.
	 * @return 分支上的风机数据对象.
	 */
	FanData*& fan(Digraph::Arc e);

	/**
	 * 获取节点的属性数据.
	 * @param u [in] 节点.
	 * @return 节点的属性数据对象.
	 */
	NodeData*& node(Digraph::Node u);

	/**
	 * 获取分支上的构筑物数据.
	 * @param e [in] 分支.
	 * @return 分支上的构筑物数据对象.
	 */
	GateData*& gate(Digraph::Arc e);

	/**
	 * 添加虚拟的源汇,使通风网络变成单一源汇网络.
	 * 新增一个虚拟的源点和汇点,虚拟源点到原有网络源点之间的虚拟分支,
	 * 原有网络汇点到虚拟汇点之间的分支以及这些虚拟节点和分支关联的数据.
	 * @note 虚拟分支上没有风机动力(fd[e]=0).
	 */
	void addVirtualST();

	/**
	 * 删除源点sn和汇点tn以及关联的数据(节点数据/分支数据).
	 */
	void delVirtualST();

	/**
	 * 添加虚拟大气分支.
	 * 该方法会增加一条从虚拟汇点指向虚拟源点的分支,将通风网络变成无源汇网络.
	 * @note 在调用addVirtualST()方法添加虚拟源汇之后才能调用该方法.
	 */
	void addAirEdge();

	/** 
	 * 删除虚拟大气分支.
	 */
	void delAirEdge();

	/**
	 * 计算节点u的出边分支的风量之和.
	 * @param u  [in] 节点.
	 * @return 节点u的出边分支流量之和.
	 */
	double outArcFlow(Digraph::Node u) const;

	/**
	 * 计算节点u的入边分支的风量之和.
	 * @param  u [in] 节点.
	 * @return 节点u的入编分支流量之和.
	 */
	double inArcFlow(Digraph::Node u) const;

	/**
	 * 统计固定风量分支的个数.
	 * @return 固定风量分支个数.
	 */
	int countFixQ() const;

	/**
	 * 查找所有的固定风量分支.
	 * @param edges [out] 固定风量分支数组.
	 */
	void fixQEdges(EdgeArray& edges) const;

	/**
	 * 查找所有的风机分支.
	 * @param edges [out] 风机所在的分支.
	 * @note 实现可能有问题,目前的方法是查找所有fd[e]不为空的分支\n
	 *       但如果网络中存在局扇呢???
	 */
	void fanEdges(EdgeArray& edges) const;

	/**
	 * 查找所有的构筑物分支.
	 * @param edges [out] 构筑物所在的分支.
	 */
	void gateEdges(EdgeArray& edges) const;

	/**
	 * 计算一条路径的总阻力.
	 * @param p [in] 一条路径.
	 * @return 路径的总阻力. 
	 */
	double hPath(EdgeArray& p) const;

	/**
	 * 调整所有的负风量分支.
	 * @note 两个步骤:(1)风量变为正数;(2)分支反向.
	 */
	void adjustNegativeEdge();

	/**
	 * 计算所有节点的压力.
	 */
	void caclNodePressures();

	/**
	 * 查找路径上的可调分支.
	 * @param p           [in]  一条路径.
	 * @param adjustEdges [out] 可调分支数组.
	 * @note 该函数的实现还有待商榷!
	 */
	void adjustableEdges(const EdgeArray& p, EdgeArray& adjustEdges);

	/**
	 * 过滤路径上的虚拟分支.
	 * @param pp [in]  一条路径.
	 * @param p  [out] 过滤后的路径.
	 */
	void filterVirutalEdges(const EdgeArray& pp, EdgeArray& p);

private:
	Digraph dg;                  /**< 有向图.   */
	EdgeMap ed;              /**< 分支数据. */
	FanMap fd;               /**< 风机数据. */
	NodeMap nd;              /**< 节点数据. */
	GateMap gd;              /**< 构筑物数据(未使用!). */
	Digraph::Node sn, tn;        /**< 虚拟源汇. */
	Digraph::Arc airEdge;        /**< 大气分支. */
};

#endif // VENT_NETWORK_H