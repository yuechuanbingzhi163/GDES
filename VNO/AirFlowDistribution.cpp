#include "vno.h"
//数据操作辅助方法类
#include "DataUtils.h"

// 独立回路类
#include "IndependentCycle.h"
//独立回路链表类
typedef std::vector<IndependentCycle*> CycleList;

//算法用的一些默认配置参数
#include "config.h"

#include <cmath>
#include <cstdlib>
#include <cfloat>
#include <climits>
#include <iostream>
using namespace std;


/** 
 * 基于最小生成树的权重生成器.
 * 基本思路:
 *      放大固定风量、风机、大气分支的权重,使得这些分支尽可能的作为最小生成树的余枝(非树边)
 *      放大比例略有不同,风机分支放大1千倍,固定风量分支放大1万倍,大气分支放大10万倍
 *      通过放大比例来控制余枝的选择顺序: 大气分支 > 固定风量分支 > 风机分支
 * @note 这是一个仿函数functor,提供给GraphUtils::BuildWeight()函数的第3个参数使用
 * @note 仿函数functor是现代C++中广泛使用的一种技巧,作用和函数相似,但使用范围和功能更强大!
 *
 * @code
 * //准备数据
 * VentNetwork* vnet;
 * bool speedUp;
 * //分支权重
 * Weight w(vnet->graph());
 * //构造TreeWeightFunc对象
 * TreeWeightFunc wf(vnet, speedUp);
 * GraphUtils::BuildWeight(vnet->graph(), w, wf);
 * @endcode
 */
struct TreeWeightFunc
{
    TreeWeightFunc(VentNetwork* _vnet, bool _speedUp)
        : vnet(_vnet),speedUp(_speedUp)
    {}

    double operator()(Digraph::Arc e)
    {
        double r = vnet->edge(e)->getSumR();
        double q = vnet->edge(e)->q;

        // 放大固定风量分支和风机分支(作为余枝)
        // r+1只是一个小技巧,避免0乘的情况
        double c = abs(speedUp?(1+r)*q:(r+1));

        if(vnet->fan(e) != 0)
        {
			c = DataUtils::Magnify_Double(c, LARGE_COEFF);    // 风机分支
        }
        else if(vnet->edge(e)->fq > 0)
        {
			c = DataUtils::Magnify_Double(c, LARGE_COEFF*10);   // 固定风量分支
        }
        else if(e == vnet->vAir())
        {
            c = DataUtils::Magnify_Double(c, LARGE_COEFF*100);   // 大气分支
        }
        return c;
    }

	VentNetwork* vnet;
    bool speedUp;
};

/** 
 * 辅助类--用于判断分支是否固定风量分支.
 * 这是一个仿函数functor,不单独使用,提供给其它函数调用
 * 例如stl(C++标准库)中的sort、find_if等模板函数接收一个仿函数参数
 * 在现代C++中已经广泛的使用仿函数来代替函数指针
 */
struct FixQTree
{
    FixQTree(EdgeMap& _ed):ed(_ed) {}
    bool operator()(Digraph::Arc e)
    {
        return ed[e]->fq > 0;
    }
    EdgeMap& ed;
};

/** 
 * 查找树枝中是否有固定风量.
 * @param tree 树边数组.
 * @param ed   分支属性数据.
 */
static bool FindFixQInTree(EdgeArray& tree, EdgeMap& ed)
{
	//调用C++标准库中的find_if函数来查找是否有固定风量
	//find_if函数需要提供一个仿函数(functor)参数
	// 通过FixQTree结构体构造一个对象传入该参数
    return (std::find_if(tree.begin(),tree.end(),FixQTree(ed)) != tree.end());
}

/** 
 * 辅助类--CycleListSwitch对象析构时自动删除独立回路对象占用的内存.
 * 这个类只是为了方便使用,不用额外编写啰嗦的for循环来释放回路占用的内存
 * 并且该类能够保证在任何情况下回路对象的内存都能够被释放,无论程序是否异常终止
 * 这种技巧在C++中称为"scope-guard"(防御式编程)
 */
class CycleListSwitch
{
    public:
		/**
		 * 构造函数.
		 */
        CycleListSwitch(CycleList& _cl) : cl(_cl) 
		{
		}
		/**
		 * 析构函数.
		 */
        ~CycleListSwitch()
        {
            for(size_t i=0;i<cl.size();i++)
            {
                delete cl[i];
            }
            cl.clear();
        }
        CycleList& cl;
};

/** 
 * 初始化风量.
 * 在迭代过程中其实不需要初始化风量,风量初始化只需要在迭代前执行一次即可
 * 目前初始化风量的方法有2种:(1)固定风量最大流法; (2)余枝分配法
 * "余枝分支法"是传统风网解算方法中常用的风量分配方法,但无法解决有固定风量的风量分配问题
 * 本程序采用的是"最大流方法"
 * @param vnet 通风网络.
 * @return 风量初始化成功与否.
 * @ref 详情参见 vno.h AirFlowDistribution.cpp 中的ifq函数的实现.
 */
static bool initQ(VentNetwork* vnet)
{
	return ifq(vnet);
}

static void findMST( VentNetwork* vnet, EdgeArray& tree, bool speedUp )
{
    //初始化分支权重
    Weight w(vnet->graph());
	GraphUtils::BuildWeight(vnet->graph(),w,TreeWeightFunc(vnet,speedUp));
    /*
     * 使用kruskal算法求最小生成树.
     * 思路：将大气分支,固定风量分支,风机分支以及的风阻乘以一个较大的系数
     *        使之远大于一般分支的风阻, 则最小生成树的算法不会选择
     *        大气分支,固定风量分支,风机分支作为树枝
     */
    typedef std::vector<Digraph::Arc> TreeArray;
    TreeArray ta;
    kruskal(vnet->graph(), w, std::back_inserter(tree));
}

static void findCycleByTree( VentNetwork* vnet, EdgeArray& tree, CycleList& cl )
{
	typedef FilterArcs<Digraph, ArcFilter> Adaptor;

    // 过滤余枝(树枝-true, 余枝-false)
    ArcFilter arc_filter(vnet->graph(), false);
    //标记树枝
    GraphUtils::BuildArcFilter(tree, arc_filter, true);
    // 查找余枝
    EdgeArray leftTree;
    GraphUtils::FindEdgeByFilter(vnet->graph(), arc_filter, leftTree, false);

    //判断树枝中是否有固定风量
    //如果有固定风量,表示无法继续解算
    //if(FindFixQInTree(tree, ed)) return;

    // 对每个余枝搜索回路
    for(size_t i=0;i<leftTree.size();i++)
    {
        Digraph::Arc e = leftTree[i];          // 回路的基准分支(余枝)
        Digraph::Node u = vnet->graph().source(e);  // 基准分支的始点
        Digraph::Node v = vnet->graph().target(e);  // 基准分支的末点
        
		//  查找v->u的回路(无向图dfs搜索,v->u的唯一路径)
		Adaptor adaptor(vnet->graph(), arc_filter);
		EdgeArray p;
		if(!GraphUtils::UDfs_Path(adaptor, v, u, p)) continue;

        // 构造独立回路对象
        IndependentCycle* pCycle = new IndependentCycle(vnet, e);
        // 将路径分支添加到回路对象
        for(size_t j=0;j<p.size();j++)
        {
            pCycle->addEdge(p[j]);
        }
        // 初始化方向系数
        pCycle->init();
		//添加回路到链表中
        cl.push_back(pCycle);
    }
}

static void findCycle(VentNetwork* vnet, CycleList& cl, bool speedUp)
{
    // 查找最小生成树
    EdgeArray tree;
    findMST(vnet, tree, speedUp);
    if(tree.empty()) return;

    // 根据最小生成树查找独立回路
    findCycleByTree(vnet, tree, cl);
}

/** 迭代结果. */
enum ITERATE_RESULT
{
	IR_FAILED=0,           /**< 错误(找不到生成树,回路等错误). */
	IR_SUCCESS=1,          /**< 迭代收敛. */
	IR_UNCOVERAGE=2        /**< 迭代不收敛. */
};

/**
 * 网络解算迭代过程.
 * 基本过程: (1)查找回路; (2)回路迭代
 * @param vent 通风网络.
 * @param count 迭代次数.
 * @param speedUp 是否进行加速.
 * @note "加速"顾名思义是为了加快收敛过程,查找最小生成树的时候利用|r*q|代替r作为分支的权重
 * @note 数学上已证明该技巧可以显著的加快迭代收敛,参考<<流体网络>>.
 */
static ITERATE_RESULT iterate(VentNetwork* vnet, int count, bool speedUp)
{
    // 根据最小生成树查找独立回路
    CycleList cl;
    findCycle(vnet, cl, speedUp);
    if(cl.empty()) return IR_FAILED;
    //cout<<"找独立回路成功.."<<endl;

    //当该函数退出后,CycleListSwitch对象的析构函数自动调用
    //删除独立回路对象的内存
    CycleListSwitch cl_switch(cl);

    // 迭代计算
    bool ret = false;
    int k = 0;  // 迭代次数
    while(!ret && k <= count)
    {
        DoubleArray dqArray, dfArray;     // 记录回路迭代得到的修正风量以及阻力差
        for(size_t i=0;i<cl.size();i++)
        {
            double dq = 0, df = 0;       // 回路修正风量和阻力差
            bool aa = cl[i]->iterate(dq, df);      // 每个回路进行迭代
            //cout<<"第"<<i+1<<"个回路:"<<"dq="<<dq<<" df="<<df<<endl;
            dqArray.push_back(dq);
            dfArray.push_back(df);
        }

        // 判断精度是否满足要求(风量精度和阻力精度)
		if(DataUtils::IsMeetErrReq(dqArray, DEF_PRECISE) &&
			DataUtils::IsMeetErrReq(dfArray, DEF_PRECISE))
        {
            //cout<<"第"<<k+1<<"次迭代收敛"<<endl;
            ret = true; 
			break;
        }
        else
        {
            k++;
        }
    }
    return (ret ? IR_SUCCESS : IR_UNCOVERAGE);
}

bool vno(VentNetwork* vnet)
{
    //初始化风量(包括固定风量),利用最大流固定风量分配算法分配
	//传统的风网解算程序由于没有解决"风机特性曲线振荡", "迭代加速"问题,要求风量分配要尽可能接近真实风量
	//本算法无此限制条件,风量分配的结果只要保证风量守恒即可!
    if(!initQ(vnet)) return false;

	//添加虚拟大气分支
	vnet->addAirEdge();

	//总的迭代次数
	int totalCount = 0;
	//迭代结果
	ITERATE_RESULT ret = IR_SUCCESS;
	do
	{
		//是否加速(如果20次计算后仍不收敛,开始加速)
		bool speedUp = (ret == IR_UNCOVERAGE);
		//迭代解算
		ret = iterate(vnet, DEF_CONVERGENCE_COUNT, speedUp);
		//总的迭代次数累加
		totalCount += DEF_CONVERGENCE_COUNT;
	}
	//如果经过500次迭代还是没有收敛,则退出迭代循环
	while(ret == IR_UNCOVERAGE && totalCount < DEF_MAX_COUNT);
	
	//删除虚拟大气分支
	vnet->delAirEdge();
    return (ret == IR_SUCCESS);
}
