#ifndef VNO_CONFIG_H
#define VNO_CONFIG_H

/** 算法中用到的一些默认常量参数. */

// 默认放大比例系数
#define LARGE_COEFF 1000
// 无穷大风阻(10^5)
#define INFINITE_R 100000
// 默认收敛次数
// 如果迭代超过该次数还没有收敛,将采取加速方法
#define DEF_CONVERGENCE_COUNT 20
// 默认初始风量
#define DEF_TOTAL_Q 1000
// 默认误差精度
#define DEF_PRECISE 0.0001
// 默认最大迭代次数
#define DEF_MAX_COUNT 500
//默认最大阻力路线的阻力误差精度(10.0Pa)
#define DEF_H_PRECISE 10.0

#endif // VNO_CONFIG_H