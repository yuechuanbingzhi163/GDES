#include "vno.h"

//算法用的一些默认配置参数
#include "config.h"

#include <cmath>
#include <cstdlib>
#include <cfloat>
#include <climits>
#include <algorithm>
#include <iterator>
#include <iostream>
using namespace std;

/*****************************************向下分割线*******************************************************/
/*********************************  尝试用解非线性方程的方法,失败!**************************************/

//#include <nlopt.hpp>

/*
typedef struct {
    double a,b;
} my_constraint_data;

double myvfunc(const std::vector<double>& x,std::vector<double>& grad, void* my_func_data)
{
    if(!grad.empty())
    {
        grad[0]=0.0;
        grad[1]=0.5/sqrt(x[1]);
    }
    //cout<<"aaa"<<endl;
    return sqrt(x[1]);
}

double myvconstraint(const std::vector<double>& x, std::vector<double>& grad, void* data)
{
    my_constraint_data* d = (my_constraint_data*)data;
    double a = a, b =  b;
    if(!grad.empty())
    {
    grad[0] = 3*a*(a*x[0]+b)*(a*x[0]+b);
    grad[1]=-1.0;
   }
   return ((a*x[0]+b)*(a*x[0]+b)*(a*x[0]+b)-x[1]);
}

void test_nlopt()
{
    nlopt::opt opt(nlopt::LD_MMA, 2);
    std::vector<double> lb(2);
    lb[0]=-HUGE_VAL; lb[1]=0;
    opt.set_lower_bounds(lb);

    my_constraint_data data[2]={{2,0}, {-1,1}};

    opt.set_min_objective(myvfunc, NULL);
    opt.add_inequality_constraint(myvconstraint, &data[0],1e-8);
    opt.add_inequality_constraint(myvconstraint, &data[1],1e-8);

    opt.set_xtol_rel(1e-4);

    std::vector<double> x(2);
    x[0]=1.234; x[1]=5.678;
    double minf;
    opt.optimize(x, minf);
}

//根据独立回路计算分支风量
static double CaclQByCycle( EdgeMap& ed, CycleList& cl, Digraph::Arc e )
{
    double sq = 0;
    for(size_t i=0;i<cl.size();i++)
    {
        IndependentCycle* pCycle = cl[i];
        double baseQ = ed[pCycle->getBaseEdge()]->q; // 回路基准分支的风量(余枝风量)
        short c = pCycle->getCycleCoeff(e);             // 分支e在回路矩阵中的系数
        sq += c*baseQ;
    }
    return sq;
}

static void ReCaclQ(Digraph& dg, EdgeMap& ed, CycleList& cl, const std::vector<double>& x)
{
    //修改回路的基准分支风量
    //也就是余枝的风量(变量)
    for(size_t i=0;i<cl.size();i++)
    {
        ed[cl[i]->getBaseEdge()]->q = x[i];
    }
    //重新计算其它树枝的风量
    for(Digraph::ArcIt e(dg);e!=INVALID;++e)
    {
        ed[e]->q = CaclQByCycle(ed, cl ,e);
    }
}

//目标函数数据,记录风机对应的分支
struct airflow_func_data
{
    airflow_func_data(Digraph& _dg, EdgeMap& _ed, CycleList& _cl)
        : dg(_dg), ed(_ed), cl(_cl)
    {
    }

    Digraph& dg;
    EdgeMap& ed;
    CycleList& cl;       // 独立回路(回路的基准分支也就是余枝)
    EdgeArray fanEdges;  // 风机分支
};

//通路风压约束
struct airflow_constraint_data_H
{
    airflow_constraint_data_H(Digraph& _dg, EdgeMap& _ed, CycleList& _cl)
        : dg(_dg), ed(_ed), cl(_cl)
    {
    }

    Digraph& dg;
    EdgeMap& ed;
    CycleList& cl; // 独立回路(回路的基准分支也就是余枝)
    int k;         // 第k个风机
    EdgeArray p;   // 一条通路
};

//树枝风量约束
struct airflow_constraint_data_Q
{
    airflow_constraint_data_Q(Digraph& _dg, EdgeMap& _ed, CycleList& _cl)
        : dg(_dg), ed(_ed), cl(_cl)
    {
    }

    Digraph& dg;
    EdgeMap& ed;
    CycleList& cl;  // 独立回路(回路的基准分支也就是余枝)
    Digraph::Arc e; // 树枝分支
};

//目标函数
//  变量x=(余枝风量1,余枝风量2,余枝,....风机风压1,风机风压2,...,)
//  计算所有风机的功耗之和
double airFlowFunc(const std::vector<double>& x,std::vector<double>& grad, void* data)
{
    static long kkk = 1;
    //cout<<"第"<<kkk++<<"次目标函数计算"<<endl;

    airflow_func_data* d = (airflow_func_data*)data;
    Digraph& dg = d->dg;
    CycleList& cl = d->cl;
    EdgeMap& ed = d->ed;
    EdgeArray& fanEdges = d->fanEdges;
    //if(!grad.empty())
    //{
        //grad[0]=0.0;
        //grad[1]=0.5/sqrt(x[1]);
    //}

    //重新计算风量
    //[重点]在目标函数中已经计算过,约束函数中就不需要再重复计算了
    ReCaclQ(dg, ed, cl, x);

    for(size_t i=0;i<x.size();i++)
    {
        //cout<<x[i]<<" ";
    }
    //cout<<endl;

    //所有风机功耗之和
    double S = 0;
    size_t n = cl.size();
    for(size_t i=0;i<fanEdges.size();i++)
    {
        Digraph::Arc e = fanEdges[i];
        double hf = x[i+n];   // 风机风压
        double qf = ed[e]->q; // 风机风量(已知)
        S += abs(hf*qf);
    }
    return S;
}

//通路风压约束: {通路的分支阻力之和} - {风机的压力} <=0
double airFlowConstraint_H(const std::vector<double>& x, std::vector<double>& grad, void* data)
{
    airflow_constraint_data_H* d = (airflow_constraint_data_H*)data;
    Digraph& dg = d->dg;
    CycleList& cl = d->cl;
    EdgeMap& ed = d->ed;

    //if(!grad.empty())
    //{
    //grad[0] = 3*a*(a*x[0]+b)*(a*x[0]+b);
    //grad[1]=-1.0;
   //}

    int k = d->k;
    EdgeArray& p = d->p;

    //[疑问]计算过程中经常出现违背约束的解
    //很容易出现所有约束都不满足,算法并不向着改进方向执行的情况
    return (fPath(ed, p)-x[cl.size()+k]);
}

//风量平衡约束: {树枝分支的风量} >=0
double airFlowConstraint_Q(const std::vector<double>& x, std::vector<double>& grad, void* data)
{
    airflow_constraint_data_Q* d = (airflow_constraint_data_Q*)data;
    Digraph& dg = d->dg;
    CycleList& cl = d->cl;
    EdgeMap& ed = d->ed;

    //if(!grad.empty())
    //{
    //grad[0] = 3*a*(a*x[0]+b)*(a*x[0]+b);
    //grad[1]=-1.0;
   //}

    //[疑问]计算过程中经常出现违背约束的解
    //很容易出现所有约束都不满足,算法并不向着改进方向执行的情况
    Digraph::Arc e = d->e;
    return -1*ed[e]->q;
}

//尝试实现王惠宾<矿井通风网络理论与算法>书中提出的"两步法"(未成功)
//1)优化风量(采用非线性规划); 2)优化调节(采用关键路径法)
//风量优化问题(非线性规划)
//已知部分固定风量和风机的风量(风机风量条件是否必须???),求风机的最小功耗
//按照书上的说法,就是减少最大阻力路线上的风量,降低风机的压力
//
//由于大气分支的缘故,导致部分固定风量分支不是余枝
//(一)在约束中还应该考虑这些固定风量分支,归纳起来有如下几种约束:
//1)通路风压约束: 通路阻力和 - 风机压力 <=0
//2)风量守恒约束: 分支风量(通过余枝变量计算) >=0
//  "为了保证风量守恒,必须知道总风量,当大气分支是余枝变量的时候,
//  固定大气分支的风量,不论其它的风量如何变化,总能保证风量不超过总风量;
//  另外,本程序搜索最小树的时候总是考虑[大气分支]作为余枝变量"
//3)部分固定风量: 分支风量 = 固定值(这部分分支不是余枝变量,不是优化问题的决策变量(目前未考虑)
//(二)优化问题的决策变量(由2部分组成):
//1)余枝变量(部分固定风量分支+风机分支+其它分支)
//2)风机风压变量(回风井的风机风压):w
//(三)初始值
//初始值很关键,设置不当很容易不收敛或者出错(程序运行卡在某一块位置)
//应优先使用按需分风得到的数据
bool optimizeAirFlow()
{
    //根据最小生成树查找独立回路
    CycleList cl;
    findCycle(cl);
    if(cl.empty()) return FAILED;
    //cout<<"找独立回路成功..."<<endl;

    //该函数退出时自动析构CycleList
    CycleListSwitch cls(cl);

    //查找汇点的入边分支(风机分支)
    //[假设]风机位于回风井口的入边分支上(!!!!重要!!!!!)
    EdgeArray fanEdges;
    FindFanEdgesBySink(dg, fd, tn, fanEdges);
    //cout<<"找风机分支:"<<fanEdges.size()<<endl;

    // 回风井计算总风量,再使用回路分配风量(保证平衡)
    double Q = 0;
    for(size_t i=0;i<fanEdges.size();i++)
    {
        Digraph::Arc e = fanEdges[i];
        Q += ed[e]->q;
    }
    //cout<<"总风量Q="<<Q<<endl;
    // 初始化风量(余枝+树枝)
    initQ_Cycle(cl, Q);

    //1)构造优化对象,设置优化方法和变量个数=风机个数+余枝个数
    int np = fanEdges.size()+cl.size();
    nlopt::opt opt(nlopt::LN_COBYLA, np);
    //2)设置变量的上界和下界(默认0~无穷)
    std::vector<double> lb(np, 0), ub(np, HUGE_VAL);
    //固定风量的上界和下界是一样的
    for(size_t i=0;i<cl.size();i++)
    {
        //前提:已进行风量分配(大气分支,固定风量分支,其它分支风量都已计算)
        Digraph::Arc e = cl[i]->getBaseEdge();
        //cout<<"回路"<<i<<":"<<dg .id(e)<<endl;
        if(e==airEdge || ed[e]->fq>0)
        {
            lb[i] = ed[e]->q;
            ub[i] = ed[e]->q;
        }
    }
    opt.set_lower_bounds(lb);
    opt.set_upper_bounds(ub);
    //3)设置目标函数
    airflow_func_data* f_data = new airflow_func_data(dg,ed,cl);
    //设置风机分支(用于后续的风量计算)
    for(size_t i=0;i<fanEdges.size();i++)
    {
        Digraph::Arc e = fanEdges[i];
        f_data->fanEdges.push_back(e);
    }
    opt.set_min_objective(airFlowFunc, f_data);
    //4)构造风压约束
    //4-1)设置随机数种子
    srand(time(0));
    for(size_t i=0;i<cl.size();i++)
    {
        //路径要经过的余枝分支e
        Digraph::Arc e = cl[i]->getBaseEdge();

        //4-2)在回风井节点中随机选择一个,搜索源点到该节点的路径,且路径通过余枝e
        int k = rand()%fanEdges.size();
        //dfs搜索一条从源点到回风井且经过余枝e的通路
        EdgeArray pp;
        Digraph::Node tt = dg.source(fanEdges[k]);
        if(!DFS_Pass(dg, sn, tt, e, pp)) continue;

        //4-3)构造约束
        airflow_constraint_data_H* c_data = new airflow_constraint_data_H(dg,ed,cl);
        //确定该通路与哪个风机关联
        c_data->k = k;
        //填充通路分支
        std::copy(pp.begin(), pp.end(), std::back_inserter(c_data->p));

        //4-4)设置约束
        opt.add_inequality_constraint(airFlowConstraint_H, c_data, 1e-8);
    }
    //5)构造风量约束
    // 过滤余枝(树枝-true, 余枝-false)
    ArcFilter tf(dg, false);
    for(size_t i=0;i<cl.size();i++)
    {
        Digraph::Arc e = cl[i]->getBaseEdge();
        tf[e] = true;
    }
    // 查找余枝
    EdgeArray leftTree;
    FindEdgeByFilter(dg, tf, leftTree, false);
    for(size_t i=0;i<leftTree.size();i++)
    {
        Digraph::Arc e = leftTree[i];
        airflow_constraint_data_Q* c_data = new airflow_constraint_data_Q(dg,ed,cl);
        c_data->e = e;
        opt.add_inequality_constraint(airFlowConstraint_Q, c_data, 1e-8);
    }
    //5)设置终止条件
    opt.set_xtol_rel(1e-8);
    opt.set_ftol_rel(1e-8);
    //6)设置初始值,以当前系统的数据为初值
    std::vector<double> x;
    //cout<<"初始值:";
    //6-1)余枝变量的风量
    for(size_t i=0;i<cl.size();i++)
    {
        Digraph::Arc e = cl[i]->getBaseEdge();
        x.push_back(ed[e]->q);
        //cout<<x .back()<<" ";
    }
    //6-2)风机风压
    //    搜索一条通路计算阻力和(准确但较慢)
    for(size_t i=0;i<fanEdges.size();i++)
    {
        Digraph::Arc e = fanEdges[i];
        Digraph::Node tt = dg.source(e);
        //搜索一条到达回风井通路
        EdgeArray pp;
        Simle_DFS_Path(dg, sn, tt, pp);
        //计算风机压力变量
        x.push_back(CaclFByPath(ed, pp));
        //cout<<x .back()<<" ";
    }
    //cout<<endl;
    //7)非线性优化计算
    //cout<<"变量个数:"<<np<<endl;
    //cout<<"开始优化计算"<<endl;
    double minf;
    std::vector<double> grad;
    //cout<<"优化前总功耗:"<<airFlowFunc(x, grad, f_data)<<endl;

    //[重要]初始值的设置很关键,如果采用按需分风得到的数据,则收敛的可能性很大
    //但如果任意设置初始值,不收敛的可能性非常大(程序就卡在某一块不执行了)
    //std::vector<double> xx(np,10);
    //for(int i=0;i<np;i++)
    //{
    ////任意设置初始值
        //xx[i]=rand()%1000000;
    //}
    nlopt::result result = opt.optimize(x, minf);
    //cout<<"result:"<<result<<endl;
    if(result > 0)
    {
        //cout<<"最小值:"<<minf<<endl;
        //cout<<"优化计算成功!"<<endl;
    }
    return false;
}
*/
/*****************************************向上分割线*******************************************************/

//将回风井的公共风路设置为不可调
static void AdjustCommonEdges(VentNetwork* vnet)
{
	//虚拟汇点所有入边分支的始节点即为原始的汇点
	NodeArray tNodes;
	for(Digraph::InArcIt e(vnet->graph(),vnet->vTarget());e!=INVALID;++e)
	{
		tNodes.push_back(vnet->graph().source(e));
	}

	//利用逆向dfs查找公共风路
	EdgeArray common_edges;
	GraphUtils::RDFS_Common(vnet->graph(),tNodes,common_edges);

	//设置公共分支的调节属性为"不可调节"
	for(EdgeArray::iterator itr=common_edges.begin();itr!=common_edges.end();++itr)
	{
		Digraph::Arc e = *itr;
		vnet->edge(e)->alevel = AL_4; // AL_4表示不可调节
	}
}

/**
 * 采用通路法对一台风机进行调节(Adjust Path Method).
 * 该函数只是一个样板函数,没有完全实现,用来大概描述通路法的思想,仅作参考(代码存在错误)!!!
 * @param  vnet  [in] 单一源汇通风网络(连通且无单向回路).
 * @param  fan   [in] 一台风机所在的分支
 * @note 通路法调节的主要思路(来自刘老师的答疑):\n
 *       "以风机为单位,一个风机一个风机的调.\n
 *       注意原则:最大阻力路线不能调,次最大调完后,次最大上的道也不能再调.\n
 *       总之,后调的不能把前面的最大阻力路线给变了!".
 * @note 我的补充思路:\n
 *       针对一条风机进行调节,搜索到最大阻力路线后,计算该条路线的总阻力maxH.\n
 *       将路线上的所有分支都着色(过滤掉),然后再搜索最大阻力路线(也就是次最大阻力路线),\n
 *       计算次最大阻力路线的总阻力,与maxH的差值就是要调节的量,然后根据每条分支的"可调属性",\n
 *       查找路线上的所有可调分支,然后让用户选择"调哪几条分支,调多少"(这是一个交互过程).\n
 *       用户调节完之后返回,再将"次最大阻力路线"上的分支着色过滤,循环上述步骤,直到不能再调为止.
 * @note 在调用函数前,必须将网络转换成单一源汇网络!!!
 */
extern bool apm_sample(VentNetwork* vnet, Digraph::Arc fan);
bool apm_sample(VentNetwork* vnet, Digraph::Arc fan)
{
	//搜索从虚拟源点到风机分支末节点的最大阻力路线
	ArcFilter arc_filter(vnet->graph(), true);
	EdgeArray p;
	if(!mrp(vnet, arc_filter, vnet->vSource(), vnet->graph().target(fan), p)) return false;

	//计算最大阻力路线的总阻力
	double maxH = vnet->hPath(p);

	while(true)
	{
		//过滤掉已搜索的最大阻力路线分支(也就是着色过程)
		GraphUtils::BuildArcFilter(p, arc_filter, false);
		p.clear();

		//再次搜索着色之后的次最大阻力路线
		//如果搜索失败或者找不到最大阻力路线就跳出循环!!!
		if(!mrp(vnet, arc_filter, vnet->vSource(), vnet->graph().target(fan), p)) break;
		if(p.empty()) break;

		//计算次最大阻力路线的总阻力
		double H = vnet->hPath(p);
		//查找最大阻力路线上的所有可调分支
		EdgeArray adjustEdges;
		vnet->adjustableEdges(p, adjustEdges);
		//todo:选择可调分支进行调节
		//未实现!!!
	}
	return false;
}

bool apm_mrp(VentNetwork* vnet, ArcFilter& arc_filter, Digraph::Arc fan, 
			 IntArray& pathNums, EdgeArray& allPath, 
			 int n, double minDeltaH, double maxDeltaH)
{
	//调整负风量分支的风流方向
	//vnet->adjustNegativeEdge();

	//如果k小于等于0,则默认搜索1条最大阻力路线
	if(n <= 0) n = 1;

	//不过滤任何分支查找得到的就是最大阻力路线
	ArcFilter def_filter(vnet->graph(), true);
	//最大阻力路线
	EdgeArray maxP;
	//搜索从网络源点到风机分支始节点的最大阻力路线
	if(!mrp(vnet, def_filter, vnet->vSource(), vnet->graph().source(fan), maxP)) return false;
	//将风机分支添加到最大阻力路线中
	maxP.push_back(fan);

	//计算最大阻力路线的总阻力
	double maxH = vnet->hPath(maxP);

	//第k最大阻力路线
	//搜索成功一次就累加一次,直到最大阻力路线的条数达到n个
	int k = 1;
	while(pathNums.size() < n)
	{
		//存储搜索的次最大阻力路线
		EdgeArray p;
		//搜索第k最大阻力路线
		if(!mrp_k(vnet, arc_filter, vnet->vSource(), vnet->graph().source(fan), k, p))
		{
			//一旦第k次搜索不成功,后续的搜索同样也会失败!
			//所以这时候可以直接退出循环了
			break;
		}
		else
		{
			//搜索成功一次就累加一次
			k++;
			//搜索成功后将风机分支添加路径中
			p.push_back(fan);
			//计算次最大阻力路线的总阻力
			double H = vnet->hPath(p);
			//只考虑总阻力的差值大于tol的次最大阻力路线
			double deltaH = fabs(maxH - H);
			if(deltaH > minDeltaH && deltaH < maxDeltaH)
			{
				//记录路径的分支个数
				pathNums.push_back(p.size());
				//将最大阻力路线复制到allPath数组中
				std::copy(p.begin(), p.end(), std::back_inserter(allPath));
			}
		}
	}
	return (!pathNums.empty());
}
