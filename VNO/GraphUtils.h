#ifndef VNO_GRAPH_UTILS_H
#define VNO_GRAPH_UTILS_H

#include "Graph.h"

/**
 * 图数据结构辅助方法类.
 * 该类中的大部分方法都是静态成员模板函数.
 * 模板函数的类型参数DGR泛指的是"有向图 以及 通过适配器Adaptor变换后的有向图".
 * 例如Digraph, FilterArcs<Digraph, ArcFilter>等等.
 */
class  GraphUtils
{
    public:
		/**
		 * 逆转数组中的数据(模板函数).
		 * @param con STL容器(比如vector、list)
		 */
		template<typename Container>
		static void ReverseArray(Container& con)
		{
			std::stack<typename Container::value_type> st;
			for(typename Container::iterator itr=con.begin();itr!=con.end();itr++)
			{
				st.push(*itr);
			}
			con.clear();
			while(!st.empty())
			{
				con.push_back(st.top());
				st.pop();
			}
		}

		/**
		 * 计算有向图的分支权重(C++模板函数).
		 * dg   -- 有向图
		 * w    -- 分支的权重属性数据
		 * wf   -- 权重functor
		 * @note wf既可以是全局/静态函数,也可以是重载了()运算符的类/结构体(functor)
		 * @note 这些函数必须要接收一个typename DGR::Arc参数
		 */
		template<typename DGR, typename WeightFunc>
		static void BuildWeight(DGR& dg, Weight& w, WeightFunc wf)
		{
			for(typename DGR::ArcIt e(dg); e!=INVALID; ++e)
			{
				w[e] = wf(e);
			}
		}

		/** 
		 * 逆转权重(将最长路径,最大阻力路线等问题转换成最短路径问题).
		 * 逆转方法有2种:(1)权重的正负逆转;(2)用一个较大的值减去每条分支的权重
		 * @param dg [in] 有向图
		 * @param w [in/out] 分支权重
		 */
		template<typename DGR>
		static void ReverseWeight(DGR& dg, Weight& w)
		{
			double max_w = 0;
			for(typename DGR::ArcIt e(dg);e!=INVALID;++e)
			{
				w[e] = max_w - w[e];
			}
		}

        /************ 图数据结构操作方法(不涉及节点/分支的属性数据)  ***************/
        /**
         * 删除度(degree)等于0的节点.
         * @param dg [in] 有向图
         */
		template<typename DGR>
        static void EraseZeroDegreeNodes(DGR& dg)
		{
			NodeArray ns;
			for(typename DGR::NodeIt n(dg); n!=INVALID; ++n)
			{
				if((countOutArcs(dg, n) + countInArcs(dg, n)) == 0)
				{
					ns.push_back(n);
				}
			}
			for(size_t i=0;i<ns.size();i++)
			{
				dg.erase(ns[i]); // 删除空节点(度等于0)
			}
		}

        /**
         * 修改多条分支的始节点为v.
         * @param dg [in] 有向图
         * @param es [in] 多条分支(数组)
         * @param v  [in] 节点
         * @note  EdgeArray是通过typedef定义的std::vector类型
         */
		template<typename DGR>
        static void ChangeOutArcSourceNode(DGR& dg, EdgeArray& es, Digraph::Node v)
		{
			for(size_t i=0;i<es.size();i++)
			{
				dg.changeSource(es[i], v);
			}
		}

        /**
         * 修改多条分支的末节点为v.
         * @param dg [in] 有向图
         * @param es [in] 多条分支(数组)
         * @param v  [in] 节点
         * @note  EdgeArray是通过typedef定义的std::vector类型
         */
		template<typename DGR>
        static void ChangeInArcTargetNode(DGR& dg, EdgeArray& es, Digraph::Node v)
		{
			for(int i=0;i<es.size();i++)
			{
				dg.changeTarget(es[i], v);
			}
		}

        /**
         * 分支反向.
         * @param dg [in] 有向图
         * @param e  [in] 待反向的分支
         * @note 交换分支e的始末节点
         */
		template<typename DGR>
        static void ReverseArc(DGR& dg, Digraph::Arc e)
		{
			dg.reverseArc(e);
		}

        /**
         * 查找图的所有源点(入度等于0的节点).
         * @param dg [in] 有向图
         * @param nodes [out] 源点
         */
		template<typename DGR>
        static void FindSourceNodes(DGR& dg, NodeArray& nodes)
		{
			// 对网络进行分析,查找源点
			for(typename DGR::NodeIt n(dg); n!=INVALID; ++n)
			{
				// 记录入度=0的源点
				if(0 == countInArcs(dg, n))
				{
					nodes.push_back(n);
				}
			}
		}

        /**
         * 查找图的所有汇点(出度等于0的节点).
         * @param dg [in] 有向图
         * @param nodes [out] 汇点
         */
		template<typename DGR>
        static void FindTargetNodes(DGR& dg, NodeArray& nodes)
		{
			// 对网络进行分析,查找汇点
			for(typename DGR::NodeIt n(dg); n!=INVALID; ++n)
			{
				// 记录出度=0的汇点
				if(0 == countOutArcs(dg, n))
				{
					nodes.push_back(n);
				}
			}
		}

        /**
         * 向图中添加一个虚拟的源点.
         * @param dg [in] 有向图
         * @return 虚拟源点
         * @note 查找图中的所有源点(入度等于0的节点),添加一个虚拟的节点s,并添加相应的虚拟分支(以虚拟节点s为始节点,原图的源点作为末节点)
         */
		template<typename DGR>
        static Digraph::Node AddVirtualSource(DGR& dg)
		{
			NodeArray sNodes;  // 源点集合
			FindSourceNodes(dg, sNodes);
			/*
			* !!!前提条件!!!
			* 至少有一个源点和一个汇点
			* 目前暂时不考虑其它的"不正常情况"
			* 例如：没有源点或没有汇点,或者源汇点都没有
			*/
			if(sNodes.empty()) return INVALID;

			// 增加一个虚拟源点
			typename DGR::Node sn = dg.addNode();

			// 增加源点"流出分支"(虚拟源点-->源点)
			for(size_t i=0;i<sNodes.size();i++)
			{
				typename DGR::Arc e = dg.addArc(sn, sNodes[i]);
			}
			return sn;  // 返回新增的虚拟源点
		}

        /**
         * 向图中添加一个虚拟的汇点.
         * @param dg [in] 有向图
         * @return 虚拟汇点
         * @note 查找图中的所有汇点(出度等于0的节点),添加一个虚拟的节点t,并添加相应的虚拟分支(以原图的汇点作为始节点,虚拟节点t作为末节点)
         */
		template<typename DGR>
        static Digraph::Node AddVirtualTarget(DGR& dg)
		{
			NodeArray tNodes;  // 源点集合
			FindTargetNodes(dg, tNodes);

			/*
			* !!!前提条件!!!
			* 至少有一个源点和一个汇点
			* 目前暂时不考虑其它的"不正常情况"
			* 例如：没有源点或没有汇点,或者源汇点都没有
			*/

			if(tNodes.empty()) return INVALID;

			// 增加一个虚拟汇点
			typename DGR::Node tn = dg.addNode();

			// 增加汇点"流入分支"(汇点-->虚拟汇点)
			for(size_t i=0;i<tNodes.size();i++)
			{
				typename DGR::Arc e = dg.addArc(tNodes[i], tn);
			}
			return tn; // 返回新增的虚拟汇点
		}

        /**
         * 获取节点u的所有出边分支.
         * @param dg [in] 有向图
         * @param u  [in] 节点
         * @param es [out] 节点u的所有出边(es是一个动态数组)
         */
		template<typename DGR>
        static void GetOutArcs(DGR& dg, Digraph::Node u, EdgeArray& es)
		{
			for(typename DGR::OutArcIt e(dg, u); e!=INVALID; ++e)
			{
				es.push_back(e);
			}
		}

        /**
         * 获取节点u的所有入边分支.
         * @param dg [in] 有向图
         * @param u  [in] 节点
         * @param es [out] 节点u的所有入边(es是一个动态数组)
         */
		template<typename DGR>
        static void GetInArcs(DGR& dg, Digraph::Node u, EdgeArray& es)
		{
			for(typename DGR::InArcIt e(dg, u); e!=INVALID; ++e)
			{
				es.push_back(e);
			}
		}

        /**
         * 向图添加一个虚拟的源点和一个虚拟的汇点.
         * @param dg [in] 有向图
         * @param sn [out] 虚拟的源点
         * @param tn [out] 虚拟的汇点
         * @return 操作是否成功,如果失败,则该有向图肯定存在存在单向回路.
         * @note 该函数内部调用的AddVirtualSource()和AddVirtualTarget()函数
         */
		template<typename DGR>
        static bool AddVirtualSTNode(DGR& dg, Digraph::Node& sn, Digraph::Node& tn)
		{
			// 增加一个虚拟源点及分支
			sn = GraphUtils::AddVirtualSource(dg);
			// 增加一个虚拟汇点及分支
			tn = GraphUtils::AddVirtualTarget(dg);

			return (sn != INVALID) && (tn != INVALID);
		}

        /**
         * 删除节点的所有出边分支.
         * @param dg [in] 有向图
         * @param u  [in] 节点
         */
		template<typename DGR>
        static void DeleteOutArcs(DGR& dg, Digraph::Node u)
		{
			EdgeArray es;
			for(typename DGR::OutArcIt e(dg, u); e!=INVALID; ++e)
			{
				es.push_back(e);
			}
			// 删除虚拟分支
			for(size_t i=0;i<es.size();i++)
			{
				dg.erase(es[i]);
			}
		}

        /**
         * 删除节点的所有入边分支.
         * @param dg [in] 有向图
         * @param u  [in] 节点
         */
		template<typename DGR>
        static void DeleteInArcs(DGR& dg, Digraph::Node u)
		{
			EdgeArray es;
			for(typename DGR::InArcIt e(dg, u); e!=INVALID; ++e)
			{
				es.push_back(e);
			}
			// 删除虚拟分支
			for(size_t i=0;i<es.size();i++)
			{
				dg.erase(es[i]);
			}
		}

        /**
         * 构造一个分支过滤器.
         * @param edges [in] 待过滤的分支
         * @param arc_filter    [in] 分支过滤属性
         * @param value [in] 过滤标记(true/false)
         * @note arc_filter向图中的所有分支附加了一个bool类型的属性数据,用于过滤分支;\n
		 *       该函数的作用就是将edges数组中的所有分支进行标记==>arc_filter[e]=value;
         */
        static void BuildArcFilter(EdgeArray& edges, ArcFilter& arc_filter, bool value)
		{
			for(size_t i=0;i<edges.size();i++)
			{
				Digraph::Arc e = edges[i];
				arc_filter[e] = value;
			}
		}

        /**
         * 根据过滤标记查找分支.
         * @param dg    [in] 有向图
         * @param arc_filter    [in] 分支过滤属性
         * @param edges [out] 过滤的分支
         * @param value [in] 过滤标记(true/false)
         * @note arc_filter向图中的所有分支附加了一个bool类型的属性数据,用于过滤分支;
		 * 该函数的作用就是遍历图中的所有分支,将()arc_filter[e]==value)的分支添加到edges数组
         */
		template<typename DGR>
        static void FindEdgeByFilter(DGR& dg, ArcFilter& arc_filter, EdgeArray& edges, bool value)
		{
			for(typename DGR::ArcIt e(dg);e!=INVALID;++e)
			{
				if(arc_filter[e] == value)
				{
					edges.push_back(e);
				}
			}
		}

        /**
         * 判断图是否连通的.
         * @param dg [in] 有向图
         * @return 是否连通(true=连通; false=不连通)
         */
		template<typename DGR>
        static bool IsConnected(DGR& dg)
		{
			typedef Undirector<DGR> Adaptor;
			Adaptor adaptor(dg);
			return lemon::connected(adaptor);
		}

        /**
         * 判断是否有向无环图(Directed Acycle Graph,缩写DAG).
         * @param dg [in] 有向图
         * @return 是否DAG(true=是,false=否)
         */
		template<typename DGR>
        static bool IsDAG(DGR& dg)
		{
			return lemon::dag(dg);
		}

		template<typename DGR, typename CompMap>
		static void FindEdgeByCCId( DGR& dg, CompMap& compMap, int cc, EdgeArray& es )
		{
			//查找第cc个连通块
			typedef typename DGR::template NodeMap<bool> NodeFilter;
			NodeFilter node_filter( dg, false );  // 用于过滤连通标识不等于cc的节点

			int count = 0;
			for( typename DGR::NodeIt n( dg ); n != INVALID; ++n )
			{
				if( compMap[n] == cc )
				{
					count++;
					node_filter[n] = true;
				}
			}
			if( count < 2 ) return; // 忽略单独一个节点是(强)连通分量的情况

			// 对有向图进行过滤, 过滤掉所有连通块标识不为cc的节点
			typedef FilterNodes<DGR, NodeFilter> Adaptor;
			Adaptor adaptor( dg, node_filter );

			// 执行一次dfs
			// 记录连通标识为cc的节点之间的分支集合(有向图分支)
			CNNEdgeVisitor<DGR> visitor( es );
			DfsVisit<Adaptor, CNNEdgeVisitor<DGR> > dv( adaptor, visitor );
			dv.run();
		}

        /**
         * 查找单向回路(采用图论的"强连通块"算法).
         * @param dg [in] 有向图
         * @param nums [out] 存储每一个单向回路的分支个数
         * @param scc [out] 所有单向回路分支
         * @note 为了方便操作,采用一个数组存储所有的单向回路分支
		 *       其中nums记录的是每个单向回路的分支个数,scc依次记录所有回路的分支
         * @code
         *      DGR dg;
         *      IntArray nums;
         *      EdgeArray scc;
         *      //计算单向回路
         *      Graphutils::SCC(dg,nums,scc);
         *      //打印所有的单向回路
         *      int k = 0;
         *      for(int i=0;i<nums.size();i++)
         *      {
         *         cout<<"第"<<i<<"个单向回路:"<<endl;
         *         for(int j=k;j<k+nums[i];j++)
         *         {
         *            cout<<"e"<<scc[j]<<" "<<;
         *         }
         *         k = k + nums[i];
         *         cout<<endl;
         *      }
         * @endcode
         */
		template<typename DGR>
        static void SCC( DGR& dg, IntArray& nums, EdgeArray& scc )
		{
			// 节点属于哪个连通分量
			typedef typename DGR::template NodeMap<int> CompMap;
			CompMap compMap( dg );

			// 返回单向回路个数
			int c = lemon::stronglyConnectedComponents( dg, compMap );

			for( int i = 0; i < c; i++ )
			{
				EdgeArray es;
				FindEdgeByCCId( dg, compMap, i, es );
				if( es.empty() ) continue;

				int len = es.size();
				for( int j = 0; j < len; j++ )
				{
					typename DGR::Arc e = es[j];
					scc.push_back( e );
				}
				nums.push_back( len );
			}
		}

		/**
		 * 拓扑排序.
		 * @param dg 有向图.
		 * @param nodes [out] 拓扑排序后的节点序列.
		 */
		template<typename DGR>
		static void TopologySort(DGR& dg, NodeArray& nodes)
		{
			for(typename DGR::NodeIt n(dg);n!=INVALID;++n)
			{
				nodes.push_back(n);
			}

			typedef typename DGR::template NodeMap<int> NodeOrder;
			NodeOrder order(dg);
			lemon::topologicalSort(dg, order);
			std::sort(nodes.begin(), nodes.end(), SortNodeByOrder<DGR>(order));
		}

		/**
		 * 深度优先搜索两个节点之间的一条路径.
		 * @param dg 有向图.
		 * @param u  搜索的始点.
		 * @param v  搜索的末点.
		 * @param p  [out] 路径(分支数组)
		 */
		template<typename DGR>
		static bool DFS_Path(DGR& dg, Digraph::Node u, Digraph::Node v, EdgeArray& p)
		{
			typedef Path<DGR> DPath;
			Dfs<DGR> aDfs(dg);
			if(!aDfs.run(u, v)) return false;

			DPath up = aDfs.path(v);
			for(typename DPath::ArcIt e(up); e!=INVALID; ++e)
			{
				p.push_back(e);
			}
			return true;
		}

		/**
		 * 深度优先搜索两个节点之间的一条路径,该路径经过指定的分支e.
		 * @param dg 有向图.
		 * @param s  搜索的始点.
		 * @param t  搜索的末点.
		 * @param e  路径要经过的分支
		 * @param p  [out] 路径(分支数组)
		 */
		template<typename DGR>
		static bool DFS_Pass(DGR& dg, Digraph::Node s, Digraph::Node t, Digraph::Arc e, EdgeArray& p)
		{
			Digraph::Node u = dg.source(e);
			Digraph::Node v = dg.target(e);
			if(t == u || s == v) return false;
			if(u == s && v == t) return DFS_Path(dg, s, t, p);

			bool ret = false;
			EdgeArray tp1, tp2; // 临时路径
			bool ret1 = DFS_Path(dg, s, u, tp1);
			bool ret2 = DFS_Path(dg, v, t, tp2);
			if(ret1 && ret2)
			{
				std::copy(tp1.begin(),tp1.end(), std::back_inserter(p));
				p.push_back(e);
				std::copy(tp2.begin(),tp2.end(), std::back_inserter(p));
				ret = true;
			}
			return ret;
		}

		/**
		 * 多个节点依次进行反向深度优先搜索,查找它们的所有通路共有的公共分支.
		 * @param dg    [in]  有向图.
		 * @param nodes [in]  要搜索的多个节点.
		 * @param edges [out] 公共分支.
		 */
		template<typename DGR>
		static void RDFS_Common(DGR& dg, const NodeArray& nodes, EdgeArray& edges)
		{
			if(nodes.size() < 2) return;

			//分支反向适配器
			typedef ReverseDigraph<Digraph> Adaptor;
			typedef std::set<Digraph::Arc> ArcSet;

			Adaptor adaptor(dg);
			ArcSet aset;
			for(NodeArray::const_iterator itr=nodes.begin();itr!=nodes.end();++itr)
			{
				// 深度优先遍历(u->)
				Dfs<Adaptor> aDfs(adaptor);
				aDfs.run(*itr);

				for(typename DGR::ArcIt e(dg);e!=INVALID;++e)
				{
					Digraph::Node u = dg.source(e);
					Digraph::Node v = dg.target(e);
					if(aDfs.reached(u) && aDfs.reached(v))
					{
						if(aset.find(e) != aset.end())
						{
							edges.push_back(e);
						}
						else
						{
							aset.insert(ArcSet::value_type(e));
						}
					}
				}
			}
		}

		/** 
		 * 无向图深度优先搜索节点u->v之间的一条路径.
		 * 基本思路:
		 *      然后将有向图转换成无向图
		 *      对这个无向图做一次深度优先搜索
		 * @param dg [in]  有向图.
		 * @param u  [in]  搜索的始节点.
		 * @param v  [in]  搜索的末节点.
		 * @param p  [out] 搜索得到的路径.
		 * @note 该算法用于在找到生成树之后,过滤掉余枝,然后搜索独立回路.\n
		 *       通过无向图深度优先搜索得到的"路径+余枝"就构成了一个独立回路.
		 */
		template<typename DGR>
		static bool UDfs_Path(DGR& dg, Digraph::Node u, Digraph::Node v, EdgeArray& p)
		{
			//将有向图转成无向图
			typedef Undirector<DGR> Graph;
			typedef Path<Graph> UPath;
			Graph g(dg);

			// 深度优先搜索(u->v)
			Dfs<Graph> aDfs(g);
			if(!aDfs.run(u, v)) return false;

			// 获取路径
			UPath up = aDfs.path(v);
			for(typename UPath::ArcIt ue(up); ue!=INVALID; ++ue)
			{
				typename UPath::Arc ua = ue;
				Digraph::Arc a = ua;     // 从无向图的(Arc->Edge)->有向图的Arc
				p.push_back(a);
			}
			return true;
		}

		//单源最短路径搜索
		template<typename DGR>
		static void SPFA(DGR& dg, Weight& w, Digraph::Node u, DistMap& dist)
		{
			//dijkstra算法求最短距离(权重必须为正)
			//Dijkstra<DGR,Weight> algo(dg, w);
			//BellmanFord算法求最短距离(可以有负权重)
			BellmanFord<DGR,Weight> algo(dg, w);
			algo.run(u);
			//cout<<"source "<<dg.id(t)<<"="<<dist[t]<<endl;

			for(typename DGR::NodeIt n(dg); n!=INVALID;++n)
			{
				if(algo.reached(n))
				{
					dist[n] = algo.dist(n);
					//cout<<"node "<<dg.id(n)<<"="<<dist[n]<<endl;
				}
			}
		}

		//正向单源最短路径搜索
		template<typename DGR>
		static void ForeSPFA(DGR& dg, Weight& w, Digraph::Node u, DistMap& dist)
		{
			SPFA(dg, w, u, dist);
		}

		//逆向单源最短路径搜索
		template<typename DGR>
		static void BackSPFA(DGR& dg, Weight& w, Digraph::Node u, DistMap& dist)
		{
			typedef ReverseDigraph<DGR> Adaptor;
			Adaptor adaptor(dg);
			SPFA(adaptor, w, u, dist);
		}

        /**
         * 构建拓扑关系,建立虚拟id与真实id之间的映射.
         * 输入参数:
         *  stream -- 流输入(代表广泛的数据流,例如文件流,字符串流,内存流,网络字节流等)
         *  dg     -- 通风网络(输入时dg是空的,没有节点和分支)
         * 输出参数:
         *  dg     -- 通风网络(添加了节点和分支)
         *  eid    -- 虚拟分支id与真实巷道id之间的映射
         *  nid    -- 虚拟节点id与真实节点id之间的映射
         * 使用示例:
         * 参见:test/test_vni
         * //用字符串表示拓扑关系
         * // 第1条分支,编号100,始节点编号10,末节点编号20
         * // 第2条分支,编号101,始节点编号10,末节点编号30
         * // 第3条分支,编号102,始节点编号20,末节点编号30
         * string topo_str="100 10 20 101 10 30 102 20 30";
         *
         * //这里使用字符串流作为输入
         * stringstream in(topo_str);
         *
         * //构建头铺关系,建立映射
         * DGR dg;
         * NodeIdMap nid(dg);
         * EdgeIdMap eid(dg);
         * Graphutils::Creategraphfromtopology(in, dg, eid, nid);
         *
         * //使用dg做一些计算,比如vno,vng等
         * // ..do something...
         *
         * //这里简单打印一下id映射
         * for(typename DGR::NodeIt n(dg);n!=INVALID;++n)
         * {
         *    cout<<"节点虚拟id:"<<dg.id(n)<< "-->真实id:"<<nid[n]<<endl;
         * }
         * for(typename DGR::ArcIt e(dg);e!=INVALID;++e)
         * {
         *    cout<<"分支虚拟id:"<<dg.id(e) <<"-->真实id:"<<eid[e]<<endl;
         * }
         */
        static bool CreateGraphFromTopology(std::istream& stream, Digraph& dg, EdgeIdMap& eid, NodeIdMap& nid);

		/**
		 * 基于固定最大流算法的流量分配.
		 * 给定总流量Q,以及部分分支的固定流量(或权重),判断固定流量能否分配下去;\n
		 * 如果能分配,则初始化一套满足流量守恒和固定流量限制条件的分配方案.
		 * @param dg [in] 有向图
		 * @param w  [in] 分支的固定流量(无固定流量就设置为0,即w[e]=0)
		 * @param Q  [in] 总流量(默认为无穷大)
		 */
		static bool FixedMaxFlow(Digraph& dg, Weight& w, double Q);

private:
	// 连通分支visitor
	template<typename DGR>
	class CNNEdgeVisitor : public DfsVisitor<DGR>
	{
	public:
		typedef typename DGR::Arc Arc;

	public:
		CNNEdgeVisitor( EdgeArray& es_ ) : es( es_ ) {}
		void discover( const Arc& e )
		{
			addCNNEdge( e );
		}
		void examine( const Arc& e )
		{
			addCNNEdge( e );
		}

	private:
		void addCNNEdge( const Arc& e )
		{
			es.push_back( e );
		}

		EdgeArray& es;
	};
		
	/**
	 * 辅助类--按节点的顺序属性(order)对节点排序.
	 * 这是一个仿函数functor,提供给C++标准库STL中的sort模板函数使用.
	 */
	template<typename DGR, typename NodeOrder= typename DGR::template NodeMap<int> >
	struct SortNodeByOrder
	{
		typedef typename DGR::Node Node;

		SortNodeByOrder(NodeOrder& _order) : order(_order)
		{
		}
		bool operator()(Node u, Node v)
		{
			return order[u] < order[v];
		}
		NodeOrder& order;
	};
};

#endif // VNO_GRAPH_UTILS_H
