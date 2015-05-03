#ifndef VNO_GRAPH_DATA_H
#define VNO_GRAPH_DATA_H

#include "Graph.h"

/** 分支的可调节属性. */
enum AdjustLevel
{
    AL_1 = 1,  /**< 优先调节. */
    AL_2 = 2,  /**< 可以调节. */
    AL_3 = 3,  /**< 不宜调节. */
    AL_4 = 4   /**< 不可调节. */
};

/**
 * 风机数据.
 * 目前将局扇和主扇的参数都看成是一样的,
 * 都使用2次多项式拟合: h(q)=a0+a1*q+a2*q^2
 */
class  FanData
{
    public:
		/** 构造函数. */
        FanData();
		/** 
		 * 风机压力. 
		 * @param q 风量.
		 * @return 风机压力.
		 * @note \f$h0=a_0+a_1*q+a_2*q^2\f$
		 */
        double h0(double q) const;
		/** 
		 * 风机压力1阶求导. 
		 * @param q 风量.
		 * @return 风机压力的1阶求导.
		 * @note \f$h1=a_1+2*a_2*q\f$
		 */
        double h1(double q) const;

    public:
        double a0;  /**< 2次多项式的0次参数(默认初始化为0). */
        double a1;  /**< 2次多项式的1次参数(默认初始化为0). */
        double a2;  /**< 2次多项式的2次参数(默认初始化为0). */
};

/**
 * 分支数据.
 * 分支数据包括巷道上的各种通风相关的属性,例如:摩擦风阻,风速,风量,风压等等.
 * @note 本程序的分支数据仅考虑了网络解算和优化调节所需要的基本数据,并未全部覆盖所有的巷道属性.
 * @note 要明确cvno和vno的调节风阻adjust_r之间的区别:\n
 *		(1) cvno中的adjust_r就是纯粹通路法调节的风阻detlaR;\n
 *		(2) vno中的adjust_r包括了deltaR,也包括了构筑物的风阻;\n
 * 因此,在cvno调用vno的调节模块后,要将vno中的adjust_r通过getDeltaR()提出来,再赋给cvno中的adjst_r\n
 * 为什么要这么设计？\n
 *		考虑的是vno只是一个算法模块,并不需要完全的和现实的通风网络物理模型完全一致,构筑物对分支
 *      就是起调节作用的一种设置,也能称它为调节风阻adjust_r\n
 *		而通路法优化调节过程就对分支风阻的进行调节,与构筑物风阻的作用都是一样的.
 *      所以,在vno中也构筑物的风阻都加到调节风阻adjust_r中,
 *      为了便于后续明确调节量是多少,又给分支额外增加了一个风压调节量adjust_h字段\n
 *		cvno一般是提供给外部调用的,由于cvno中设计了gate_data类型,用于绑定构筑物风阻到分支上,
 *      所以分支的adjust_r只能包括通路法调节的风量.
 */
class  EdgeData
{
    public:
		/** 构造函数. */
        EdgeData();

		/**
		 * 获取分支的摩擦风阻.
		 * @return 分支摩擦风阻.
		 */
		double getR() const;

		/** 
		 * 获取分支的总风阻.
		 * @return 分支总风阻.
		 * @note 总风阻等于摩擦风阻、调节风阻之和.
		 */
        double getSumR() const;
		
		/**
		 * 分支风阻调节量.
		 * @return 分支风阻调节量.
		 * @note adjust_r*q^2 >= adjust_h.
		 */
		double getDeltaR() const;

		/**
		 * 是否虚拟分支.
		 * @return 是否虚拟分支.
		 * @note 风阻为0的分支被视为虚拟分支.\n
		 *       虚拟分支一般是某些算法为了特定要求临时添加的无阻力、无附加动力、无构筑物的分支.
		 */
        bool isNull() const;
		/**
		 * 分支总阻力.
		 * @return 分支阻力.
		 * @note \f$f0=r*q*|q|\f$,其中r=getSumR();
		 * @note 风阻参数含义参见成员变量的注释
		 */
        double f0() const;
		/**
		 * 分支总阻力1阶求导.
		 * @return 分支阻力1阶求导.
		 * @note \f$f1=|2*r*q|\f$,其中r=getSumR();
		 */
        double f1() const;

    public:
        double r;         /**< 风阻(默认初始化为0). */
        double q;         /**< 风量(默认初始化为0). */
        double fq;        /**< 固定风量(默认初始化为0). */
        double adjust_r;  /**< 调节风阻(默认初始化为0). */
		double adjust_h;  /**< 风压调节量.        */
        double density;   /**< 空气密度(默认初始化为1.2,暂未使用). */
        AdjustLevel alevel;/**< 可调节属性(默认初始化为AL_2:可以调节). */
};

/** 
 * 节点数据.
 * @note 节点数据的用途暂时没考虑好!
 */
class NodeData
{
    public:
		/** 构造函数. */
        NodeData();

    public:
        double p;        /**< 节点压力(默认初始化为0). */
        double x,y,z;    /**< 坐标x,y,z(默认初始化为0). */
};

/**
 * 构筑物数据.
 * 构筑物指的是风门,风窗这类导风,阻风设施.
 */
class GateData
{
    public:
		/** 构造函数. */
        GateData();

    public:
        double r;     /**< 等效风阻(默认初始化为0). */
        double area;  /**< 风窗面积(默认初始化为0). */
};

/** 分支数据. */
typedef Digraph::ArcMap<EdgeData*> EdgeMap;

/** 分支上的风机数据. */
typedef Digraph::ArcMap<FanData*> FanMap;

/** 分支上的构筑物数据. */
typedef Digraph::ArcMap<GateData*> GateMap;

/** 节点数据. */
typedef Digraph::NodeMap<NodeData*> NodeMap;

#endif // VNO_GRAPH_DATA_H
