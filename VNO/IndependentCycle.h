#ifndef VNO_INDEPENDENT_CYCLE_H
#define VNO_INDEPENDENT_CYCLE_H

#include "VentNetwork.h"

/**
 * 独立回路.
 * 独立回路是通风网络解算中的一个概念,也称为基本回路;
 * 通风网络中的所有回路都可以通过这些独立回路组合得到
 * 独立回路是通过查找一个生成树而求得的,非树枝称为余枝,也称为基准分支(baseEdge)
 * 独立回路是按照顺序依次排列的分支数组,基准分支的方向用于表示回路的正方向,其它分支与基本分支方向进行比较,确定的分支方向的正负号
 */
class IndependentCycle
{
public:
    //构造函数
    IndependentCycle(VentNetwork* vnet, Digraph::Arc baseEdge);

    Digraph::Arc getBaseEdge() const;              // 返回基准分支
    void addEdge(Digraph::Arc e);                  // 向回路中增加分支(排除基准分支)
    void getPath(EdgeArray& p);                    // 返回回路中的其它分支(1条路径)

    void init();                                   // 添加分支完成之后,进行初始化
    short getCycleCoeff(Digraph::Arc e);              // 求分支在回路中的方向系数(c={1, -1, 0})
    bool iterate(double& delta_q, double& delta_f);   // 迭代计算,并返回风量校正值和回路阻力代数和

private:
    void initDirection();                 // 初始化回路分支方向系数
    bool canDoIterate() const;            // 判断回路能否进行迭代计算
    bool isAirCycle() const;              // 是否大气回路
    bool isFixQCycle() const;             // 是否固定风量回路
    bool hasExtraF() const;               // 回路是否包含附加阻力(风机动力)
    bool hasFixQ() const;                 // 回路是否包含附加阻力(风机动力)
    double deltaQ() const;                // 计算回路的风量修正值
    void increaseQ(double dq);            // 修正回路风量

    double f0() const;              // 回路阻力代数和(包含基准分支)-0阶求导
    double f1() const;              // 回路阻力代数和(包含基准分支)-1阶导数
    double h0() const;              // 计算风机压力的0阶求导(如果是风机分支)
    double h1() const;              // 计算风机压力的1阶求导(如果是风机分支)
    void increaseQ(Digraph::Arc e, double q); // 修正分支风量

    double f0(Digraph::Arc e) const;     // 求分支阻力(0阶求导)
    double f1(Digraph::Arc e) const;     // 求分支阻力(1阶求导)
    double h0(Digraph::Arc e) const;     // 求风机分支的风机压力(0阶求导)
    double h1(Digraph::Arc e) const;     // 求风机分支的风机压力(1阶求导)

    double crackFan(double dq);          // 保证风机分支的风量落在"右侧",避免落在特性曲线驼峰的"左侧"

private:
    typedef std::vector<short> EdgeDirCoeffArray;  // 回路中的分支方向系数

	VentNetwork* vnet;
    Digraph::Arc baseEdge;       // 基准分支
    EdgeArray path;              // 回路的分支
    EdgeDirCoeffArray c;         // 回路分支方向系数
    bool bCanDoIterate;          // 回路是否可以进行迭代计算
};

#endif // VNO_INDEPENDENT_CYCLE_H
