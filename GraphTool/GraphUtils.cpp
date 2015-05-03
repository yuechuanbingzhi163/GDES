#include "StdAfx.h"

#include <cmath>
#include <cfloat>
#include <climits>
#include <iostream>
#include "GraphUtils.h"

//从文件中读取数据,包括拓扑数据, 巷道实际风阻,风量;通风系统总风量
static bool ReadTopologyDatas( std::istream& stream, IntArray& E, IntArray& U, IntArray& V)
{
	/* 读取分支数 */
	int n = 0;
	stream>>n;

	/* 读取网络拓扑和初始分支风阻 */
	for(int i=0;i<n;i++)
	{
		int e, u, v;
		//double r, rmin, rmax;
		stream >> e >> u >> v;

		//if(stream.fail()) break;

		E.push_back(e);
		U.push_back(u);
		V.push_back(v);
	}

	return true;
}

static void BuildNodesMap(const IntArray& nodes, Digraph& dg, IntNodeMap& nodes_map)
{
	for(unsigned int i=0;i<nodes.size();i++)
	{
		if(nodes_map.find(nodes[i]) == nodes_map.end())
		{
			nodes_map.insert(IntNodeMap::value_type(nodes[i], dg.addNode()));
		}
	}
}

static void BuildEdgesMap(const IntArray& E, const IntArray& U, const IntArray& V,
						  Digraph& dg, IntNodeMap& nodes_map, IntArcMap& edges_map)
{
	for(unsigned int i=0;i<E.size();i++)
	{
		if(edges_map.find(E[i]) == edges_map.end())
		{
			Digraph::Node u = nodes_map[U[i]];
			Digraph::Node v = nodes_map[V[i]];
			Digraph::Arc e = dg.addArc(u, v);
			edges_map.insert(IntArcMap::value_type(E[i], e));
		}
	}
}

static void InitVNR_IdMap(
						  Digraph& dg, IntNodeMap& nodes_map, IntArcMap& edges_map,
						  NodeIdMap& nid, EdgeIdMap& eid)
{
	for(IntNodeMap::iterator itr=nodes_map.begin();itr!=nodes_map.end();++itr)
	{
		nid[itr->second]=itr->first;
	}
	for(IntArcMap::iterator itr=edges_map.begin();itr!=edges_map.end();++itr)
	{
		eid[itr->second]=itr->first;
	}
}

static void BuildNetworkMap(const IntArray& E, const IntArray& U, const IntArray& V,
							Digraph& dg, IntNodeMap& nodes_map, IntArcMap& edges_map)
{
	// 建立节点编号-->节点映射
	BuildNodesMap(U, dg, nodes_map);
	BuildNodesMap(V, dg, nodes_map);

	// 建立分支编号-->分支映射
	BuildEdgesMap(E, U, V, dg, nodes_map, edges_map);

	// 删除度为0的节点
	GraphUtils::EraseZeroDegreeNodes(dg);
}

bool GraphUtils::CreateGraphFromTopology( std::istream& stream, Digraph& dg, EdgeIdMap& eid, NodeIdMap& nid)
{
	//读取拓扑数据
	IntArray E, U, V;
	if(!ReadTopologyDatas(stream, E, U, V)) return false;

	// 构建编号到网络分支和节点的映射关系
	// 可能存在分支和节点编号重复的情况,需要剔除掉
	IntNodeMap nodes_map;
	IntArcMap edges_map;
	BuildNetworkMap(E, U, V, dg, nodes_map, edges_map);
	InitVNR_IdMap(dg, nodes_map, edges_map, nid ,eid);
	return true;
}

typedef Digraph::ArcMap<double> Weight;
typedef Digraph::ArcMap<double> Capacity;
typedef Digraph::ArcMap<double> Flow;

//有上下界的最大流
static bool MaxFlow_Bound(Digraph& dg, ArcFilter& arc_filter, Digraph::Node s, Digraph::Node t, double Q, 
						  Capacity& low, Capacity& high, Flow& flow, bool fixTotalQ=false)
{
	typedef FilterArcs<Digraph, ArcFilter> Adaptor;
	typedef Adaptor::ArcMap<double> CapMap;
	typedef Adaptor::ArcMap<Digraph::Arc> LowUpArcMap;
	typedef Preflow<Adaptor,CapMap> FastPreFlow;

	//过滤后的图
	Adaptor adaptor(dg, arc_filter);

	//在源点s之前再添加一条分支,用来限制总流量Q
	Digraph::Node s0 = adaptor.addNode();
	Digraph::Arc e0 = adaptor.addArc(s0, s);
	low[e0] = (fixTotalQ?Q:0); high[e0] = Q; arc_filter[e0] = true; // 注:ArcFilter默认false

	//添加一条t->s的分支,下界0,上界无穷大
	Digraph::Arc ts = adaptor.addArc(t, s0);
	low[ts] = 0; high[ts] = DBL_MAX; arc_filter[ts] = true;

	//分支容量(默认无穷大)
	CapMap cap(adaptor, DBL_MAX);
	//上界分支拆分后的分支映射记录
	//e --> e=(u->v), ee1=(ss->v), e22=(u->tt)
	//lu[ee1] = e; lu[ee2] = e;
	LowUpArcMap lu(adaptor, INVALID);

	//添加超级源汇
	Digraph::Node ss = adaptor.addNode();
	Digraph::Node tt = adaptor.addNode();

	//注:后续新增的节点和分支属性map不会自动更新,需要手动设置
	//例如cap[e0]、cap[ts]、arc_filter、lu等
	//为了减少重复代码,将属性初始化声明转移到添加节点和分支之后
	//拆分有上下界的分支,其它分支上界默认无穷大
	for(Adaptor::ArcIt e(adaptor);e!=INVALID;++e)
	{
		if(low[e] > 0 && high[e] >= low[e])
		{
			Digraph::Node u = adaptor.source(e);
			Digraph::Node v = adaptor.target(e);
			Digraph::Arc ee1 = adaptor.addArc(ss, v);
			Digraph::Arc ee2 = adaptor.addArc(u, tt);
			cap[e] = high[e] - low[e];
			cap[ee1] = low[e]; cap[ee2] = low[e];   // 更新cap
			low[ee1] = 0; low[ee2] = 0;             // 更新low
			lu[ee1] = e; lu[ee2] = e;               // 更新lu
			arc_filter[ee1] = true; arc_filter[ee2] = true;
		}
		//cout<<"e"<<g .id(e)<<" low:"<<low[e]<<", high:"<<high[e]<<", cap:"<<cap[e]<<endl;
	}

	//执行一次最大流计算
	FastPreFlow preflow(adaptor, cap, ss, tt);
	preflow.run();
	//最大流值
	double max_flow = preflow.flowValue();
	//计算得到的分支流量
	const FastPreFlow::FlowMap& flow_map = preflow.flowMap();

	//判断是否可行流(即额外添加的分支满流,满足下界low)
	//2种方法:1) 所有的流量的下界之和是否等于最大流  2) 所有分支的下界是否满流
	//使用第1种方法判定: 计算所有分支的下界之和
	//用于后续判断是否可行流(满足下界,且下界之和等于最大流)
	double S = 0;
	for(Adaptor::ArcIt e(adaptor);e!=INVALID;++e)
	{
		S += low[e];
	}

	const double PRE_VNC_EPSILON=0.01;
	bool ret = (fabs(S-max_flow) < PRE_VNC_EPSILON);
	if(ret)
	{
		//有上下界的最大流分配成功!!!
		//得到计算的流量值,并记录拆分的分支
		EdgeArray edges;
		for(Adaptor::ArcIt ee(adaptor);ee!=INVALID;++ee)
		{
			flow[ee] = flow_map[ee];
			if(lu[ee] != INVALID)
			{
				edges.push_back(ee);
			}
		}

		typedef std::set<Digraph::Arc> ArcSet;
		//记录已处理的分支
		ArcSet aset;
		for(EdgeArray::iterator itr=edges.begin();itr!=edges.end();++itr)
		{
			Digraph::Arc ee = *itr;
			Digraph::Arc e = lu[ee];
			// 如果分支已处理,则添加到aset集合中,避免重复流量叠加
			if(aset.find(e) == aset.end())
			{
				flow[e] = low[e] + flow_map[e];
				aset.insert(e);
			}
		}
	}
	// erase(node)函数会自动删除节点以及节点的流入流出分支
	// erase(arc)函数则只会删除分支
	adaptor.erase(ss);
	adaptor.erase(tt);
	adaptor.erase(s0);

	return ret;
}

bool GraphUtils::FixedMaxFlow(Digraph& dg, Weight& w, double Q)
{
	Digraph::Node sn, tn;
	GraphUtils::AddVirtualSTNode(dg, sn, tn);  // 处理多源多汇网络为单一源汇网络

	Capacity low(dg,0.1);
	Capacity high(dg,DBL_MAX);
	Flow flow(dg);
	// 分配固定风量
	for(Digraph::ArcIt e(dg);e!=INVALID;++e)
	{
		if(w[e] > 0)
		{
			low[e] = w[e];
			high[e] = w[e];
		}
		//cout<<low[e]<<" "<<high[e]<<endl;
	}
	//执行有上下界的最大流算法
	ArcFilter arc_filter(dg ,true);
	bool ret = MaxFlow_Bound(dg,arc_filter,sn,tn,Q,low,high,flow,false);
	if(ret)
	{
		// 固定分量可分配,则分配所有分支的风量
		for(Digraph::ArcIt e(dg);e!=INVALID;++e)
		{
			w[e] = flow[e];
			//cout<<"w(e"<<dg .id(e)<<")="<<w[e]<<endl;
		}
	}

	GraphUtils::DeleteOutArcs(dg, sn); // 删除虚拟源点的所有出边分支(虚拟分支)
	GraphUtils::DeleteInArcs(dg, tn); // 删除虚拟源点的所有入边分支(虚拟分支)
	dg.erase(sn); dg.erase(tn);                               // 删除虚拟源汇节点
	return ret;
}