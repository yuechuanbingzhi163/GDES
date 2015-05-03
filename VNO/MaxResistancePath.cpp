#include "vno.h"

#include <cmath>
#include <cstdlib>
#include <cfloat>
#include <climits>
#include <stack>
#include <queue>

#include <iostream>
using namespace std;

/** 
 * 默认权重生成器.
 * 这是一个仿函数functor,提供给GraphUtils::BuildWeight()函数的第3个参数使用
 * 注:仿函数functor是现代C++中广泛使用的一种技巧,作用和函数相似,但使用范围和功能更强大!
 * 基本思路:
 *      所有分支的权重都等于1.0
 * 调用方法:
 * // 准备数据
 * VentNetwork* vnet;
 * // 分支权重
 * Weight w(vnet->graph());
 * // 构造FixQWeightFunc对象
 * DefWeightFunc wf;
 * GraphUtils::BuildWeight(vnet->graph(), w, wf);
 */
struct DefWeightFunc
{
    DefWeightFunc()
    {
    }
    double operator()(Digraph::Arc e)
    {
        return 1.0;
    }
};

/** 
 * 基于分支阻力的权重生成器.
 * 这是一个仿函数functor,提供给GraphUtils::BuildWeight()函数的第3个参数使用
 * 注:仿函数functor是现代C++中广泛使用的一种技巧,作用和函数相似,但使用范围和功能更强大!
 * 基本思路:
 *      以分支的阻力作为权重
 * 调用方法:
 * // 准备数据
 * VentNetwork* vnet;
 * // 分支权重
 * Weight w(vnet->graph());
 * // 构造FixQWeightFunc对象
 * PressureWeightFunc wf(vnet);
 * GraphUtils::BuildWeight(vnet->graph(), w, wf);
 */
struct PressureWeightFunc
{
    PressureWeightFunc(VentNetwork* _vnet) : vnet(_vnet)
    {
    }
    double operator()(Digraph::Arc e)
    {
        return vnet->edge(e)->f0();
    }
    VentNetwork* vnet;
};

//用于priority_queue的比较函数.
//[知识点]:K最短路径
//节点的估价函数值,决定A*算法的搜索方向
//对于A*算法,估计函数=当前值+当前位置到终点的距离成本
//即f(p)=g(p)+h(p),其中g(n)表示
//对于k最短路径算法来说,g(p)为当前从到p所走的路径的长度;
//h(p)为点p到t的最短路的长度

struct QNode
{
	QNode(Digraph::Node _u, Digraph::Node _p, double _g, double _h) 
		: u(_u), g(_g), h(_h), parent(_p) {}
	Digraph::Node u;
	Digraph::Node parent;
	double g, h;

	bool operator<(const QNode& b) const
	{
		// 浮点数误差精度
		const double tol = 1e-4;
		double f1 = g+h;
		double f2 = b.g+b.h;
		if(fabs(f1-f2) < tol)
		{
			return g > b.g;
		}
		else
		{
			return f1 > f2;
		}
	}
};

//BFS和Dijkstra算法都是A*算法的一种(f(n)=g(n)+h(n))
//其中BFS的h(n)=0,没有利用启发式信息,盲目搜索,而Dijkstra也是如此
//A*算法和Dijkstra算法都属于一类,原则上不能应用于有负权重分支的图
//但本程序假设除了虚拟分支以外所有的权重都为负值,所以也是可以适用的
template<typename DGR>
static bool A_star(DGR& dg, Weight& w, Digraph::Node s, Digraph::Node t, int k, DistMap& dist, PredMap& pred)
{
    if(s == t) return false;

	typedef QNode Node;
    typedef std::vector<QNode> NodeCon;
    typedef std::priority_queue<Node,NodeCon> PriorityQueue;
    PriorityQueue pq;

    int cnt = 0;
    pred[s] = INVALID;
	pq.push(Node(s, INVALID, 0, dist[s]));
    while(!pq.empty())
    {
		Node node = pq.top();
		Digraph::Node u = node.u;
		pred[u] = node.parent;
		//cout<<"->pop:f("<<Digraph::id(u)<<")="<<node.g+p.h
		//	  <<"\tg("<<Digraph::id(u)<<")="<<node.g
		//	  <<"\th("<<Digraph::id(u)<<")="<<node.h
		//	  <<endl;
        pq.pop();
        if(u == t)
        {
			//cout<<"---------------------"<<endl;
            cnt++;
        }
        if(cnt == k)
        {
            break;
        }
        for(typename DGR::OutArcIt e(dg,u);e!=INVALID;++e)
        {
            Digraph::Node v = dg.target(e);
            pred[v] = u;
			Node child_node(v, u, node.g + w[e], dist[v]);
			//cout<<"\t->push:f("<<Digraph::id(v)<<")="<<child_node.g+child_node.h
				//<<"\tg("<<Digraph::id(v)<<")="<<child_node.g
				//<<"\th("<<Digraph::id(v)<<")="<<child_node.h
				//<<endl;
			pq.push(child_node);
        }
    }
    return (cnt == k);
}

//计算关键活动分支
template<typename DGR>
static void CriticalActivity(DGR& dg, Weight& w, EdgeArray& criticalEdges)
{
    //过滤后的图
    typedef typename DGR::template NodeMap<double> EventTimeMap;
    typedef typename DGR::template ArcMap<double> ActivityTimeMap;

    //正向拓扑排序,计算ve
    //事件最早发生时间vt
    EventTimeMap ve(dg,0);
    NodeArray nodes;
    GraphUtils::TopologySort(dg, nodes); // nodes[0] == sn
    for(size_t i=0;i<nodes.size();i++)
    {
        Digraph::Node uu = nodes[i];

        double maxTime = DBL_MIN;
        int count = 0;
        for(typename DGR::InArcIt e(dg,uu);e!=INVALID;++e)
        {
            count++;
            double t = ve[dg.source(e)] + w[e];
            if(t > maxTime)
            {
                maxTime = t;
            }
        }
        if(count > 0)
        {
            ve[uu]=maxTime;
        }
    }

    //逆向拓扑排序,计算vt
    //vt默认初始话为ve拓扑序列的最后一个节点值
    //事件最晚发生时间vt
    EventTimeMap vt(dg,ve[nodes.back()]);
    //拓扑序列反向
    GraphUtils::ReverseArray(nodes);
    for(size_t i=0;i<nodes.size();i++)
    {
        Digraph::Node uu = nodes[i];
        double minTime = DBL_MAX;
        int count = 0;
        for(typename DGR::OutArcIt e(dg,uu);e!=INVALID;++e)
        {
            double t = ve[dg.target(e)] - w[e];
            if(t < minTime)
            {
                minTime = t;
            }
            count++;
        }
        if(count > 0)
        {
            vt[uu] = minTime;
        }
    }

    //活动最早开始和最晚开始时间
    ActivityTimeMap et(dg), lt(dg);
    for(typename DGR::ArcIt e(dg);e!=INVALID;++e)
    {
        Digraph::Node u = dg.source(e);
        Digraph::Node v = dg.target(e);
        et[e] = ve[dg.source(e)];
        lt[e] = vt[dg.target(e)] - w[e];
        //如果et[e] == lt[e],则该分支为关键活动分支
        if(abs(et[e] - lt[e]) < 0.001)
        {
            criticalEdges.push_back(e);
        }
    }
}

//关键路径算法--求最短路径
template<typename DGR>
static bool CriticalPathMethod(DGR& dg, Weight& w, Digraph::Node s, Digraph::Node t, EdgeArray& p)
{
    //搜索关键活动分支
    EdgeArray criticalEdges;
    CriticalActivity(dg, w, criticalEdges);
    if(criticalEdges.empty()) return false;
    //过滤非关键活动分支,dfs搜索关键路径
    //基本原理:过滤掉其它非关键活动分支,执行一次dfs
    
    typename DGR::template ArcMap<bool> arc_filter(dg, false);
    GraphUtils::BuildArcFilter(criticalEdges, arc_filter, true);

	//用FilterArc适配器过滤一部分分支
    typedef FilterArcs<DGR, ArcFilter> Adaptor;
    Adaptor adaptor(dg, arc_filter);
    return GraphUtils::DFS_Path(adaptor, s, t, p);
}

//根据前向节点查找路径
static bool PathFromPred(VentNetwork* vnet, Digraph::Node s, Digraph::Node t, PredMap& pred, EdgeArray& p)
{
    EdgeArray pp;
    Digraph::Node u = t;
    while(pred[u] != INVALID)
    {
        Digraph::Node v = pred[u];
        if(v != INVALID)
        {
            for(Digraph::InArcIt e(vnet->graph(),u);e!=INVALID;++e)
            {
                if(vnet->graph().source(e) == v)
                {
                    pp.push_back(e);
                }
            }
        }
        u = v;
    }
    if(u == s)
    {
        //得到的路径是反向的,需要逆转过来
        GraphUtils::ReverseArray(pp);
        //过滤虚拟分支
        vnet->filterVirutalEdges(pp, p);
    }
    return (u == s);
}

/*****************************************向下分割线*******************************************************/
/***********************************  练手代码,尚未完全测试成功  ****************************************/

//指定一些特殊分支,要求搜索一条路径时尽可能多的通过这些特殊分支
//例如:搜索一条串联通风路径(通过多个用风地点的一条路径)
//使用关键路径算法实现
static bool MaxKeyEdgePass_CPM(Digraph& dg, EdgeArray& airEdges, Digraph::Node s, Digraph::Node t, EdgeArray& mpp)
{
    EdgeArray p;
    if(!GraphUtils::DFS_Path(dg, s, t, p)) return false;

    NodeArray left, right;
    left.push_back(s); right.push_back(t);
    for(size_t i=0;i<airEdges.size();i++)
    {
        Digraph::Arc e = airEdges[i];
        Digraph::Node u = dg.source(e);
        Digraph::Node v = dg.target(e);

        p.clear();
        bool ret = GraphUtils::DFS_Path(dg, v, t, p);
        if(!ret) continue;

        p.clear();
        ret = GraphUtils::DFS_Path(dg, s, u, p);
        if(!ret) continue;

        left.push_back(v); right.push_back(u);
    }
    //cout<<"left="<<left.size()<<" right="<<right.size()<<endl;

    //构造新的图
    Digraph new_dg;
    //记录新图节点和原始图节点的关系
    Digraph::NodeMap<Digraph::Node> udm(new_dg);
    //记录原始图节点与新构造图节点的关系
    typedef std::map<Digraph::Node, Digraph::Node> DDMap;
    DDMap ddm;
    //左右节点数组
    NodeArray left_nodes, right_nodes;

    //添加节点
    for(size_t i=0;i<left.size();i++)
    {
        Digraph::Node u = left[i], uu = INVALID;
        DDMap::iterator itr = ddm.find(u);
        if(itr == ddm.end())
        {
            uu = new_dg.addNode();
        }
        else
        {
            uu = itr->second;
        }
        udm[uu] = u; ddm[u] = uu; 
        left_nodes.push_back(uu);
        //cout<<dg.id(u)<< " ";
    }
    //cout<<endl;
    for(size_t i=0;i<right.size();i++)
    {
        Digraph::Node u = right[i], uu = INVALID;
        DDMap::iterator itr = ddm.find(u);
        if(itr == ddm.end())
        {
            uu = new_dg.addNode();
        }
        else
        {
            uu = itr->second;
        }
        udm[uu] = u; ddm[u] = uu; 
        right_nodes.push_back(uu);
        //cout<<dg.id(u)<<" ";
    }
    //cout<<endl;
    //添加分支
    for(size_t i=0;i<left_nodes.size();i++)
    {
        Digraph::Node uv = left_nodes[i];

        for(size_t j=0;j<right_nodes.size();j++)
        {
            Digraph::Node uu = right_nodes[j];
            Digraph::Node v = udm[uv];
            Digraph::Node u = udm[uu];
            if(u == v || (v == s && u == t)) continue;

            p.clear();
            if(GraphUtils::DFS_Path(dg, v, u, p))
            {
                Digraph::Arc e = new_dg.addArc(uv,uu);
                //cout<<"新图:"<<dg.id(v)<<"<-->"<<dg.id(u)<<endl;
            }
        }
    }
    for(size_t i=0;i<left_nodes.size();i++)
    {
        Digraph::Node uv = left_nodes[i];
        Digraph::Node uu = right_nodes[i];
        Digraph::Node v = udm[uv];
        Digraph::Node u = udm[uu];
        if(u == v || u == t) continue;

        Digraph::Arc e = new_dg.addArc(uu,uv);
        //cout<<"新图:"<<dg.id(u)<<"<-->"<<dg.id(v)<<endl;
    }

    //新图搜索关键路径,分配权重,默认为1(最长路径)
    Weight w(new_dg);
    GraphUtils::BuildWeight(new_dg, w, DefWeightFunc());
    EdgeArray pp;
    if(!CriticalPathMethod(new_dg, w, left_nodes[0], right_nodes[0], pp)) return false;
    //cout<<"搜索关键路径:"<<pp.size()<<endl;

    bool ret = true;
    //转换成实际图的分支
    for(size_t i=0;i<pp.size();i++)
    {
        Digraph::Arc ee = pp[i];
        Digraph::Node uu = new_dg.source(ee);
        Digraph::Node uv = new_dg.target(ee);
        Digraph::Node u = udm[uu];
        Digraph::Node v = udm[uv];
        EdgeArray ppp;
        if(!GraphUtils::DFS_Path(dg, u, v, ppp))
        {
            ret = false;
            mpp.clear();
            break;
        }
        else
        {
            std::copy(ppp .begin(), ppp .end(), std::back_inserter(mpp));
        }
    }
    //cout<<"mpp:"<<mpp.size()<<endl;
    return ret;
}

//指定一些特殊分支,要求搜索一条路径时尽可能多的通过这些特殊分支
//例如:搜索一条串联通风路径(通过多个用风地点的一条路径)
//采用二分匹配实现失败!!!
static bool MaxKeyEdgePass_Match(Digraph& dg, EdgeArray& airEdges, Digraph::Node s, Digraph::Node t, EdgeArray& mpp)
{
    EdgeArray p;
    if(!GraphUtils::DFS_Path(dg, s, t, p)) return false;

    typedef Digraph::NodeMap<Digraph::Node> DDMap;
    DDMap ddm(dg, INVALID);

    NodeArray left, right;
    left.push_back(s); right.push_back(t);
    for(size_t i=0;i<airEdges.size();i++)
    {
        Digraph::Arc e = airEdges[i];
        Digraph::Node u = dg.source(e);
        Digraph::Node v = dg.target(e);

        p.clear();
        bool ret = GraphUtils::DFS_Path(dg, v, t, p);
        if(!ret) continue;

        p.clear();
        ret = GraphUtils::DFS_Path(dg, s, u, p);
        if(!ret) continue;

        left.push_back(v); right.push_back(u);
    }
    //cout<<"left="<<left.size()<<" right="<<right.size()<<endl;

    ListGraph g;
    ListGraph::NodeMap<Digraph::Node> udm(g);
    typedef std::vector<ListGraph::Node> UNodeArray;
    UNodeArray left_nodes, right_nodes;
    typedef std::map<Digraph::Node, ListGraph::Node> DUMap;
    DUMap dum;

    //添加节点
    for(size_t i=0;i<left.size();i++)
    {
        Digraph::Node u = left[i];
        ListGraph::Node uu = g.addNode();
        udm[uu] = u; left_nodes.push_back(uu);
        //cout<<dg.id(u)<< " ";
    }
    //cout<<endl;
    for(size_t i=0;i<right.size();i++)
    {
        Digraph::Node u = right[i];
        ListGraph::Node uu = g.addNode();
        udm[uu] = u; right_nodes.push_back(uu);
        //cout<<dg.id(u)<<" ";
    }
    //cout<<endl;
    for(size_t i=0;i<right.size();i++)
    {
        Digraph::Node u = right[i];
        ListGraph::Node uu = g.addNode();
        udm[uu] = u; left_nodes.push_back(uu);
        //cout<<dg.id(u)<< " ";
    }
    //cout<<endl;
    for(size_t i=0;i<left.size();i++)
    {
        Digraph::Node u = left[i];
        ListGraph::Node uu = g.addNode();
        udm[uu] = u; right_nodes.push_back(uu);
        //cout<<dg.id(u)<< " ";
    }
    //cout<<endl;
    //添加分支
    for(size_t i=0;i<left_nodes.size();i++)
    {
        ListGraph::Node uv = left_nodes[i];

        for(size_t j=0;j<right_nodes.size();j++)
        {
            ListGraph::Node uu = right_nodes[j];

            Digraph::Node v = udm[uv];
            Digraph::Node u = udm[uu];
            if(u == v) continue;

            p.clear();
            if(GraphUtils::DFS_Path(dg, v, u, p))
            {
                //ListGraph::Node uv = left_nodes[i];
                //ListGraph::Node uu = right_nodes[j];
                ListGraph::Edge e = g.addEdge(uu,uv);
                //cout<<"二分图:"<<dg.id(v)<<"<-->"<<dg.id(u)<<endl;
            }
        }
    }
    //二分图最大匹配
    MaxMatching<ListGraph> mm(g);
    mm.run();

    //cout<<"分支数:"<<countEdges(g)<<" 匹配数:"<<mm.matchingSize()<<endl;
    for(ListGraph::EdgeIt e(g);e!=INVALID;++e)
    {
        if(mm.matching(e))
        {
            ListGraph::Node du = g.u(e);
            ListGraph::Node dv = g.v(e);
            Digraph::Node u = udm[du];
            Digraph::Node v = udm[dv];
            //cout<<"匹配:"<<dg.id(v)<<"<-->"<<dg.id(u)<<endl;
            if(ddm[u] != INVALID && ddm[v] == INVALID)
            {
                ddm[v] = u;
                //cout<<" v"<<dg.id(v)<<"-->v"<<dg.id(u) <<"  v"<<dg.id(u)<<"-->"<<dg.id(ddm[u])<<endl;
            }
            else if(ddm[v] != INVALID && ddm[u] == INVALID)
            {
                ddm[u] = v;
                //cout<<" v"<<dg.id(v)<<"-->v"<<dg.id(ddm[v]) <<"  v"<<dg.id(u)<<"-->"<<dg.id(v)<<endl;
            }
        }
    }

    NodeArray node_path;
    Digraph::Node u = s;
    bool ret = true;
    while(u != t)
    {
        Digraph::Node v = ddm[u];
        //cout<<dg.id(u)<<"->"<<endl;
        if(v == INVALID)
        {
            //cout<<"错误"<<endl;
            ret = false;
            break;
        }
        //cout<<" ->"<<dg.id(v)<<" "<<endl;
        node_path.push_back(v);
        u = v;
    }
    if(ret)
    {
        u = s;
        for(size_t i=0;i<node_path.size();i++)
        {
            Digraph::Node v = node_path[i];
            GraphUtils::DFS_Path(dg, u, v, mpp);
            u = v;
        }
    }
    return ret;
}

//void mrpByFixQ(VentNetwork* vnet, EdgeArray& p)
//{
//    //查找固定风量
//    EdgeArray edges;
//    vnet->fixQEdges(edges);
//    //搜索一条经过关键分支最多的路径
//    EdgeArray pp;
//    //1)二分匹配法(失败!!!)
//    //MaxKeyEdgePass_Match(dg, edges, sn, tn, pp);
//    //2)关键路径法(可用)
//    MaxKeyEdgePass_CPM(dg, edges, sn, tn, pp);
//    //过滤虚拟分支
//    filterVirutalEdges(ed, pp, p);
//}
/*****************************************向上分割线*******************************************************/

bool mrp_k(VentNetwork* vnet, ArcFilter& arc_filter, Digraph::Node s, Digraph::Node t, int k, EdgeArray& p)
{
    //如果始节点的值"无效"(INVALID),那么默认始节点为虚拟的源点
    if(s == INVALID) s = vnet->vSource();
    //如果末节点的值"无效"(INVALID),那么默认末节点为虚拟的汇点
    if(t == INVALID) t = vnet->vTarget();

	//如果k小于等于0,则默认k=1(也就是搜索第1最大阻力路线)
	if(k<=0) k = 1;

    //以分支阻力为权重
    Weight w(vnet->graph(), 0);
    GraphUtils::BuildWeight(vnet->graph(), w, PressureWeightFunc(vnet));
    //权重取负值,将求最大权重相关的问题(例如最长路径,最大阻力路线等)转换成最短路径问题
    GraphUtils::ReverseWeight(vnet->graph(), w);
	
	//用FilterArc适配器过滤一部分分支
	typedef FilterArcs<Digraph, ArcFilter> Adaptor;
	Adaptor adaptor(vnet->graph(), arc_filter);

	//计算节点到末节点t的最短路径
	DistMap dist(vnet->graph(), DBL_MAX);
	GraphUtils::BackSPFA(adaptor, w, t, dist);
    
	//节点的前驱节点(父节点),用于后续计算路径
    PredMap pred(vnet->graph(), INVALID);
	//使用A*算法搜索k最短路径
    if(!A_star(adaptor, w, s, t, k, dist, pred)) return false;
    //根据前向节点查找路径
    bool ret = PathFromPred(vnet, s, t, pred, p);
    return (ret && !p.empty());
}

bool mrp(VentNetwork* vnet, ArcFilter& arc_filter, Digraph::Node s, Digraph::Node t, EdgeArray& p)
{
    return mrp_k(vnet, arc_filter, s, t, 1, p);
}

/**
 * 求两点之间的一条最大阻力路线(Max Resistance Path).
 * 使用"关键路径算法(Critical Path Method,简称CPM)"实现.
 * @param  vnet       [in]  单一源汇通风网络(连通且无单向回路).
 * @param  arc_filter [in]  分支过滤标记(arc_filter[e]=false,则分支e被过滤掉).
 * @param  s          [in]  始节点(如果s=INVALID,那么默认始节点为虚拟的源点).
 * @param  t          [in]  末节点(如果t=INVALID,那么默认末节点为虚拟的汇点).
 * @param  p          [out] 最大阻力路线上的所有分支.
 * @return 返回值,求最大阻力路线是否成功.
 * @note 在调用函数前,必须将网络转换成单一源汇网络!!!
 */
extern bool mrp_cpm(VentNetwork* vnet, ArcFilter& arc_filter, Digraph::Node s, Digraph::Node t, EdgeArray& p);
bool mrp_cpm(VentNetwork* vnet, ArcFilter& arc_filter, Digraph::Node s, Digraph::Node t, EdgeArray& p)
{
    //如果始节点的值"无效"(INVALID),那么默认始节点为虚拟的源点
    if(s == INVALID) s = vnet->vSource();
    //如果末节点的值"无效"(INVALID),那么默认末节点为虚拟的汇点
    if(t == INVALID) t = vnet->vTarget();

    //以分支阻力为权重
    Weight w(vnet->graph());
    GraphUtils::BuildWeight(vnet->graph(), w, PressureWeightFunc(vnet));

	//用FilterArc适配器过滤一部分分支
    typedef FilterArcs<Digraph, ArcFilter> Adaptor;
    Adaptor adaptor(vnet->graph(), arc_filter);

    //搜索关键路径(也即最大阻力路线)
    EdgeArray pp;
    if(!CriticalPathMethod(adaptor, w, s, t, pp)) return false;
    //过滤虚拟分支
    vnet->filterVirutalEdges(pp, p);
    return (!p.empty());
}
