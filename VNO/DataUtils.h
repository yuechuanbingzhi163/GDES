#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include "Graph.h"

/**
 * 一些数据操作的辅助方法.
 * 该类中的所有方法都是静态成员函数
 */
class  DataUtils
{
    public:
		static double MaxAbsValue(DoubleArray& dvalues);
		static double MeanSquare(DoubleArray& dvalues);
		static bool IsMeetErrReq(DoubleArray& dvalues, double precise);
		static double Magnify_Double(double v, double coeff);
};

#endif // DATA_UTILS_H