#include "vno.h"

#include <cfloat>
#include <climits>

/** 
 * 基于固定风量的权重生成器.
 * 基本思路:
 *      非固定风量分支的权重为0
 * @note 这是一个仿函数functor,提供给GraphUtils::BuildWeight()函数的第3个参数使用
 * @note 仿函数functor是现代C++中广泛使用的一种技巧,作用和函数相似,但使用范围和功能更强大!
 *
 * @code
 * // 准备数据
 * VentNetwork* vnet;
 * // 分支权重
 * Weight w(vnet->graph());
 * // 构造FixQWeightFunc对象
 * FixQWeightFunc wf(vnet);
 * GraphUtils::BuildWeight(vnet->graph(), w, wf);
 * @endcode
 */
struct FixQWeightFunc
{
	FixQWeightFunc(VentNetwork* _vnet) : vnet(_vnet), fixq_count(0)
    {
		//统计固定风量个数
		fixq_count= vnet->countFixQ();
		//此时通风网络中没有固定风量
		//如果没有特殊处理,InitFixQ返回0值,风量分配失败!
		//特殊处理方法:
		//    虚拟源点的第一条出边的风量人为固定一个数值
		//    这就使得InitFixQ方法可以正确执行了
		//    再固定分配完毕后将虚拟源点的第一条出边的固定风量恢复
		if(fixq_count==0)
		{
			// 人为固定第一条出边的风量为100
			Digraph::Arc e;
			vnet->graph().firstOut(e, vnet->vSource());
			vnet->edge(e)->fq = 100;
		}
	}
	~FixQWeightFunc()
	{
		if(fixq_count==0)
		{
			Digraph::Arc e;
			vnet->graph().firstOut(e, vnet->vSource());
			vnet->edge(e)->fq=0;
		}
	}
    double operator()(Digraph::Arc e)
    {
        double fq = vnet->edge(e)->fq;
        return (fq>0)?fq:0;
    }
	VentNetwork* vnet;
	int fixq_count;
};

bool ifq(VentNetwork* vnet)
{
	//计算分支权重
	Weight w(vnet->graph());
	GraphUtils::BuildWeight(vnet->graph(), w, FixQWeightFunc(vnet));

	//调用基于最大流的固定风量分配算法
	if(!GraphUtils::FixedMaxFlow(vnet->graph(), w, DBL_MAX))
	{
		//cout<<"分配固定风量失败..."<<endl;
		return false;
	}
	//分配计算的风量
	for(Digraph::ArcIt e(vnet->graph());e!=INVALID;++e)
	{
		vnet->edge(e)->q = w[e];
	}
	return true;
}
