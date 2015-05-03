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
 * Weight w(dg);
 * // 构造FixQWeightFunc对象
 * DefWeightFunc wf;
 * GraphUtils::BuildWeight(dg, w, wf);
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
 * Weight w(dg);
 * // 构造FixQWeightFunc对象
 * PressureWeightFunc wf(vnet);
 * GraphUtils::BuildWeight(dg, w, wf);
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
			pq.push(child_node);
        }
    }
    return (cnt == k);
}

//根据前向节点查找路径
static bool PathFromPred(Digraph& dg, Digraph::Node s, Digraph::Node t, PredMap& pred, EdgeArray& p)
{
	EdgeArray pp;
	Digraph::Node u = t;
	while(pred[u] != INVALID)
	{
		Digraph::Node v = pred[u];
		if(v != INVALID)
		{
			for(Digraph::InArcIt e(dg,u);e!=INVALID;++e)
			{
				if(dg.source(e) == v)
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
	}
	p = pp;
	return (u == s);
}

bool mrp_k(Digraph& dg, ArcFilter& arc_filter, Digraph::Node s, Digraph::Node t, int k, EdgeArray& p)
{
    //以分支阻力为权重
    Weight w(dg, -1);

    //用FilterArc适配器过滤一部分分支
    typedef FilterArcs<Digraph, ArcFilter> Adaptor;
    Adaptor adaptor(dg, arc_filter);

    //计算节点到末节点t的最短路径
    DistMap dist(dg, DBL_MAX);
    GraphUtils::BackSPFA(adaptor, w, t, dist);

    //节点的前驱节点(父节点),用于后续计算路径
    PredMap pred(dg, INVALID);
    //使用A*算法搜索k最短路径
    if(!A_star(adaptor, w, s, t, k, dist, pred)) return false;

    //acutPrintf(_T("\n根据前向节点查找路径"));
    //根据前向节点查找路径
    bool ret = PathFromPred(dg, s, t, pred, p);
    return (ret && !p.empty());
}

bool dfs_all_path(Digraph& dg, ArcFilter& arc_filter, Digraph::Node s, Digraph::Node t, IntArray& pathNums, EdgeArray& allPath)
{
    for(int k=1;k<1000;k++)
    {
        //存储搜索的路径
        EdgeArray p;
        //搜索第k路径
        //acutPrintf(_T("\nK=%d"),k);
        if(!mrp_k(dg, arc_filter, s, t, k, p))
        {
            //一旦第k次搜索不成功,后续的搜索同样也会失败!
            //所以这时候可以直接退出循环了
            break;
        }
        else
        {
            //记录路径的分支个数
            pathNums.push_back(p.size());
            //将路径复制到allPath数组中
            std::copy(p.begin(), p.end(), std::back_inserter(allPath));
        }
    }
    return (!pathNums.empty());
	//return false;
}

struct PositiveTubeVisitorFunctor
{
	PositiveTubeVisitorFunctor(GasTubeTypeMap& gtt_) : gtt( gtt_ ), inSeriable(false)
	{

	}
	void operator()(Digraph::Arc e)
	{
		gtt[e] = GTT_POSITIVE;
	}
	GasTubeTypeMap& gtt;
	bool inSeriable;
};

// 正压管visitor
template<typename DGR, typename VisitorFunc>
class PositiveTubeVisitor : public DfsVisitor<DGR>
{
public:
	PositiveTubeVisitor( VisitorFunc& func_) : func(func_)
	{
	}
	void discover( const typename DGR::Arc& e )
	{
		func(e);
	}
	void examine( const typename DGR::Arc& e )
	{
		func(e);
	}
private:
	VisitorFunc& func;
};

bool partition_gas_tubes(Digraph& dg, ArcFilter& arc_filter, NodeArray& pumps, GasTubeTypeMap& gtt)
{
	//标记所有分支为负压管
	for( Digraph::ArcIt e( dg ); e != INVALID; ++e )
	{
		gtt[e] = GTT_NEGATIVE;
	}
	//用FilterArc适配器过滤一部分分支
	typedef FilterArcs<Digraph, ArcFilter> Adaptor;
	Adaptor adaptor(dg, arc_filter);

	PositiveTubeVisitorFunctor func(gtt);
	typedef PositiveTubeVisitor<Adaptor, PositiveTubeVisitorFunctor> VisitoAdaptor;
	typedef DfsVisit<Adaptor, VisitoAdaptor> DfsVisitAdaptor;

	bool ret = true;

	//从所有的泵分支的末点开始，做一次dfs
	//遍历到的所有分支均视作正压管
	for( int i = 0; i < pumps.size(); i++ )
	{
		//初始化假设没有出现泵串联的情况
		func.inSeriable = false;

		//定义dfs visitor,用于在dfs遍历的过程中对分支进行标记
		VisitoAdaptor visitor( func );
		DfsVisitAdaptor dv( adaptor, visitor );
		//从泵分支的末节点开始dfs
		Digraph::Node u = pumps[i];
		dv.run(u);
		
		//如果串联,则标记结果为false
		if(func.inSeriable)
		{
			ret = false;
		}
	}
	return ret;
}