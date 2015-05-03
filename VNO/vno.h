#ifndef VNO_OPTIMIZATION_H
#define VNO_OPTIMIZATION_H

/**
 * 本文件中除了ifq()函数以外,其它函数的vnet参数都代表的是单一源汇网络.
 * 因此,在调用这些函数前,需要人为的调用vnet->addVirtualST和vnet->delVirtualST
 * 增删虚拟源汇,将网络转换成单一源汇网络!!!
 * @ref VentNetwork::addVirtualST VentNetwork::delVirtualST
 */

//通风网络
#include "VentNetwork.h"

/**
 * 分配固定风量(Initialize Fixed Q).
 * 判断固定风量能否分配下去;如果能分配,则初始化一套风量方案.
 * @param vnet [in/out] 通风网络.
 * @return 分配成功与否.
 * @note 在迭代过程中其实不需要初始化风量,风量初始化只需要在迭代前执行一次即可\n
 *       目前初始化风量的方法有2种:(1)固定风量最大流法; (2)余枝分配法.\n
 *       "余枝分支法"是传统风网解算方法中常用的风量分配方法,但无法解决有固定风量的风量分配问题.\n
 *       本程序采用的是"最大流方法".
 */
extern bool ifq(VentNetwork* vnet);

/**
 * 通风网络解算(Ventilation Network Optimization).
 * @param  vnet [in/out] 单一源汇通风网络(连通且无单向回路).
 * @return  返回值,表示解算是否成功.
 * @note 如果分支上没有风机,则fd[e]=0;
 * @note 在调用函数前,必须将网络转换成单一源汇网络!!!
 * @code
 * 使用示例:
 *   VentNetwork* vnet;
 *   //创建通风网络并初始化
 *   //do something  
 *   //添加虚拟源汇,将网络变成单一源汇网络
 *   vnet->addVirtualST();
 *   //调用VNO模块
 *   bool ret=vno(vnet);
 *   //删除虚拟源汇
 *   vnet->delVirtualST();
 *  @endcode
 */
extern bool vno(VentNetwork* vnet);

/**
 * 求两点之间的一条最大阻力路线(Max Resistance Path).
 * 使用"A* + K最短路径算法"实现.
 * @param  vnet       [in]  单一源汇通风网络(连通且无单向回路).
 * @param  arc_filter [in]  分支过滤标记(arc_filter[e]=false,则分支e被过滤掉).
 * @param  s          [in]  始节点(如果s=INVALID,那么默认始节点为虚拟的源点).
 * @param  t          [in]  末节点(如果t=INVALID,那么默认末节点为虚拟的汇点).
 * @param  p          [out] 最大阻力路线上的所有分支.
 * @return 返回值,求最大阻力路线是否成功.
 * @note 在调用函数前,必须将网络转换成单一源汇网络!!!
 * @note 最大阻力路线内部要计算分支的阻力(不仅仅只有摩擦阻力,包括调节风阻产生的阻力)
 */
extern bool mrp(VentNetwork* vnet, ArcFilter& arc_filter, Digraph::Node s, Digraph::Node t, EdgeArray& p);

/**
 * 求两点之间的一条k最大阻力路线(Max Resistance Path).
 * 使用"A* + K最短路径算法"实现.
 * @param  vnet       [in]  单一源汇通风网络(连通且无单向回路).
 * @param  arc_filter [in]  分支过滤标记(arc_filter[e]=false,则分支e被过滤掉).
 * @param  s          [in]  始节点(如果s=INVALID,那么默认始节点为虚拟的源点).
 * @param  t          [in]  末节点(如果t=INVALID,那么默认末节点为虚拟的汇点).
 * @param  k          [in]  第k最大阻力路线.
 * @param  p          [out] 第k最大阻力路线上的所有分支
 * @return 返回值,求最大阻力路线是否成功
 * @note 最大阻力路线称为第1最大阻力路线,比最大阻力路线小一点的称为次最大阻力路线
 * @note 该算法可选择第2最大阻力路线,第3最大阻力路线....依次类推
 * @note 该算法只是一个副产品,最初我理解的通路法调节是一次性把第1最大阻力路线、第2最大阻力路线等等全\n
 *       找出来,然后依次进行调节;而实际的通路法调节不是这回事,调节前是将已搜索的最大阻力路线上的分支\n
 *       着色后再找最大阻力路线(也就是所谓的"次最大阻力路线"),然后依次进行调节.
 * @note 在调用函数前,必须将网络转换成单一源汇网络!!!
 */
extern bool mrp_k(VentNetwork* vnet, ArcFilter& arc_filter, Digraph::Node s, Digraph::Node t, int k, EdgeArray& p);

/**
 * 搜索一台风机的前n条满足风压调节量范围要求的次最大阻力路线.
 * @param  vnet      [in]  单一源汇通风网络(连通且无单向回路).
 * @param  arc_filter [in]  分支过滤标记(arc_filter[e]=false,则分支e被过滤掉).
 * @param  fan       [in]  一台风机所在的分支
 * @param  pathNums  [out] 记录每条次最大阻力路线的分支个数.
 * @param  allPath   [out] 记录所有的次最大阻力路线(用1维数组表示).
 * @param  n         [in]  次最大阻力路线的个数(如果k小于等于0,函数内部默认k=1).
 * @param minDeltaH  [in]  次最大阻力路线与最大阻力路线的风压调节量下限值!(默认10.0Pa)
 * @param maxDeltaH  [in]  次最大阻力路线与最大阻力路线的风压调节量上限值!(默认10000.0Pa)
 * @return 搜索是否成功.
 * @note 通路法优化调节的核心就是搜索可以调节的次最大阻力路线!\n
 *       基本过程:\n
 *            (1)调用mrp()搜索一条最大阻力路线;
 *            (2)重复调用apm_mrp()函数查找k条满足要求的次最大阻力路线;
 *            (3)查找阻力路线上的所有可调分支;
 *            (4)用户交互过程,选择一条可调分支;
 *            (5)调节用户选择的分支(压差加到分支的adjust_h上,同步更新分支的adjust_r;
 *            (6)该条次最大阻力路线上的所有分支设置为不可调;
 *			  (7)重复(2)~(6)步骤.
 * @note 在调用函数前,必须将网络转换成单一源汇网络!!!
 */
extern bool apm_mrp(VentNetwork* vnet, ArcFilter& arc_filter, Digraph::Arc fan, 
					IntArray& pathNums, EdgeArray& allPath,
					int n = 1, double minDeltaH = 10.0, double maxDeltaH = 10000.0);

/**
 * dfs查找所有路径.
 */
extern bool dfs_all_path(Digraph& dg, ArcFilter& arc_filter, Digraph::Node s, Digraph::Node t, IntArray& pathNums, EdgeArray& allPath);

enum GasTubeType
{
	GTT_NEGATIVE = 0,    /**< 连接泵的负压管. */
	GTT_POSITIVE = 1     /**< 连接泵的正压管. */
};
typedef Digraph::ArcMap<GasTubeType> GasTubeTypeMap;

/**
 * 划分正负压管路.
 * @param dg 网络
 * @param arc_filter 过滤部分分支
 * @param 
 */
extern bool partition_gas_tubes(Digraph& dg, ArcFilter& arc_filter, NodeArray& pumps, GasTubeTypeMap& gtt);

#endif // VNO_OPTIMIZATION_H