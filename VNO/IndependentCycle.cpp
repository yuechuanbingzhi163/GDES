#include "IndependentCycle.h"
#include <algorithm>
#include <iterator>

#include <iostream>
using namespace std;

IndependentCycle::IndependentCycle(VentNetwork* _vnet, Digraph::Arc _baseEdge)
: vnet(_vnet), baseEdge(_baseEdge)
{
}

Digraph::Arc IndependentCycle::getBaseEdge() const
{
    return baseEdge;
}

void IndependentCycle::addEdge( Digraph::Arc e )
{
    if(e == INVALID || e == baseEdge) return;
    if(std::find(path.begin(), path.end(), e) != path.end()) return;

    path.push_back(e);
}

void IndependentCycle::getPath( EdgeArray& p )
{
    std::copy(path.begin(), path.end(), std::back_inserter(p));
}

void IndependentCycle::init()
{
    // 计算方向系数
    initDirection();

    // 判断回路能否进行迭代???
    bCanDoIterate = canDoIterate();
    //cout<<vnet->graph().id(baseEdge)<<" can iter "<<bCanDoIterate<<endl;
}

void IndependentCycle::initDirection()
{
    if(path.empty()) return;

    Digraph::Node u = vnet->graph().target(baseEdge);
    short d = 1;  // 以基准分支方向为正方向
    for(size_t i=0;i<path.size();i++)
    {
        Digraph::Arc e = path[i];
        Digraph::Node s = vnet->graph().source(e);
        Digraph::Node t = vnet->graph().target(e);
        if(d>0)
        {
            if(s != u)
            {
                d = -1*d;
                u = s;
            }
            else
            {
                u = t;
            }
        }
        else
        {
            if(t != u)
            {
                d = -1*d;
                u = t;
            }
            else
            {
                u = s;
            }
        }
        c.push_back(d);
    }
}

bool IndependentCycle::isFixQCycle() const
{
    // 固定风量大于0
    return vnet->edge(getBaseEdge())->fq > 0;
}

bool IndependentCycle::hasExtraF() const
{
    // 检查回路中是否存在附加阻力(风机)
    bool ret= false;
    for(size_t i=0;i<path.size();i++)
    {
        Digraph::Arc e = path[i];
        // 分支上有附加阻力(风机)
        if(vnet->fan(e) != 0)
        {
            ret= true; break;
        }
        //else
        //{
            // 其它的情况,例如自然风压,火风压等等
            // ???可能不太正确
            // ...
        //}
    }
    return ret;
}

double IndependentCycle::crackFan(double dq)
{
    //如果基准分支上有风机,且斜率>0
    if(h1(baseEdge) > 0) return 1000;

    for(size_t i=0;i<path.size();i++)
    {
        Digraph::Arc e = path[i];
        //如果回路分支上有风机,且斜率>0
        if(h1(e) > 0)
        {
            dq += c[i]*1000;
            break;
        }
    }
    return dq;
}

bool IndependentCycle::hasFixQ() const
{
    // 检查回路中是否存在固定风量
    bool ret= false;
    for(size_t i=0;i<path.size();i++)
    {
        Digraph::Arc e = path[i];
        // 回路上有固定风量
        if(vnet->edge(e)->fq > 0)
        {
            ret= true; break;
        }
    }
    return ret;
}

bool IndependentCycle::canDoIterate() const
{
    // 1) 基准分支是固定风量分支,不参与迭代计算
    // 2) 回路中包含固定风量分支,不参与迭代计算
    if(isFixQCycle() || hasFixQ())
    {
        //cout<<"基准分支是固定风量"<<endl;
        return false;
    }
    // 3) 如果基准分支是大气分支, 且不包含附加阻力, 则不参与迭代计算
    else if(isAirCycle() && !hasExtraF())
    {
        //cout<<"基准分支是大气分支或者没有局部阻力"<<endl;
        return false;
    }
    // 4) 如果基准分支是未使用的风机分支,且回路中没有附加阻力,则不参与迭代计算
    //else if(!hasExtraF())
    //{
        //cout<<vnet->graph().id(baseEdge)<<"  基准分支是未使用的风机分支,且回路中没有附加阻力"<<endl;
        //return false;
    //}

    return true;
}

double IndependentCycle::h0() const
{
    double s = h0(baseEdge);
    for(size_t i=0;i<path.size();i++)
    {
        Digraph::Arc e = path[i];
        s += h0(e)*c[i];
    }
    return s;
}

double IndependentCycle::h1() const
{
    double s = h1(baseEdge);;
    for(size_t i=0;i<path.size();i++)
    {
        Digraph::Arc e = path[i];
        s += h1(e)*c[i];
    }
    return s;
}

double IndependentCycle::f0() const
{
    double s = f0(baseEdge);
    for(size_t i=0;i<path.size();i++)
    {
        s += c[i]*f0(path[i]);
    }
    return s;
}

double IndependentCycle::f1() const
{
    // 一定要记住：基准分支也要参与计算
    // f1()方法的返回值始终>=0
    double s = f1(baseEdge);
    // 回路中的其它分支计算阻力1阶导数的绝对值之和
    for(size_t i=0;i<path.size();i++)
    {
        s += abs(c[i])*f1(path[i]);
    }
    return s;
}

double IndependentCycle::f0(Digraph::Arc e) const
{
    if(e == INVALID) return 0;
    return vnet->edge(e)->f0();
}

double IndependentCycle::f1(Digraph::Arc e) const
{
    if(e == INVALID) return 0;
    return vnet->edge(e)->f1();
}

double IndependentCycle::h0(Digraph::Arc e) const
{
    if(e == INVALID) return 0;
    if(vnet->fan(e) == 0) return 0;
    return vnet->fan(e)->h0(vnet->edge(e)->q);
}

double IndependentCycle::h1(Digraph::Arc e) const
{
    if(e == INVALID) return 0;
    if(vnet->fan(e) == 0) return 0;
    return vnet->fan(e)->h1(vnet->edge(e)->q);
}

double IndependentCycle::deltaQ() const
{
    // 风机动力方向与方向系数c有关
    // sq - c*hf
    // sq表示回路分支阻力代数和,hf表示风机动力(根据特性曲线计算出来的)
    // 而风机分支始终是余枝,也即c=1
    double F0 = f0();
    double H0 = h0();
    double F1 = f1();
    double H1 = h1();

    return -1.0*(F0-H0)/(F1-H1);
}

void IndependentCycle::increaseQ(double dq)
{
    // 修正基准分支风量
    increaseQ(getBaseEdge(), dq);
    // 回路中的其它分支修正风量
    for(size_t i=0;i<path.size();i++)
    {
        increaseQ(path[i], c[i]*dq);  // 修正回路分支风量
    }
}

void IndependentCycle::increaseQ(Digraph::Arc e, double dq)
{
    vnet->edge(e)->q += dq;
}

bool IndependentCycle::iterate(double& dq, double& df)
{
    //cout<<"迭代前:基准分支q(e"<<vnet->graph().id(baseEdge)<<")="<<vnet->edge(baseEdge)->q<<" f="<<f0(baseEdge)<<endl;
    dq = 0; df = 0;
    //回路不参与迭代计算
    if(!bCanDoIterate) return false;

    //回路风量修正量
    dq = deltaQ();
    //cout<<"dq="<<dq<<endl;
    //特殊处理风机分支
    //保证风机分支的风量始终在抛物线的右侧(斜率小于0的一侧)
    dq = crackFan(dq);
    //修正回路风量
    increaseQ(dq);
    //回路阻力代数和
    df = f0()-h0();

    //cout<<"基准分支q(e"<<vnet->graph().id(baseEdge)<<")="<<vnet->edge(baseEdge)->q<<" f="<<f0(baseEdge)<<endl;
    //for(int i=0;i<path.size();i++)
    //{
        //cout<<"第"<<i+1<<"个回路分支q(e"<<vnet->graph().id(path[i])<<")="<<vnet->edge(path[i])->q<<" f="<<f0(path[i])<<endl;
    //}
    //cout<<"dq="<<dq<<" df="<<df<<endl;
    return true;
}

short IndependentCycle::getCycleCoeff( Digraph::Arc e )
{
    if(e == INVALID) return 0;
    if(e == baseEdge) return 1;

    EdgeArray::iterator itr = std::find(path.begin(), path.end(), e);
    if(itr == path.end()) return 0;

    return c[itr-path.begin()];
}

bool IndependentCycle::isAirCycle() const
{
    //假设1:风阻为0的基准分支是大气分支,即为大气回路
    //假设2:虚拟汇点和原始网络汇点之间的虚拟分支不可能是基准分支!!!
    return vnet->edge(baseEdge)->isNull();
}